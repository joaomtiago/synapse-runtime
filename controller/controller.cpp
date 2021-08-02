#include "synapse/runtime/connector.hpp"
#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"
#include "synapse/runtime/wrapper/p4runtime/stream/handler/environment.hpp"
#include "synapse/runtime/wrapper/utils/wrappers.hpp"

#define INFO(msg) env->logger->info(msg)

namespace synapse::runtime {

bool synapse_runtime_handle_pre_configure(env_ptr_t env) {
  INFO("Preconfiguring the switch...");
  return true;
}

bool synapse_runtime_handle_packet_received(env_ptr_t env) {
  INFO("Received a packet...");
  return true;
}

bool synapse_runtime_handle_idle_timeout_notification_received(env_ptr_t env) {
  INFO("Received an idle timeout notification...");
  return true;
}

} // namespace synapse::runtime

using synapse::runtime::conn_ptr_t;
using synapse::runtime::conn_t;
using synapse::runtime::logging_level_t;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout << "usage: ./cpp_controller <grpcAddr> <p4InfoFilepath> "
                 "<bmv2JsonFilepath>"
              << std::endl;
    exit(1);
  }

  conn_ptr_t connector = new conn_t(argv[1], logging_level_t::LoggerDebug);
  return connector->configure(argv[3], argv[2]) && connector->startAndWait();
}