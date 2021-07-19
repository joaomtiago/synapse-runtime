#ifndef SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_BUFFER_UPDATE_BUFFER_HPP_S
#define SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_BUFFER_UPDATE_BUFFER_HPP_S

#include "synapse/runtime/p4runtime/stream/handler/environment/buffer/update_buffer.hpp"

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool synapse_runtime_update_buffer_buffer(upd_buff_ptr_t update_buffer,
                                          p4_update_t *update);

p4_write_request_t *
synapse_runtime_update_buffer_flush(upd_buff_ptr_t update_buffer);

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_BUFFER_UPDATE_BUFFER_HPP_S
