#include "synapse/runtime/wrapper/p4runtime/stream/handler/environment.hpp"

#define NOT_NULL(ptr) assert(nullptr != ptr)

namespace synapse::runtime {

helper_ptr_t synapse_runtime_environment_helper(env_ptr_t env) {
  NOT_NULL(env);
  return env->helper;
}

update_queue_ptr_t synapse_runtime_environment_queue(env_ptr_t env) {
  NOT_NULL(env);
  return env->queue;
}

stack_ptr_t synapse_runtime_environment_stack(env_ptr_t env) {
  NOT_NULL(env);
  return env->stack;
}

} // namespace synapse::runtime