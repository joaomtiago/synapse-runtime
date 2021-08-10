#include "synapse/runtime/p4runtime/stream/handler/standard.hpp"
#include "synapse/runtime/p4runtime/constants.hpp"
#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"
#include "synapse/runtime/p4runtime/stream/tags.hpp"
#include "synapse/runtime/p4runtime/typedefs.hpp"
#include "synapse/runtime/utils/file.hpp"
#include "synapse/runtime/wrapper/utils/wrappers.hpp"
#include <inttypes.h>

#define MALLOC(var_type, var_size)                                             \
  S_CAST(var_type *, std::malloc(var_size * sizeof(var_type)))

#define VAR_MALLOC(var_type, var_name, var_size)                               \
  auto var_name = MALLOC(var_type, var_size)

#define VECTOR(var_type) new std::vector<var_type>();

#define VAR_VECTOR(var_type, var_name) auto var_name = VECTOR(var_type)

#define NOT_NULL(exp) assert(nullptr != (exp))

namespace synapse::runtime {

// Declaration of auxiliary functions

void extractPacketIn(helper_ptr_t &helper, p4_packet_in_ptr_t &packet,
                     std::string *&payload, pair_ptr_t *&meta,
                     size_t &metaSize);

bool installProgram(env_ptr_t &env, stack_ptr_t &stack);

bool flushUpdatesIfNeeded(upd_buff_ptr_t &updBuffer,
                          p4runtime_stub_ptr_t &stub);

bool mustUpdateTags(stack_ptr_t &stack);

bool updateTags(stack_ptr_t &stack, helper_ptr_t &helper,
                upd_buff_ptr_t &updBuffer, p4runtime_stub_ptr_t &stub);

bool mustSendPacketOut(stack_ptr_t &stack);

bool sendPacketOut(stack_ptr_t &stack, helper_ptr_t &helper,
                   p4_stream_message_request_ptr_t &request);

// Definition on listener handlers

bool handleConnected(env_ptr_t &env, stack_ptr_t &stack, void **&nextStates) {
  p4_stream_message_request_ptr_t request = env->helper->p4StreamMessageRequest(
      env->helper->p4MasterArbitrationUpdate(SYNAPSE_DEVICE_ID,
                                             SYNAPSE_ELECTION_ID_LOW,
                                             SYNAPSE_ELECTION_ID_HIGH));

  S_CAST(p4runtime_stream_ptr_t, stack->top())->Write(*request, nextStates[0]);

  return true;
}

bool handleMakePrimarySent(env_ptr_t &env, stack_ptr_t &stack,
                           void **&nextStates) {
  auto response = new p4_stream_message_response_t();

  S_CAST(p4runtime_stream_ptr_t, stack->top())->Read(response, nextStates[0]);
  stack->push(response);

  return true;
}

bool handleMakePrimaryReceived(env_ptr_t &env, stack_ptr_t &stack,
                               void **&nextStates) {
  VAR_S_CAST(p4_stream_message_response_ptr_t, response, stack->pop());

  if (response->has_error()) {
    SYNAPSE_ERROR("Error during arbitration:");
    SYNAPSE_ERROR("%.*s", (int)response->mutable_error()->DebugString().size(),
                  response->mutable_error()->DebugString().c_str());

    return false;

  } else if (response->has_arbitration()) {
    auto arbitration = response->mutable_arbitration();
    if (grpc::StatusCode::OK == arbitration->mutable_status()->code()) {
      SYNAPSE_DEBUG("Controller is the primary client of the switch");

      /**
       * Now that the controller is the primary client, we still need to install
       * the forward-pipeline configuration, as well as perform any
       * preconfiguration steps required by the controller/user.
       */
      if (!installProgram(env, stack)) {
        SYNAPSE_ERROR("Could not install the program on the switch");
        return false;
      }

      VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->pop());
      if (!synapse_runtime_handle_pre_configure(env)) {
        SYNAPSE_ERROR("Failed to configure the environment");
        return false;
      }

      VAR_S_CAST(p4runtime_stub_ptr_t, stub, stack->top());
      if (!flushUpdatesIfNeeded(env->update_buffer, stub)) {
        SYNAPSE_ERROR("Could not flush update(s)");
        return false;
      }

      if (mustUpdateTags(env->stack)) {
        SYNAPSE_DEBUG("Updating tags");

        if (!updateTags(env->stack, env->helper, env->update_buffer, stub)) {
          SYNAPSE_ERROR("Could not update tags");
          return false;
        }

        SYNAPSE_DEBUG("Updated tags");
      }

      // We're done with the configuration, queue the first read!
      auto response = new p4_stream_message_response_t();
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

bool handleMessageReceivedArbitration(
    p4_master_arbitration_update_ptr_t arbitration) {
  NOT_NULL(arbitration);

  if (grpc::OK != arbitration->mutable_status()->code()) {
    SYNAPSE_ERROR("Error during arbitration:");
    SYNAPSE_ERROR("%.*s",
                  (int)arbitration->mutable_status()->mutable_message()->size(),
                  arbitration->mutable_status()->mutable_message()->c_str());
    SYNAPSE_DEBUG("The highest election ID is now %" SCNu64,
                  arbitration->mutable_election_id()->low());
    return false;

  } else {
    SYNAPSE_INFO("Received arbitration update:");
    SYNAPSE_INFO("%.*s", (int)arbitration->DebugString().size(),
                 arbitration->DebugString().c_str());
  }

  return true;
}

bool handleMessageReceivedError(p4_stream_error_ptr_t error) {
  NOT_NULL(error);

  SYNAPSE_ERROR("Received an error:");
  SYNAPSE_ERROR("%.*s", (int)error->DebugString().size(),
                error->DebugString().c_str());

  return true;
}

bool handleMessageReceivedIdleTimeoutNotification(
    env_ptr_t &env, p4runtime_stub_ptr_t &stub,
    p4_idle_timeout_notification_ptr_t notification) {
  NOT_NULL(env);
  NOT_NULL(stub);
  NOT_NULL(notification);

  auto entriesSize = S_CAST(size_t, notification->table_entry_size());
  VAR_MALLOC(p4_table_entry_ptr_t, entries, entriesSize);
  NOT_NULL(entries);

  for (size_t i = 0; i < entriesSize; i++) {
    entries[i] = notification->mutable_table_entry(S_CAST(int, i));
    NOT_NULL(entries[i]);
  }

  env->stack->clear();
  env->stack->push(entries);
  env->stack->push(new size_t(entriesSize));

  if (!synapse_runtime_handle_idle_timeout_notification_received(env)) {
    return false;
  }

  if (!flushUpdatesIfNeeded(env->update_buffer, stub)) {
    SYNAPSE_ERROR("Could not flush update(s)");
    return false;
  }

  if (mustUpdateTags(env->stack)) {
    if (!updateTags(env->stack, env->helper, env->update_buffer, stub)) {
      SYNAPSE_ERROR("Could not update tags");
      return false;
    }
  }

  return true;
}

bool handleMessageReceivedPacket(env_ptr_t &env, p4runtime_stub_ptr_t &stub,
                                 p4_packet_in_ptr_t packet) {
  NOT_NULL(env);
  NOT_NULL(stub);
  NOT_NULL(packet);

  std::string *payloadIn = nullptr;
  pair_ptr_t *metaIn = nullptr;
  size_t metaInSize;

  extractPacketIn(env->helper, packet, payloadIn, metaIn, metaInSize);

  env->stack->clear();
  env->stack->push(metaIn);
  env->stack->push(new size_t(metaInSize));
  env->stack->push(new string_t(payloadIn));

  if (!synapse_runtime_handle_packet_received(env)) {
    return false;
  }

  if (!flushUpdatesIfNeeded(env->update_buffer, stub)) {
    SYNAPSE_ERROR("Could not flush update(s)");
    return false;
  }

  if (mustUpdateTags(env->stack)) {
    SYNAPSE_DEBUG("Updating tags");

    if (!updateTags(env->stack, env->helper, env->update_buffer, stub)) {
      SYNAPSE_ERROR("Could not update tags");
      return false;
    }

    SYNAPSE_DEBUG("Updated tags");
  }

  return true;
}

bool handleMessageReceived(env_ptr_t &env, stack_ptr_t &stack,
                           void **&nextStates) {
  /**
   * The stack must look like this:
   * ^
   * | stream message response
   * | stream
   * | stub
   * |-------------------------------------
   */
  assert(3 == stack->size());

  VAR_S_CAST(p4_stream_message_response_ptr_t, response, stack->pop());
  NOT_NULL(response);

  VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->pop());
  NOT_NULL(stream);

  VAR_S_CAST(p4runtime_stub_ptr_t, stub, stack->top());
  NOT_NULL(stub);

  switch (response->update_case()) {
  case p4_stream_message_response_t::UpdateCase::kPacket: {
    if (!handleMessageReceivedPacket(env, stub, response->mutable_packet())) {
      return false;
    }

    if (mustSendPacketOut(env->stack)) {
      p4_stream_message_request_ptr_t request = nullptr;
      if (!sendPacketOut(env->stack, env->helper, request)) {
        SYNAPSE_ERROR("Could not send packet out of the switch");
        return false;
      }

      stream->Write(*request, nextStates[1]);
      stack->push(stream);
      return true; // Return here to prevent queueing a reading
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kIdleTimeoutNotification: {
    if (!handleMessageReceivedIdleTimeoutNotification(
            env, stub, response->mutable_idle_timeout_notification())) {
      return false;
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kArbitration: {
    if (!handleMessageReceivedArbitration(response->mutable_arbitration())) {
      return false;
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kError: {
    if (!handleMessageReceivedError(response->mutable_error())) {
      return false;
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kOther:
  case p4_stream_message_response_t::UpdateCase::kDigest:
  default: {
    SYNAPSE_ERROR("Received an unsupported message type");
  } break;
  }

  stream->Read(response, nextStates[0]);
  stack->push(stream);
  stack->push(response);

  return true;
}

bool handleMessageSent(env_ptr_t &env, stack_ptr_t &stack, void **&nextStates) {
  /**
   * Every time this function executes, the stack contains the following:
   * ^
   * | stream
   * | stub
   * |-------------------------------------
   */
  assert(2 == stack->size());

  auto response = new p4_stream_message_response_t();

  // Nothing to be done, queue the next reading
  VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->top());
  stream->Read(response, nextStates[0]);
  stack->push(response);

  return true;
}

// Definition of auxiliary functions

void extractPacketIn(helper_ptr_t &helper, p4_packet_in_ptr_t &packet,
                     std::string *&payload, pair_ptr_t *&meta,
                     size_t &metaSize) {
  NOT_NULL(helper);
  NOT_NULL(packet);

  payload = packet->mutable_payload();
  NOT_NULL(payload);

  SYNAPSE_INFO("Receiving packet with size %lu B", payload->size());
  SYNAPSE_INFO("\n%s", packet->DebugString().c_str());

  metaSize = packet->metadata_size();
  meta = MALLOC(pair_ptr_t, metaSize);
  NOT_NULL(meta);

  static p4_info_controller_packet_metadata_ptr_t metaInfo = nullptr;
  if (nullptr == metaInfo) {
    metaInfo = helper->p4InfoControllerPacketMetadata("packet_in");
    NOT_NULL(metaInfo);
  }

  for (size_t i = 0; i < metaSize; i++) {
    auto entry = packet->mutable_metadata(S_CAST(int, i));
    NOT_NULL(entry);

    auto metaName = helper
                        ->p4InfoControllerPacketMetadataMetadataById(
                            metaInfo, entry->metadata_id())
                        ->mutable_name();

    meta[i] = new pair_t(new string_t(metaName),
                         new string_t(entry->mutable_value()));
  }
}

bool mustSendPacketOut(stack_ptr_t &stack) {
  /**
   * If not empty, the environment stack must contain all information
   * necessary to send a packet back to the switch, in this order:
   * ^
   * | payload size + payload
   * | metadata size
   * | metadata
   * |-----------------------
   */
  return stack->empty() ? false : 3 <= stack->size();
}

bool sendPacketOut(stack_ptr_t &stack, helper_ptr_t &helper,
                   p4_stream_message_request_ptr_t &request) {
  auto payloadOut = S_CAST(string_ptr_t, stack->pop());
  NOT_NULL(payloadOut);

  auto metaOutSizePtr = S_CAST(size_t *, stack->pop());
  NOT_NULL(metaOutSizePtr);
  auto metaOutSize = *metaOutSizePtr;

  auto metaOut = S_CAST(pair_ptr_t *, stack->pop());
  NOT_NULL(metaOut);

  static p4_info_controller_packet_metadata_ptr_t metaInfo = nullptr;
  if (nullptr == metaInfo) {
    metaInfo = helper->p4InfoControllerPacketMetadata("packet_out");
    NOT_NULL(metaInfo);
  }

  VAR_VECTOR(p4_packet_metadata_ptr_t, packetOutMetadataVector);
  for (size_t i = 0; i < metaOutSize; i++) {
    auto entry = metaOut[i];
    NOT_NULL(entry);

    auto metaName = S_CAST(string_ptr_t, entry->left);
    NOT_NULL(metaName);

    // Get the metadata ID
    auto metaId = helper
                      ->p4InfoControllerPacketMetadataMetadataByName(
                          metaInfo, metaName->toStdString())
                      ->id();

    auto metaValue = S_CAST(string_ptr_t, entry->right);
    NOT_NULL(metaValue);

    auto metaPtr = helper->p4PacketMetadata(metaId, metaValue->toStdString());
    NOT_NULL(metaPtr);

    packetOutMetadataVector->push_back(metaPtr);
  }

  // Send packet out of the controller
  auto packetOut =
      helper->p4PacketOut(payloadOut->toStdString(), packetOutMetadataVector);
  NOT_NULL(packetOut);

  SYNAPSE_INFO("Writing packet with size %lu B", payloadOut->sz);
  SYNAPSE_INFO("\n%s", packetOut->DebugString().c_str());

  request = helper->p4StreamMessageRequest(packetOut);
  NOT_NULL(request);

  return true;
}

bool mustUpdateTags(stack_ptr_t &stack) {
  /**
   * If not empty, the environment stack must always contain information about
   * the new values of table tags, in this order:
   * ^
   * | tags size
   * | tags
   * | ...
   */
  return stack->empty() ? false : 2 <= stack->size();
}

/**
 * TODO Complete the documentation on this function
 */
bool updateTags(stack_ptr_t &stack, helper_ptr_t &helper,
                upd_buff_ptr_t &updBuffer, p4runtime_stub_ptr_t &stub) {
  auto tagsSizePtr = S_CAST(size_t *, stack->pop());
  NOT_NULL(tagsSizePtr);
  auto tagsSize = *tagsSizePtr;

  auto tags = S_CAST(pair_ptr_t *, stack->pop());
  NOT_NULL(tags);

  auto tableInfo = helper->p4InfoTable("MyIngress.tag_control");
  auto tableId = tableInfo->mutable_preamble()->id();

  auto actionInfo = helper->p4InfoAction("MyIngress.tag_versions");
  auto actionId = actionInfo->mutable_preamble()->id();

  VAR_VECTOR(p4_action_param_ptr_t, actionParams);
  for (size_t i = 0; i < tagsSize; i++) {
    auto tag = tags[i];

    auto tagName = S_CAST(string_ptr_t, tag->left);
    auto tagValue = S_CAST(p4_uint32_ptr_t, tag->right);

    auto tagId =
        helper->p4InfoActionParam(actionInfo, tagName->toStdString())->id();

    actionParams->push_back(
        helper->p4ActionParam(tagId, tagValue->raw->toStdString()));
  }

  static bool firstUpdate = true;
  p4_update_type_t updateType;

  if (firstUpdate) {
    SYNAPSE_INFO("Updating tags for the first time");
    updateType = p4_update_type_t::Update_Type_INSERT;
    firstUpdate = false;

  } else {
    updateType = p4_update_type_t::Update_Type_MODIFY;
  }

  return updBuffer->buffer(helper->p4Update(
             updateType, helper->p4Entity(helper->p4TableEntry(
                             tableId, new std::vector<p4_field_match_ptr_t>(),
                             helper->p4TableAction(
                                 helper->p4Action(actionId, actionParams)),
                             0, 0)))) &&
         flushUpdatesIfNeeded(updBuffer, stub);
}

bool flushUpdatesIfNeeded(upd_buff_ptr_t &updBuffer,
                          p4runtime_stub_ptr_t &stub) {
  p4_write_request_ptr_t request;
  if (updBuffer->flush(request)) {
    SYNAPSE_INFO("Flushing update(s) to the switch");
    SYNAPSE_DEBUG("%.*s", (int)request->DebugString().size(),
                  request->DebugString().c_str());

    grpc_cctx_t context;
    grpc_status_t status =
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

/**
 * This auxiliary function tries to install the forwarding pipeline
 * configuration onto the P4 switch. If the call fails, the listener is
 * interrupted.
 */
bool installProgram(env_ptr_t &env, stack_ptr_t &stack) {
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
