#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_CUSTOM_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_CUSTOM_HPP_

#include "synapse/runtime/p4runtime/stream/handler/environment/buffer/update_buffer.hpp"
#include "synapse/runtime/utils/logger.hpp"

#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

typedef struct CustomEnvironment {
  helper_ptr_t helper;
  upd_buff_ptr_t update_buffer;
  logger_ptr_t logger;

} custom_env_t;

#ifdef __cplusplus
typedef custom_env_t *custom_env_ptr_t;
#else
typedef void *custom_env_ptr_t;
#endif // __cplusplus

typedef bool (*custom_handler_ptr_t)(custom_env_ptr_t);

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_CUSTOM_HPP_
