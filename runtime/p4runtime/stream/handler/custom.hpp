#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_CUSTOM_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_CUSTOM_HPP_

#include "environment/custom.hpp"

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Below we declare the multiple user-space handlers (which all take a reference
 * to a user-space environment as argument).
 */

bool synapse_runtime_handle_pre_configure(custom_env_ptr_t env);

bool synapse_runtime_handle_packet_received(custom_env_ptr_t env);

bool synapse_runtime_handle_idle_timeout_notification_received(
    custom_env_ptr_t env);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_CUSTOM_HPP_