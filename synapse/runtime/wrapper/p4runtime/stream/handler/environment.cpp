#include "synapse/runtime/wrapper/p4runtime/stream/handler/environment.hpp"

namespace synapse::runtime {

helper_ptr_t synapse_runtime_environment_helper(env_ptr_t env) {
  return env->helper;
}

logger_ptr_t synapse_runtime_environment_logger(env_ptr_t env) {
  return env->logger;
}

stack_ptr_t synapse_runtime_environment_stack(env_ptr_t env) {
  return env->stack;
}

upd_buff_ptr_t synapse_runtime_environment_update_buffer(env_ptr_t env) {
  return env->update_buffer;
}

} // namespace synapse::runtime