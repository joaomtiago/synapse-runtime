#ifndef SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_BUFFER_HPP_
#define SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_BUFFER_HPP_

#include "synapse/runtime/p4runtime/stream/handler/buffer/update_buffer.hpp"

#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool synapse_runtime_update_buffer_buffer(upd_buff_ptr_t update_buffer,
                                          p4_update_ptr_t update);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_BUFFER_HPP_