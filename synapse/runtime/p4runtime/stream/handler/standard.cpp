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
  auto var_name = MALLOC(var_type, var_size);                                  \
  assert(nullptr != var_name);

#define VECTOR(var_type) new std::vector<var_type>();

#define VAR_VECTOR(var_type, var_name) auto var_name = VECTOR(var_type)

namespace synapse::runtime {

// Auxiliary functions

bool installProgram(helper_ptr_t &helper, stack_ptr_t &stack) {
  VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->pop());
  VAR_S_CAST(p4runtime_stub_ptr_t, stub, stack->pop());
  VAR_S_CAST(std::string *, bmv2JsonFilepath, stack->pop());
  stack->push(stub);
  stack->push(stream);

  grpc_cctx_t context;
  p4_set_forwarding_pipeline_config_response_t response;

  if (!stub->SetForwardingPipelineConfig(
               &context,
               *helper->p4SetForwardingPipelineConfigRequest(
                   SYNAPSE_DEVICE_ID, SYNAPSE_ELECTION_ID_LOW,
                   SYNAPSE_ELECTION_ID_HIGH,
                   p4_set_forwarding_pipeline_config_request_action_t::
                       SetForwardingPipelineConfigRequest_Action_VERIFY_AND_COMMIT,
                   helper->p4ForwardingPipelineConfig(
                       helper->p4InfoP4Info(), readFile(bmv2JsonFilepath))),
               &response)
           .ok()) {

    SYNAPSE_ERROR("Failed to set the forwarding pipeline configuration");
    return false;
  }

  SYNAPSE_INFO("Installed the P4 program on the switch");
  return true;
}

bool mustParseIdleTimeoutNotification(
    p4_stream_message_response_ptr_t &response) {
  return nullptr != response &&
         p4_stream_message_response_t::UpdateCase::kIdleTimeoutNotification ==
             response->update_case();
}

bool parseIdleTimeoutNotification(env_ptr_t &env,
                                  p4_idle_timeout_notification_ptr_t notif) {
  auto entriesSz = notif->table_entry_size();
  p4_table_entry_ptr_t *entries = nullptr;

  if (entriesSz > 0) {
    if (nullptr == (entries = MALLOC(p4_table_entry_ptr_t, entriesSz))) {
      return false;
    }

    p4_table_entry_ptr_t entry = nullptr;
    for (size_t i = 0;
         0 < entriesSz - i &&
         nullptr != (entry = notif->mutable_table_entry(S_CAST(int, i)));
         i++) {
      entries[i] = entry;
    }

    env->stack->push(entries);
    env->stack->push(new size_t(entriesSz));
  }

  return true;
}

bool mustParsePacket(p4_stream_message_response_ptr_t &response) {
  return nullptr != response &&
         p4_stream_message_response_t::UpdateCase::kPacket ==
             response->update_case();
}

bool parsePacket(env_ptr_t &env, p4_packet_in_ptr_t packet) {
  auto payload = new string_t(packet->mutable_payload());
  auto metaSz = packet->metadata_size();
  pair_ptr_t *meta = nullptr;

  if (metaSz > 0) {
    if (nullptr == (meta = MALLOC(pair_ptr_t, metaSz))) {
      return false;
    }

    static p4_info_controller_packet_metadata_ptr_t metaInfo = nullptr;
    if (nullptr == metaInfo) {
      metaInfo = env->helper->p4InfoControllerPacketMetadata("packet_in");
      assert(nullptr != metaInfo);
    }

    p4_packet_metadata_ptr_t entry = nullptr;
    for (size_t i = 0;
         0 < metaSz - i &&
         nullptr != (entry = packet->mutable_metadata(S_CAST(int, i)));
         i++) {
      meta[i] = new pair_t(
          new string_t(env->helper
                           ->p4InfoControllerPacketMetadataMetadataById(
                               metaInfo, entry->metadata_id())
                           ->mutable_name()),
          new string_t(entry->mutable_value()));
    }
  }

  env->stack->push(meta);
  env->stack->push(new size_t(metaSz));
  env->stack->push(payload);

  return true;
}

bool mustFlushUpdates(update_queue_ptr_t &queue) { return !queue->empty(); }

bool flushUpdates(update_queue_ptr_t &queue, p4runtime_stub_ptr_t &stub) {
  p4_write_request_ptr_t request;
  if (!queue->flush(request) || nullptr == request) {
    return false;
  }

  SYNAPSE_DEBUG("Flushing update(s) to the switch");
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

  return true;
}

bool mustUpdateTags(stack_ptr_t &stack, size_t *tagsSz, pair_ptr_t **tags) {
  if (stack->size() < 2) {
    return false;
  }

  if (0 == (*tagsSz = *S_CAST(size_t *, stack->pop()))) {
    stack->pop();
    return false;
  }

  return nullptr != (*tags = S_CAST(pair_ptr_t *, stack->pop()));
}

bool updateTags(env_ptr_t &env, p4runtime_stub_ptr_t &stub, size_t tagsSz,
                pair_ptr_t *tags) {
  auto helper = env->helper;
  auto queue = env->queue;

  static auto tableInfo = helper->p4InfoTable("SyNAPSE_Ingress.tag_control");
  static auto tableId = tableInfo->mutable_preamble()->id();

  static auto actionInfo = helper->p4InfoAction("SyNAPSE_Ingress.tag_versions");
  static auto actionId = actionInfo->mutable_preamble()->id();

  VAR_VECTOR(p4_action_param_ptr_t, actionParams);
  for (size_t i = 0; i < tagsSz; i++) {
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

  return queue->queue(helper->p4Update(
             updateType, helper->p4Entity(helper->p4TableEntry(
                             tableId, new std::vector<p4_field_match_ptr_t>(),
                             helper->p4TableAction(
                                 helper->p4Action(actionId, actionParams)),
                             0, 0)))) &&
         mustFlushUpdates(queue) && flushUpdates(queue, stub);
}

bool installProgram(helper_ptr_t &helper, stack_ptr_t &stack);

bool mustPreparePacketOut(env_ptr_t &env, string_ptr_t *payload, size_t *metaSz,
                          pair_ptr_t **meta) {
  if (env->stack->size() >= 3) {
    *payload = S_CAST(string_ptr_t, env->stack->pop());
    *metaSz = *S_CAST(size_t *, env->stack->pop());
    *meta = S_CAST(pair_ptr_t *, env->stack->pop());

    return nullptr != payload && (*metaSz > 0 ? nullptr != meta : true);
  }

  return false;
}

bool preparePacketOut(helper_ptr_t &helper,
                      p4_stream_message_request_ptr_t &request,
                      string_ptr_t payload, size_t metaSz, pair_ptr_t *meta) {
  VAR_VECTOR(p4_packet_metadata_ptr_t, pktMeta);

  if (metaSz > 0) {
    static p4_info_controller_packet_metadata_ptr_t metaInfo = nullptr;
    if (nullptr == metaInfo) {
      metaInfo = helper->p4InfoControllerPacketMetadata("packet_out");
      assert(nullptr != metaInfo);
    }

    pair_ptr_t entry = nullptr;
    for (size_t i = 0; i < metaSz && nullptr != (entry = meta[i]); i++) {
      pktMeta->push_back(helper->p4PacketMetadata(
          helper
              ->p4InfoControllerPacketMetadataMetadataByName(
                  metaInfo, S_CAST(string_ptr_t, entry->left)->toStdString())
              ->id(),
          S_CAST(string_ptr_t, entry->right)->toStdString()));
    }
  }

  return nullptr != (request = helper->p4StreamMessageRequest(
                         helper->p4PacketOut(payload->toStdString(), pktMeta)));
}

// Definition on listener handlers

bool handleConnected(env_ptr_t &env, stack_ptr_t &stack, void **&nextStates) {
  auto request = env->helper->p4StreamMessageRequest(
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

  switch (response->update_case()) {
  case p4_stream_message_response_t::UpdateCase::kArbitration: {
    auto arbitration = response->mutable_arbitration();

    if (grpc::StatusCode::OK == arbitration->mutable_status()->code()) {
      SYNAPSE_DEBUG("Controller is the primary client of the switch");

      if (!installProgram(env->helper, stack)) {
        SYNAPSE_ERROR("Failed to install program on the switch");
        return false;
      }

      env->stack->clear();
      if (!synapse_runtime_handle_pre_configure(env)) {
        SYNAPSE_ERROR("Failed to preconfigure the controller");
        return false;
      }

      VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->pop());
      VAR_S_CAST(p4runtime_stub_ptr_t, stub, stack->top());

      if (mustFlushUpdates(env->queue)) {
        if (!flushUpdates(env->queue, stub)) {
          SYNAPSE_ERROR("Failed to flush updates");
          return false;
        }
      }

      size_t tagsSz;
      pair_ptr_t *tags = nullptr;
      if (mustUpdateTags(env->stack, &tagsSz, &tags)) {
        SYNAPSE_INFO("Tags need to be updated at configuration time");

        if (!updateTags(env, stub, tagsSz, tags)) {
          SYNAPSE_ERROR("Could not update tags");
          return false;
        }
      }

      // We're done with the configuration, queue the first read!
      auto response = new p4_stream_message_response_t();
      stack->push(stream);
      stack->push(response);

      env->stack->clear();

      stream->Read(response, nextStates[0]);
      return true;

    } else {
      SYNAPSE_ERROR("A primary client to the switch already exists");
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kError: {
    SYNAPSE_ERROR("Error during arbitration:");
    SYNAPSE_ERROR("%.*s", (int)response->mutable_error()->DebugString().size(),
                  response->mutable_error()->DebugString().c_str());

  } break;

  default: {
    SYNAPSE_ERROR("Unexpected message type (expecting arbitration update)");

  } break;
  }

  return false;
}

bool handleMessageReceived(env_ptr_t &env, stack_ptr_t &stack,
                           void **&nextStates) {
  assert(3 == stack->size());
  VAR_S_CAST(p4_stream_message_response_ptr_t, response, stack->pop());
  VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->pop());
  VAR_S_CAST(p4runtime_stub_ptr_t, stub, stack->top());

  switch (response->update_case()) {
  case p4_stream_message_response_t::UpdateCase::kPacket: {
    env->stack->clear();

    if (mustParsePacket(response) &&
        !parsePacket(env, response->mutable_packet())) {
      SYNAPSE_ERROR("Could not extract payload and/or metadata from packet");
      return false;
    }

    if (!synapse_runtime_handle_packet_received(env)) {
      SYNAPSE_ERROR("Failed to handle packet");
      return false;
    }

    if (mustFlushUpdates(env->queue) && !flushUpdates(env->queue, stub)) {
      SYNAPSE_ERROR("Failed to flush updates");
      return false;
    }

    size_t tagsSz;
    pair_ptr_t *tags = nullptr;

    if (mustUpdateTags(env->stack, &tagsSz, &tags) &&
        !updateTags(env, stub, tagsSz, tags)) {
      SYNAPSE_ERROR("Could not update tags");
      return false;
    }

    string_ptr_t payload;
    size_t metaSz;
    pair_ptr_t *meta = nullptr;

    if (mustPreparePacketOut(env, &payload, &metaSz, &meta)) {
      p4_stream_message_request_ptr_t request = nullptr;
      if (!preparePacketOut(env->helper, request, payload, metaSz, meta)) {
        SYNAPSE_ERROR("Could not prepare packet out");
        return false;
      }

      stack->push(stream);
      stream->Write(*request, nextStates[1]);

      return true; // Return here to prevent queueing a reading
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kIdleTimeoutNotification: {
    env->stack->clear();

    if (mustParseIdleTimeoutNotification(response) &&
        !parseIdleTimeoutNotification(
            env, response->mutable_idle_timeout_notification())) {
      SYNAPSE_ERROR("Could not extract payload and/or metadata from packet");
      return false;
    }

    if (!synapse_runtime_handle_idle_timeout_notification_received(env)) {
      SYNAPSE_ERROR("Failed to parse idle timeout notification");
      return false;
    }

    env->stack->clear();
    // TODO The stack must be populated with all entries to remove

  } break;

  case p4_stream_message_response_t::UpdateCase::kArbitration: {
    auto arbitration = response->mutable_arbitration();

    if (grpc::OK == arbitration->mutable_status()->code()) {
      SYNAPSE_INFO("Received arbitration update:");
      SYNAPSE_INFO("%.*s", (int)arbitration->DebugString().size(),
                   arbitration->DebugString().c_str());

    } else {
      SYNAPSE_ERROR("Error during arbitration:");
      SYNAPSE_ERROR(
          "%.*s", (int)arbitration->mutable_status()->mutable_message()->size(),
          arbitration->mutable_status()->mutable_message()->c_str());
      SYNAPSE_DEBUG("The highest election ID is now %" SCNu64,
                    arbitration->mutable_election_id()->low());
      return false;
    }

  } break;

  case p4_stream_message_response_t::UpdateCase::kError: {
    auto error = response->mutable_error();

    SYNAPSE_ERROR("Received an error:");
    SYNAPSE_ERROR("%.*s", (int)error->DebugString().size(),
                  error->DebugString().c_str());

  } break;

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
  assert(2 == stack->size());

  auto response = new p4_stream_message_response_t();

  // Nothing to be done, queue the next reading
  VAR_S_CAST(p4runtime_stream_ptr_t, stream, stack->top());
  stream->Read(response, nextStates[0]);
  stack->push(response);

  return true;
}

} // namespace synapse::runtime
