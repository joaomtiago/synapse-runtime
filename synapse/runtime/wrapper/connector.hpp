#ifndef SYNAPSE_RUNTIME_WRAPPER_CONNECTOR_HPP_
#define SYNAPSE_RUNTIME_WRAPPER_CONNECTOR_HPP_

#include "synapse/runtime/connector.hpp"

#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

conn_ptr_t synapse_runtime_connector_new(const char *grpc_addr,
                                         logging_level_t logging_level);

bool synapse_runtime_connector_configure(conn_ptr_t connector,
                                         const char *bmv2JsonFilepath,
                                         const char *p4InfoFilepath);

bool synapse_runtime_connector_start_and_wait(conn_ptr_t connector);

void synapse_runtime_connector_destroy(conn_ptr_t connector);

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_CONNECTOR_HPP_
