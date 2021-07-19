#ifndef SYNAPSE_RUNTIME_WRAPPER_CONNECTOR_HPP_
#define SYNAPSE_RUNTIME_WRAPPER_CONNECTOR_HPP_

#include "synapse/runtime/connector.hpp"

#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

conn_t *synapse_runtime_connector_new(const char *grpc_addr,
                                      const char *p4_info_filepath,
                                      logging_level_t logging_level);

bool synapse_runtime_connector_start_and_wait(conn_t *connector,
                                              const char *bmv2_json_filepath);

void synapse_runtime_connector_destroy(conn_t *connector);

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_CONNECTOR_HPP_
