#include "synapse/runtime/wrapper/connector.hpp"

namespace synapse::runtime {

conn_ptr_t synapse_runtime_connector_new(const char *grpc_addr,
                                         logger_logging_level_t logging_level) {
  return new Connector(grpc_addr, logging_level);
}

bool synapse_runtime_connector_configure(conn_ptr_t connector,
                                         const char *bmv2JsonFilepath,
                                         const char *p4InfoFilepath) {
  return connector->configure(bmv2JsonFilepath, p4InfoFilepath);
}

bool synapse_runtime_connector_start_and_wait(conn_ptr_t connector) {
  return connector->startAndWait();
}

void synapse_runtime_connector_destroy(conn_ptr_t connector) {
  delete connector;
}

} // namespace synapse::runtime
