#ifndef SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_UPDATE_QUEUE_HPP_
#define SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_UPDATE_QUEUE_HPP_

#include "synapse/runtime/p4runtime/stream/handler/update_queue.hpp"

#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool synapse_runtime_update_queue_queue(update_queue_ptr_t queue,
                                        p4_update_ptr_t update);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_UPDATE_QUEUE_HPP_