#include "synapse/runtime/p4runtime/stream/handler/standard.hpp"
#include "synapse/runtime/p4runtime/constants.hpp"
#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"
#include "synapse/runtime/p4runtime/stream/tags.hpp"
#include "synapse/runtime/p4runtime/typedefs.hpp"
#include "synapse/runtime/utils/file.hpp"
#include "synapse/runtime/wrapper/utils/wrappers.hpp"

namespace synapse::runtime {

bool flushUpdates(env_ptr_t env, p4runtime_stub_ptr_t stub) {
  p4_write_request_ptr_t request = nullptr;
  if (env->update_buffer->flush(request)) {
    env->logger->debug("Writing to the switch:");
    env->logger->debug(request->DebugString());

    grpc_cctx_t context;
    grpc::Status status =
        stub->Write(&context, *request, new p4_write_response_t());

    if (!status.ok()) {
      env->logger->error(
          "Received the following error when writing to the switch:");
      env->logger->error(status.error_details());

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
    env->logger->error("Received an error during arbitration:");
    env->logger->error(response->error().DebugString());

    return false;

  } else if (response->has_arbitration()) {
    p4::v1::MasterArbitrationUpdate arbitration = response->arbitration();
    if (grpc::StatusCode::OK == arbitration.status().code()) {
      env->logger->info("This controller is now primary");

      /**
       * Now that the controller is the primary client, we still need to install
       * the forward-pipeline configuration, as well as perform any
       * preconfiguration steps required by the controller/user.
       */
      if (!installProgram(env, stack)) {
        env->logger->error("Could not install the program on the switch");
        return false;
      }

      VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->pop());
      if (!synapse_runtime_handle_pre_configure(env)) {
        env->logger->error("Could not preconfigure the processing environment");
        return false;
      }

      if (!flushUpdates(env, S_CAST(p4runtime_stub_ptr_t, stack->top()))) {
        env->logger->error("Could not flush updates");
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
      env->logger->error("A controller already exists");
      return false;
    }

  } else {
    env->logger->error("Expected an arbitration update");
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
  // And the stream itself
  VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->pop());

  switch (response->update_case()) {
  case p4_stream_message_response_t::UpdateCase::kPacket: {
    env->logger->debug("Clearing the stack");
    env->stack->clear();

    p4_packet_in_ptr_t packet = response->mutable_packet();

    // Push packet metadata
    size_t metadataSize = packet->metadata_size();
    env->stack->push(std::malloc(metadataSize * sizeof(pair_ptr_t)));
    pair_ptr_t *metadata = S_CAST(pair_ptr_t *, env->stack->top());
    env->stack->push(new size_t(metadataSize));

    for (size_t i = 0; i < metadataSize; i++) {
      p4_packet_metadata_ptr_t entry = packet->mutable_metadata(S_CAST(int, i));

      *(metadata + i) = new pair_t(
          S_CAST(void *, new uint32_t(entry->metadata_id())),
          S_CAST(void *, new string_t(entry->mutable_value()->c_str(),
                                      entry->mutable_value()->size())));
    }

    // Push string containing the packet payload
    size_t payloadSize = packet->mutable_payload()->size();
    const char *payload = packet->mutable_payload()->c_str();
    env->stack->push(new string_t(payload, payloadSize));

    proceed = synapse_runtime_handle_packet_received(env);

    if (!flushUpdates(env, S_CAST(p4runtime_stub_ptr_t, stack->top()))) {
      env->logger->error("Could not flush updates");
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

      string_ptr_t packetOutPayload = S_CAST(string_ptr_t, env->stack->pop());
      size_t packetOutMetadataSize = *S_CAST(size_t *, env->stack->pop());
      pair_ptr_t *packetOutMetadata = S_CAST(pair_ptr_t *, env->stack->pop());

      std::vector<p4_packet_metadata_ptr_t> *packetOutMetadataVector =
          new std::vector<p4_packet_metadata_ptr_t>();

      if (0 < packetOutMetadataSize) {
        assert(nullptr != packetOutMetadata);

        for (size_t i = 0; i < packetOutMetadataSize; i++) {
          pair_ptr_t metadata = packetOutMetadata[i];
          uint32_t metadataId = *S_CAST(uint32_t *, metadata->left);
          string_ptr_t metadataValue = S_CAST(string_ptr_t, metadata->right);

          packetOutMetadataVector->push_back(env->helper->p4PacketMetadata(
              metadataId,
              std::string(metadataValue->value, metadataValue->value_sz)));
        }
      }

      // Send packet out of the controller
      stream->Write(
          *env->helper->p4StreamMessageRequest(env->helper->p4PacketOut(
              std::string(packetOutPayload->value, packetOutPayload->value_sz),
              packetOutMetadataVector)),
          nextStates[1]);

      // We cannot proceed as we would be queueing a reading!
      stack->push(stream);
      return true;
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kIdleTimeoutNotification: {
    env->stack->push(response->mutable_idle_timeout_notification());
    proceed = synapse_runtime_handle_idle_timeout_notification_received(env);
    env->stack->empty();

    if (!flushUpdates(env, S_CAST(p4runtime_stub_ptr_t, stack->top()))) {
      env->logger->error("Could not flush updates");
      return false;
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kArbitration: {
    p4_master_arbitration_update_t arbitration = response->arbitration();

    if (grpc::OK != arbitration.status().code()) {
      env->logger->error("Received an arbitration error:");
      env->logger->error(arbitration.status().message());

      // print the highest election id known to the switch
      env->logger->debug("The highest election ID is now " +
                         arbitration.election_id().low());

    } else {
      env->logger->debug("Received an arbitration update:");
      env->logger->debug(arbitration.DebugString());
      proceed = true;
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kError: {
    env->logger->error("Received an error:");
    env->logger->error(response->error().DebugString());

  } break;

  case p4_stream_message_response_t::UpdateCase::kOther:
  case p4_stream_message_response_t::UpdateCase::kDigest:
  default: {
    env->logger->error("Received an unsupported message type");
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

    env->logger->error("Could not apply pipeline configuration");
    return false;
  }

  stack->push(stub);
  stack->push(stream);

  env->logger->info("Installed the program on the switch");
  return true;
}

} // namespace synapse::runtime
