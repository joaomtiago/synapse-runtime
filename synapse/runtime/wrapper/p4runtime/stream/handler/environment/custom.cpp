#include "synapse/runtime/wrapper/p4runtime/stream/handler/environment/custom.hpp"

namespace synapse::runtime {

helper_ptr_t synapse_runtime_custom_environment_helper(custom_env_ptr_t env) {
  return env->helper;
}

upd_buff_ptr_t
synapse_runtime_custom_environment_update_buffer(custom_env_ptr_t env) {
  return env->update_buffer;
}

logger_ptr_t synapse_runtime_custom_environment_logger(custom_env_ptr_t env) {
  return env->logger;
}

} // namespace synapse::runtime
