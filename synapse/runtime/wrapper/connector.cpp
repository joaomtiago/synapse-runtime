#include "synapse/runtime/wrapper/connector.hpp"

namespace synapse::runtime {

conn_t *synapse_runtime_connector_new(const char *grpc_addr,
                                      const char *p4_info_filepath,
                                      logging_level_t logging_level) {
  return new Connector(grpc_addr, p4_info_filepath, logging_level);
}

bool synapse_runtime_connector_start_and_wait(conn_t *connector,
                                              const char *bmv2_json_filepath) {
  return connector->startAndWait(bmv2_json_filepath);
}

void synapse_runtime_connector_destroy(conn_t *connector) { delete connector; }

} // namespace synapse::runtime
