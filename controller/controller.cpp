#include "synapse/runtime/connector.hpp"
#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"

using synapse::runtime::conn_ptr_t;
using synapse::runtime::conn_t;
using synapse::runtime::logging_level_t;

namespace synapse::runtime {

bool synapse_runtime_handle_pre_configure(custom_env_ptr_t env) {
  std::cout << "Preconfiguring the switch..." << std::endl;
  return true;
}

bool synapse_runtime_handle_packet_received(custom_env_ptr_t env,
                                            uint8_t *packet_in,
                                            uint16_t packet_in_length) {
  std::cout << "Received a packet..." << std::endl;
  return true;
}

bool synapse_runtime_handle_idle_timeout_notification_received(
    custom_env_ptr_t env) {
  std::cout << "Received an idle timeout notification..." << std::endl;
  return true;
}

} // namespace synapse::runtime

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout << "usage: ./cpp_controller <grpcAddr> <p4InfoFilepath> "
                 "<bmv2JsonFilepath>"
              << std::endl;
    exit(1);
  }

  conn_ptr_t connector = new conn_t(argv[1], argv[2], logging_level_t::Debug);
  return connector->startAndWait(argv[3]);
}