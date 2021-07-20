#ifndef SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_CUSTOM_HPP_
#define SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_CUSTOM_HPP_

#include "synapse/runtime/p4runtime/stream/handler/environment/custom.hpp"

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

helper_ptr_t synapse_runtime_custom_environment_helper(custom_env_ptr_t env);

upd_buff_ptr_t
synapse_runtime_custom_environment_update_buffer(custom_env_ptr_t env);

logger_ptr_t synapse_runtime_custom_environment_logger(custom_env_ptr_t env);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_CUSTOM_HPP_