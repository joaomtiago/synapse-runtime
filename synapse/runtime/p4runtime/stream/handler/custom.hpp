#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_CUSTOM_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_CUSTOM_HPP_

#include "synapse/runtime/p4runtime/stream/handler/environment.hpp"

#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool synapse_runtime_handle_pre_configure(env_ptr_t env);

bool synapse_runtime_handle_packet_received();

bool synapse_runtime_handle_idle_timeout_notification_received();

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_CUSTOM_HPP_