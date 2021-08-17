#ifndef SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_HPP_
#define SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_HPP_

#include "synapse/runtime/p4runtime/stream/handler/environment.hpp"

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

helper_ptr_t synapse_runtime_environment_helper(env_ptr_t env);

update_queue_ptr_t synapse_runtime_environment_queue(env_ptr_t env);

stack_ptr_t synapse_runtime_environment_stack(env_ptr_t env);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_HPP_