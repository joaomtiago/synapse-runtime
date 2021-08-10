#include "synapse/runtime/connector.hpp"
#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"

namespace synapse::runtime {

bool synapse_runtime_handle_pre_configure(env_ptr_t env) {
  env->stack->clear();
  return true;
}

bool synapse_runtime_handle_packet_received(env_ptr_t env) {
  env->stack->clear();
  return true;
}

bool synapse_runtime_handle_idle_timeout_notification_received(env_ptr_t env) {
  env->stack->clear();
  return true;
}

} // namespace synapse::runtime

using synapse::runtime::conn_ptr_t;
using synapse::runtime::conn_t;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout << "usage: ./cpp_controller <grpcAddr> <p4InfoFilepath> "
                 "<bmv2JsonFilepath>"
              << std::endl;
    exit(1);
  }

  conn_ptr_t connector = new conn_t(argv[1]);
  return connector->configure(argv[3], argv[2]) && connector->startAndWait();
}