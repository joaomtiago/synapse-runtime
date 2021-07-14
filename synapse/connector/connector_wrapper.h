#ifndef SYNAPSE_P4RUNTIME_CONNECTOR_CONNECTOR_WRAPPER_H_
#define SYNAPSE_P4RUNTIME_CONNECTOR_CONNECTOR_WRAPPER_H_

#ifdef __cplusplus
#include "connector.hpp"
#endif  // __cplusplus

#ifndef __cplusplus
struct connector;
typedef struct connector conn_t;
#endif  // __cplusplus

#ifdef __cplusplus
namespace synapse::p4runtime {
#endif  // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

conn_t* synapse_connector_new(const char* grpc_addr,
                              const char* p4_info_filepath);

bool synapse_connector_start_and_wait(conn_t* connector,
                                      const char* bmv2_json_filepath);

void synapse_connector_destroy(conn_t* connector);

#ifdef __cplusplus
}
#endif  // __cplusplus

#ifdef __cplusplus
}  // namespace synapse::p4runtime
#endif  // __cplusplus

#endif  // SYNAPSE_P4RUNTIME_CONNECTOR_CONNECTOR_WRAPPER_H_
