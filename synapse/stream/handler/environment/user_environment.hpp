#ifndef SYNAPSE_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_USER_ENVIRONMENT_HPP_
#define SYNAPSE_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_USER_ENVIRONMENT_HPP_

#include "synapse/stream/buffer/update_buffer_wrapper.h"

#ifdef __cplusplus
namespace synapse::p4runtime {
#endif  // __cplusplus

typedef struct UserEnvironment {
  helper_ptr_t helper;
  info_helper_ptr_t info_helper;

  upd_buff_ptr_t update_buffer;

} user_env_t;

typedef user_env_t* user_env_ptr_t;
typedef bool (*user_handler_ptr_t)(user_env_t*);

#ifdef __cplusplus
}  // namespace synapse::p4runtime
#endif  // __cplusplus

#endif  // SYNAPSE_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_USER_ENVIRONMENT_HPP_
