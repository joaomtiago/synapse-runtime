#ifndef SYNAPSE_P4RUNTIME_STREAM_HANDLER_USER_HANDLERS_WRAPPER_H_
#define SYNAPSE_P4RUNTIME_STREAM_HANDLER_USER_HANDLERS_WRAPPER_H_

#include "environment/user_environment.hpp"

#ifdef __cplusplus
namespace synapse::p4runtime {
#endif  // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * Below we declare the multiple user-space handlers (which all take a reference
 * to a user-space environment as argument).
 */

bool synapse_user_handle_pre_configure(user_env_ptr_t env);

bool synapse_user_handle_packet_received(user_env_ptr_t env);

bool synapse_user_handle_idle_timeout_notification_received(user_env_ptr_t env);

#ifdef __cplusplus
}
#endif  // __cplusplus

#ifdef __cplusplus
}  // namespace synapse::p4runtime
#endif  // __cplusplus

#endif  // SYNAPSE_P4RUNTIME_STREAM_HANDLER_USER_HANDLERS_WRAPPER_H_