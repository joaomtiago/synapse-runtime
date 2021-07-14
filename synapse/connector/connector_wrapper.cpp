#include "connector_wrapper.h"

namespace synapse::p4runtime {

conn_t* synapse_connector_new(const char* grpc_addr,
                              const char* p4_info_filepath) {
  return new Connector(grpc_addr, p4_info_filepath);
}

bool synapse_connector_start_and_wait(conn_t* connector,
                                      const char* bmv2_json_filepath) {
  return connector->startAndWait(bmv2_json_filepath);
}

void synapse_connector_destroy(conn_t* connector) { delete connector; }

}  // namespace synapse::p4runtime
