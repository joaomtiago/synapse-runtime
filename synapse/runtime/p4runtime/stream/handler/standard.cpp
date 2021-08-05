#include "synapse/runtime/p4runtime/stream/handler/standard.hpp"
#include "synapse/runtime/p4runtime/constants.hpp"
#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"
#include "synapse/runtime/p4runtime/stream/tags.hpp"
#include "synapse/runtime/p4runtime/typedefs.hpp"
#include "synapse/runtime/utils/file.hpp"
#include "synapse/runtime/wrapper/utils/wrappers.hpp"
#include <inttypes.h>

#define MALLOC(var_type, var_name, var_size)                                   \
  var_type *var_name =                                                         \
      S_CAST(var_type *, std::malloc(var_size * sizeof(var_type)))

#define VECTOR(var_type, var_name)                                             \
  std::vector<var_type> *var_name = new std::vector<var_type>();

#define NOT_NULL(exp) assert(NULL != (exp))

namespace synapse::runtime {

bool flushUpdates(env_ptr_t env, p4runtime_stub_ptr_t stub) {
  p4_write_request_ptr_t request = nullptr;
  if (env->update_buffer->flush(request)) {
    SYNAPSE_INFO("Flushing update(s) to the switch");
    SYNAPSE_DEBUG("%.*s", (int)request->DebugString().size(),
                  request->DebugString().c_str());

    grpc_cctx_t context;
    grpc::Status status =
        stub->Write(&context, *request, new p4_write_response_t());

    if (!status.ok()) {
      SYNAPSE_ERROR("Error while writing to the switch:");
      SYNAPSE_ERROR("%.*s", (int)status.error_details().size(),
                    status.error_details().c_str());

      return false;
    }
  }

  return true;
}

bool handleConnected(env_ptr_t env, stack_ptr_t stack, void **nextStates) {
  p4_stream_message_request_ptr_t request = env->helper->p4StreamMessageRequest(
      env->helper->p4MasterArbitrationUpdate(SYNAPSE_DEVICE_ID,
                                             SYNAPSE_ELECTION_ID_LOW,
                                             SYNAPSE_ELECTION_ID_HIGH));

  S_CAST(p4runtime_stream_ptr_t, stack->top())->Write(*request, nextStates[0]);

  return true;
}

bool handleMakePrimarySent(env_ptr_t env, stack_ptr_t stack,
                           void **nextStates) {
  p4_stream_message_response_ptr_t response =
      new p4_stream_message_response_t();

  S_CAST(p4runtime_stream_ptr_t, stack->top())->Read(response, nextStates[0]);
  stack->push(response);

  return true;
}

bool installProgram(env_ptr_t env, stack_ptr_t stack);

bool handleMakePrimaryReceived(env_ptr_t env, stack_ptr_t stack,
                               void **nextStates) {
  VAR_S_CAST(p4_stream_message_response_ptr_t, response, stack->pop());

  if (response->has_error()) {
    SYNAPSE_ERROR("Error during arbitration:");
    SYNAPSE_ERROR("%.*s", (int)response->error().DebugString().size(),
                  response->error().DebugString().c_str());

    return false;

  } else if (response->has_arbitration()) {
    p4::v1::MasterArbitrationUpdate arbitration = response->arbitration();
    if (grpc::StatusCode::OK == arbitration.status().code()) {
      SYNAPSE_DEBUG("Controller is the primary client of the switch");

      /**
       * Now that the controller is the primary client, we still need to install
       * the forward-pipeline configuration, as well as perform any
       * preconfiguration steps required by the controller/user.
       */
      if (!installProgram(env, stack)) {
        SYNAPSE_ERROR("Could not install the program on the switch...");
        return false;
      }

      VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->pop());
      if (!synapse_runtime_handle_pre_configure(env)) {
        SYNAPSE_ERROR("Failed to configure the environment");
        return false;
      }

      if (!flushUpdates(env, S_CAST(p4runtime_stub_ptr_t, stack->top()))) {
        SYNAPSE_ERROR("Could not flush update(s)");
        return false;
      }

      // We're done with the configuration, queue the first read!
      p4_stream_message_response_ptr_t response =
          new p4_stream_message_response_t();
      stream->Read(response, nextStates[0]);

      stack->push(stream);
      stack->push(response);
      assert(3 == stack->size());

      return true;

    } else {
      SYNAPSE_ERROR("A primary client of the switch already exists");
      return false;
    }

  } else {
    SYNAPSE_ERROR("Unexpected message type (expecting arbitration update)");
    return false;
  }
}

bool handleMessageReceived(env_ptr_t env, stack_ptr_t stack,
                           void **nextStates) {
  /**
   * Every time this function executes, the stack contains the following:
   * ^
   * | stream message response (pointer to)
   * | stream (pointer to)
   * | stub (pointer to)
   * |-------------------------------------
   */
  assert(3 == stack->size());

  // By default, assume processing must stop
  bool proceed = false;

  // Pop stream response from the stack
  VAR_S_CAST(p4_stream_message_response_ptr_t, response, stack->pop());
  NOT_NULL(response);

  // And the stream itself
  VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->pop());
  NOT_NULL(stream);

  switch (response->update_case()) {
  case p4_stream_message_response_t::UpdateCase::kPacket: {
    env->stack->clear();

    p4_packet_in_ptr_t packetIn = response->mutable_packet();
    NOT_NULL(packetIn);

    std::string *payloadIn = packetIn->mutable_payload();
    NOT_NULL(payloadIn);

    size_t metaInSize = packetIn->metadata_size();
    MALLOC(pair_ptr_t, metaIn, metaInSize);
    NOT_NULL(metaIn);

    for (size_t i = 0; i < metaInSize; i++) {
      p4_packet_metadata_ptr_t entry =
          packetIn->mutable_metadata(S_CAST(int, i));
      NOT_NULL(entry);

      metaIn[i] = new pair_t(new uint32_t(entry->metadata_id()),
                             new string_t(entry->mutable_value()));
    }

    SYNAPSE_INFO("Receiving packet with size %lu B", payloadIn->size());
    SYNAPSE_INFO("\n%s", packetIn->DebugString().c_str());

    env->stack->push(metaIn);
    env->stack->push(new size_t(metaInSize));
    env->stack->push(new string_t(payloadIn));

    proceed = synapse_runtime_handle_packet_received(env);

    if (!flushUpdates(env, S_CAST(p4runtime_stub_ptr_t, stack->top()))) {
      SYNAPSE_ERROR("Could not flush update(s)");
      return false;
    }

    /**
     * If the environment stack is not empty, then it must contain all the
     * necessary information to send a packet to the switch, namely (in this
     * order):
     * ^
     * | payload size + payload
     * | metadata size
     * | metadata
     * |-----------------------
     */

    if (!env->stack->empty()) {
      assert(3 == env->stack->size());

      string_ptr_t payloadOut = S_CAST(string_ptr_t, env->stack->pop());
      NOT_NULL(payloadOut);

      size_t metaOutSize = *S_CAST(size_t *, env->stack->pop());
      pair_ptr_t *metaOut = S_CAST(pair_ptr_t *, env->stack->pop());
      NOT_NULL(metaOut);

      VECTOR(p4_packet_metadata_ptr_t, packetOutMetadataVector);
      for (size_t i = 0; i < metaOutSize; i++) {
        pair_ptr_t metaEntry = metaOut[i];
        NOT_NULL(metaEntry);

        uint32_t *entryId = S_CAST(uint32_t *, metaEntry->left);
        NOT_NULL(entryId);

        string_ptr_t entryVal = S_CAST(string_ptr_t, metaEntry->right);
        NOT_NULL(entryVal);

        p4_packet_metadata_ptr_t metaPtr =
            env->helper->p4PacketMetadata(*entryId, entryVal->toStdString());
        NOT_NULL(metaPtr);

        packetOutMetadataVector->push_back(metaPtr);
      }

      // Send packet out of the controller
      p4_packet_out_ptr_t packetOut = env->helper->p4PacketOut(
          payloadOut->toStdString(), packetOutMetadataVector);
      NOT_NULL(packetOut);

      SYNAPSE_INFO("Writing packet with size %lu B", payloadOut->size);
      SYNAPSE_INFO("\n%s", packetOut->DebugString().c_str());

      p4_stream_message_request_ptr_t request =
          env->helper->p4StreamMessageRequest(packetOut);
      NOT_NULL(request);

      stream->Write(*request, nextStates[1]);

      // We cannot proceed as we would be queueing a reading!
      stack->push(stream);
      return true;
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kIdleTimeoutNotification: {
    env->stack->clear();

    p4_idle_timeout_notification_ptr_t notification =
        response->mutable_idle_timeout_notification();
    NOT_NULL(notification);

    size_t entriesSize = notification->table_entry_size();
    MALLOC(p4_table_entry_ptr_t, entries, entriesSize);
    NOT_NULL(entries);

    for (size_t i = 0; i < entriesSize; i++) {
      entries[i] = notification->mutable_table_entry(S_CAST(int, i));
      NOT_NULL(entries[i]);
    }

    env->stack->push(entries);
    env->stack->push(new size_t(entriesSize));

    proceed = synapse_runtime_handle_idle_timeout_notification_received(env);

    if (!flushUpdates(env, S_CAST(p4runtime_stub_ptr_t, stack->top()))) {
      SYNAPSE_ERROR("Could not flush update(s)");
      return false;
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kArbitration: {
    p4_master_arbitration_update_t arbitration = response->arbitration();

    if (grpc::OK != arbitration.status().code()) {
      SYNAPSE_ERROR("Error during arbitration:");
      SYNAPSE_ERROR("%.*s", (int)arbitration.status().message().size(),
                    arbitration.status().message().c_str());
      SYNAPSE_DEBUG("The highest election ID is now %" SCNu64,
                    arbitration.election_id().low());

    } else {
      SYNAPSE_INFO("Received arbitration update:");
      SYNAPSE_INFO("%.*s", (int)arbitration.DebugString().size(),
                   arbitration.DebugString().c_str());
      proceed = true;
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kError: {
    SYNAPSE_ERROR("Received an error:");
    SYNAPSE_ERROR("%.*s", (int)response->error().DebugString().size(),
                  response->error().DebugString().c_str());

  } break;

  case p4_stream_message_response_t::UpdateCase::kOther:
  case p4_stream_message_response_t::UpdateCase::kDigest:
  default: {
    SYNAPSE_ERROR("Received an unsupported message type");
    proceed = true;

  } break;
  }

  if (proceed) {
    stream->Read(response, nextStates[0]);
    stack->push(stream);
    stack->push(response);

    return true;
  }

  return false;
}

bool handleMessageSent(env_ptr_t env, stack_ptr_t stack, void **nextStates) {
  /**
   * Every time this function executes, the stack contains the following:
   * ^
   * | stream (pointer to)
   * | stub (pointer to)
   * |-------------------------------------
   */
  assert(2 == stack->size());

  p4_stream_message_response_ptr_t response =
      new p4_stream_message_response_t();

  // Nothing to be done, queue the next reading
  VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->top());
  stream->Read(response, nextStates[0]);
  stack->push(response);

  return true;
}

/**
 * This auxiliary function tries to install the forwarding pipeline
 * configuration onto the P4 switch. If the call fails, the listener is
 * interrupted.
 */
bool installProgram(env_ptr_t env, stack_ptr_t stack) {
  VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->pop());
  VAR_S_CAST(p4runtime_stub_ptr_t, stub, stack->pop());
  VAR_S_CAST(std::string *, bmv2JsonFilepath, stack->pop());

  grpc_cctx_t context;
  p4_set_forwarding_pipeline_config_response_t response;

  if (!stub->SetForwardingPipelineConfig(
               &context,
               *env->helper->p4SetForwardingPipelineConfigRequest(
                   SYNAPSE_DEVICE_ID, SYNAPSE_ELECTION_ID_LOW,
                   SYNAPSE_ELECTION_ID_HIGH,
                   p4_set_forwarding_pipeline_config_request_action_t::
                       SetForwardingPipelineConfigRequest_Action_VERIFY_AND_COMMIT,
                   env->helper->p4ForwardingPipelineConfig(
                       env->helper->p4InfoP4Info(),
                       readFile(bmv2JsonFilepath))),
               &response)
           .ok()) {

    SYNAPSE_ERROR("Failed to set the forwarding pipeline configuration");
    return false;
  }

  stack->push(stub);
  stack->push(stream);

  SYNAPSE_INFO("Installed the P4 program on the switch");
  return true;
}

} // namespace synapse::runtime
