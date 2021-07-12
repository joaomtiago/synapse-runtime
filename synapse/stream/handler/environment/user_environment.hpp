#ifndef SYNAPSE_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_USER_ENVIRONMENT_HPP_
#define SYNAPSE_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_USER_ENVIRONMENT_HPP_

#include "synapse/stream/buffer/update_buffer.hpp"

namespace synapse::p4runtime {

typedef struct UserEnvironment {
  helper_ptr_t helper;
  info_helper_ptr_t info_helper;

  upd_buff_ptr_t update_buffer;

} user_env_t;

typedef bool (*user_handler_ptr_t)(user_env_t *);

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_USER_ENVIRONMENT_HPP_
