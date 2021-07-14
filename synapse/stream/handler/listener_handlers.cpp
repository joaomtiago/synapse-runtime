#include "synapse/stream/handler/listener_handlers.hpp"

#include "synapse/stream/handler/user_handlers.hpp"
#include "synapse/utils/file_utils.hpp"

#define DEVICE_ID 0
#define ELECTION_ID_LOW 1
#define ELECTION_ID_HIGH 0

namespace synapse::p4runtime {

bool handleConnected(listener_env_t* env) {
  env->tags.tagMakePrimarySent.next_handler = &handleMakePrimarySent;

  env->request = env->connector->helper->streamMessageRequest(
      env->connector->helper->masterArbitrationUpdate(
          DEVICE_ID, ELECTION_ID_LOW, ELECTION_ID_HIGH));

  env->write(env->request, &env->tags.tagMakePrimarySent);
  return true;
}

bool handleMakePrimarySent(listener_env_t* env) {
  env->tags.tagMakePrimaryReceived.next_handler = &handleMakePrimaryReceived;

  env->response = new p4_stream_message_response_t();
  env->read(env->response, &env->tags.tagMakePrimaryReceived);

  return true;
}

bool installProgram(listener_env_t* env);

bool handleWriteResponse(listener_env_t* env);

bool handleMakePrimaryReceived(listener_env_t* env) {
  p4_stream_message_response_t* response = env->response;
  if (response->has_error()) {
    std::cerr << "[-] received the following error during arbitration:"
              << std::endl;
    response->error().PrintDebugString();
    return false;

  } else if (response->has_arbitration()) {
    p4::v1::MasterArbitrationUpdate arbitration = response->arbitration();
    if (grpc::StatusCode::OK == arbitration.status().code()) {
      std::cout << "[+] this controller is now primary" << std::endl;

      /**
       * Now that the controller is the primary client, we still need to install
       * the forward-pipeline configuration, as well as perform any
       * preconfiguration steps required by the controller/user.
       */
      if (!installProgram(env)) {
        std::cerr << "[-] could not install the program on the switch"
                  << std::endl;
        return false;
      }

      if (!synapse_user_handle_pre_configure(env->user_env)) {
        std::cerr << "[-] could not preconfigure the processing environment"
                  << std::endl;
        return false;
      }

      ctx_t context;
      env->connector->response = new p4_write_response_t();
      std::cout << "[*] flushing updates and writing to the switch"
                << std::endl;
      env->connector->stub->Write(&context, *env->update_buffer->flush(),
                                  env->connector->response);

      // Process response, and pass control to the natural handler successor.
      return handleWriteResponse(env) && handleMessageReceived(env);

    } else {
      std::cerr << "[-] a controller already exists" << std::endl;
      return false;
    }

  } else {
    std::cerr << "[-] expected an arbitration update" << std::endl;
    return false;
  }
}

bool handleMessageReceived(listener_env_t* env) {
  bool _proceed = false;
  auto _msg = env->response;

  if (nullptr != _msg) {
    switch (_msg->update_case()) {
      case p4::v1::StreamMessageResponse::UpdateCase::kPacket: {
        _proceed = synapse_user_handle_packet_received(env->user_env);
      } break;

      case p4::v1::StreamMessageResponse::UpdateCase::
          kIdleTimeoutNotification: {
        _proceed =
            synapse_user_handle_idle_timeout_notification_received(env->user_env);
      } break;

      case p4::v1::StreamMessageResponse::UpdateCase::kArbitration: {
        auto _arbitration = _msg->arbitration();

        if (grpc::OK != _arbitration.status().code()) {
          std::cerr << "[-] got an arbitration error:" << std::endl;
          std::cerr << _arbitration.status().message() << std::endl;

          // print the highest election id known to the switch
          std::cerr << "[*] the highest election id is now "
                    << _arbitration.election_id().low() << std::endl;

        } else {
          std::cout << "[*] got an arbitration update:" << std::endl;
          _arbitration.PrintDebugString();
          _proceed = true;
        }
      } break;

      case p4::v1::StreamMessageResponse::UpdateCase::kOther:
      case p4::v1::StreamMessageResponse::UpdateCase::kError:
      case p4::v1::StreamMessageResponse::UpdateCase::kDigest:
      default: {  // unsupported
        std::cout << "[-] received an unsupported message type" << std::endl;
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
bool installProgram(listener_env_t* env) {
  ctx_t _ctx;
  p4::v1::SetForwardingPipelineConfigResponse _res;
  auto _cfg = readFile(env->connector->params->find("bmv2"));

  auto _status = env->connector->stub->SetForwardingPipelineConfig(
      &_ctx,
      *env->connector->helper->setForwardingPipelineConfigRequest(
          DEVICE_ID, ELECTION_ID_LOW, ELECTION_ID_HIGH,
          p4::v1::SetForwardingPipelineConfigRequest_Action_VERIFY_AND_COMMIT,
          env->connector->helper->forwardingPipelineConfig(
              env->connector->info_helper->p4Info(), _cfg)),
      &_res);

  // make sure the status is ok
  if (!_status.ok()) {
    std::cerr
        << "[-] could not verify and/or commit such pipeline configuration"
        << std::endl;
    return false;
  }

  std::cout << "[+] installed the program on the switch" << std::endl;
  return true;
}

/**
 * This auxiliary function processes a write response generated in response to a
 * previous write request on the gRPC stub. The Protobuf response message is
 * empty, so return true;
 */
bool handleWriteResponse(listener_env_t* env) {
  // The write response is available at `env->connector->response`.
  return true;
}

}  // namespace synapse::p4runtime
