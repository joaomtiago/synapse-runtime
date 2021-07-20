#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_CUSTOM_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_CUSTOM_HPP_

#include "synapse/runtime/p4runtime/stream/handler/environment/custom.hpp"

#ifndef __cplusplus
#include <stdint.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool synapse_runtime_handle_pre_configure(custom_env_ptr_t env);

bool synapse_runtime_handle_packet_received(custom_env_ptr_t env,
                                            uint8_t *packet_in,
                                            uint16_t packet_in_length);

bool synapse_runtime_handle_idle_timeout_notification_received(
    custom_env_ptr_t env);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_CUSTOM_HPP_