#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"
#include "synapse/runtime/wrapper/connector.hpp"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

bool synapse_runtime_handle_pre_configure(custom_env_ptr_t env) { return true; }

bool synapse_runtime_handle_packet_received(custom_env_ptr_t env,
                                            uint8_t *packet_in,
                                            uint16_t packet_in_length) {
  printf("Received a new packet...\n");
  return true;
}

bool synapse_runtime_handle_idle_timeout_notification_received(
    custom_env_ptr_t env) {
  return true;
}

int main(int argc, char const *argv[]) {
  if (argc != 4) {
    printf("usage: ./c_controller <grpcAddr> <p4InfoFilepath> "
           "<bmv2JsonFilepath>\n");
    exit(1);
  }

  conn_ptr_t conn = synapse_runtime_connector_new(argv[1], argv[2], Debug);
  bool result = synapse_runtime_connector_start_and_wait(conn, argv[3]);
  synapse_runtime_connector_destroy(conn);

  return result;
}
