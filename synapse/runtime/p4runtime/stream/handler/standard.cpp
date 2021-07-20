#include "synapse/runtime/p4runtime/stream/handler/environment/standard.hpp"
#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"
#include "synapse/runtime/p4runtime/stream/handler/standard.hpp"
#include "synapse/runtime/utils/constants.hpp"
#include "synapse/runtime/utils/file.hpp"

namespace synapse::runtime {

bool handleConnected(standard_env_ptr_t env) {
  env->tags.tagMakePrimarySent.next_handler = &handleMakePrimarySent;

  env->request = env->connector->helper->p4StreamMessageRequest(
      env->connector->helper->p4MasterArbitrationUpdate(
          DEVICE_ID, ELECTION_ID_LOW, ELECTION_ID_HIGH));

  env->write(env->request, &env->tags.tagMakePrimarySent);
  return true;
}

bool handleMakePrimarySent(standard_env_ptr_t env) {
  env->tags.tagMakePrimaryReceived.next_handler = &handleMakePrimaryReceived;

  env->response = new p4_stream_message_response_t();
  env->read(env->response, &env->tags.tagMakePrimaryReceived);

  return true;
}

bool installProgram(standard_env_ptr_t env);

bool handleWriteResponse(standard_env_ptr_t env);

bool handleMakePrimaryReceived(standard_env_ptr_t env) {
  p4_stream_message_response_t *response = env->response;
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
      if (!installProgram(env)) {
        env->logger->error("Could not install the program on the switch");
        return false;
      }

      if (!synapse_runtime_handle_pre_configure(env->custom_env)) {
        env->logger->error("Could not preconfigure the processing environment");
        return false;
      }

      grpc_cctx_t context;
      env->connector->response = new p4_write_response_t();
      env->logger->debug("Flushing updates, and writing to the switch");
      env->connector->stub->Write(&context, *env->update_buffer->flush(),
                                  env->connector->response);

      // Process response, and pass control to the natural handler successor.
      return handleWriteResponse(env) && handleMessageReceived(env);

    } else {
      env->logger->error("A controller already exists");
      return false;
    }

  } else {
    env->logger->error("Expected an arbitration update");
    return false;
  }
}

bool handleMessageReceived(standard_env_ptr_t env) {
  bool _proceed = false;
  auto _msg = env->response;

  if (nullptr != _msg) {
    switch (_msg->update_case()) {
    case p4_stream_message_response_t::UpdateCase::kPacket: {
      env->custom_env->packet_in = reinterpret_cast<uint8_t *>(
          _msg->mutable_packet()->mutable_payload());
      env->custom_env->packet_in_length = static_cast<uint16_t>(
          _msg->mutable_packet()->mutable_payload()->size());

      _proceed = synapse_runtime_handle_packet_received(env->custom_env);
    } break;

    case p4_stream_message_response_t::UpdateCase::kIdleTimeoutNotification: {
      _proceed = synapse_runtime_handle_idle_timeout_notification_received(
          env->custom_env);
    } break;

    case p4_stream_message_response_t::UpdateCase::kArbitration: {
      auto _arbitration = _msg->arbitration();

      if (grpc::OK != _arbitration.status().code()) {
        env->logger->error("Received an arbitration error:");
        env->logger->error(_arbitration.status().message());

        // print the highest election id known to the switch
        env->logger->debug("The highest election ID is now " +
                           _arbitration.election_id().low());

      } else {
        env->logger->debug("Received an arbitration update:");
        env->logger->debug(_arbitration.DebugString());
        _proceed = true;
      }
    } break;

    case p4_stream_message_response_t::UpdateCase::kOther:
    case p4_stream_message_response_t::UpdateCase::kError:
    case p4_stream_message_response_t::UpdateCase::kDigest:
    default: { // unsupported
      env->logger->error("Received an unsupported message type");
      _proceed = true;
    } break;
    }
  }

  if (_proceed) {
    // queue the first read
    env->tags.tagMessageReceived.next_handler = &handleMessageReceived;
    env->response = new p4_stream_message_response_t();
    env->read(env->response, (&env->tags.tagMessageReceived));

    return true;
  }

  return false;
}

/**
 * This auxiliary function tries to install the forwarding pipeline
 * configuration onto the P4 switch. If the call fails, the listener is
 * interrupted.
 */
bool installProgram(standard_env_ptr_t env) {
  grpc_cctx_t context;
  p4::v1::SetForwardingPipelineConfigResponse _res;
  auto _cfg = readFile(env->connector->params.find("bmv2"));

  auto _status = env->connector->stub->SetForwardingPipelineConfig(
      &context,
      *env->connector->helper->p4SetForwardingPipelineConfigRequest(
          DEVICE_ID, ELECTION_ID_LOW, ELECTION_ID_HIGH,
          p4::v1::SetForwardingPipelineConfigRequest_Action_VERIFY_AND_COMMIT,
          env->connector->helper->p4ForwardingPipelineConfig(
              env->connector->helper->p4InfoP4Info(), _cfg)),
      &_res);

  // make sure the status is ok
  if (!_status.ok()) {
    env->logger->error("Could not apply pipeline configuration");
    return false;
  }

  env->logger->info("Installed the program on the switch");
  return true;
}

/**
 * This auxiliary function processes a write response generated in response to a
 * previous write request on the gRPC stub. The Protobuf response message is
 * empty, so return true;
 */
bool handleWriteResponse(standard_env_ptr_t env) {
  // The write response is available at `env->connector->response`.
  return true;
}

} // namespace synapse::runtime
