#include "synapse/runtime/wrapper/p4runtime/stream/handler/environment.hpp"

#define NOT_NULL(ptr) assert(nullptr != ptr)

namespace synapse::runtime {

helper_ptr_t synapse_runtime_environment_helper(env_ptr_t env) {
  NOT_NULL(env);
  return env->helper;
}

stack_ptr_t synapse_runtime_environment_stack(env_ptr_t env) {
  NOT_NULL(env);
  return env->stack;
}

upd_buff_ptr_t synapse_runtime_environment_update_buffer(env_ptr_t env) {
  NOT_NULL(env);
  return env->update_buffer;
}

} // namespace synapse::runtime