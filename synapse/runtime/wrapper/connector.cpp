#include "synapse/runtime/wrapper/connector.hpp"

#define NOT_NULL(exp) assert(nullptr != (exp))

namespace synapse::runtime {

conn_ptr_t synapse_runtime_connector_new(const char *grpc_addr) {
  return new Connector(grpc_addr);
}

bool synapse_runtime_connector_configure(conn_ptr_t connector,
                                         const char *bmv2JsonFilepath,
                                         const char *p4InfoFilepath) {
  NOT_NULL(connector);
  return connector->configure(bmv2JsonFilepath, p4InfoFilepath);
}

bool synapse_runtime_connector_start_and_wait(conn_ptr_t connector) {
  NOT_NULL(connector);
  return connector->startAndWait();
}

void synapse_runtime_connector_destroy(conn_ptr_t connector) {
  NOT_NULL(connector);
  delete connector;
}

} // namespace synapse::runtime
