#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"
#include "synapse/runtime/wrapper/connector.hpp"

#include <stdio.h>
#include <stdlib.h>

bool synapse_runtime_handle_pre_configure(env_ptr_t env) {
  synapse_runtime_wrappers_stack_clear(synapse_runtime_environment_stack(env));
  return true;
}

bool synapse_runtime_handle_packet_received(env_ptr_t env) {
  synapse_runtime_wrappers_stack_clear(synapse_runtime_environment_stack(env));
  return true;
}

bool synapse_runtime_handle_idle_timeout_notification_received(env_ptr_t env) {
  synapse_runtime_wrappers_stack_clear(synapse_runtime_environment_stack(env));
  return true;
}

int main(int argc, char const *argv[]) {
  if (argc != 4) {
    printf("usage: ./c_controller <grpcAddr> <p4InfoFilepath> "
           "<bmv2JsonFilepath>\n");
    exit(1);
  }

  conn_ptr_t connector = synapse_runtime_connector_new(argv[1]);
  if (synapse_runtime_connector_configure(connector, argv[3], argv[2])) {
    synapse_runtime_connector_start_and_wait(connector);
  }

  synapse_runtime_connector_destroy(connector);
  return 0;
}
