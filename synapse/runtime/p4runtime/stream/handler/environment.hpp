#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_HPP_

#include "synapse/runtime/wrapper/p4runtime/helper.hpp"
#include "synapse/runtime/wrapper/p4runtime/stream/handler/buffer/update_buffer.hpp"
#include "synapse/runtime/wrapper/utils/wrappers.hpp"

#ifdef __cplusplus
#include <stack>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

typedef struct Environment {
  helper_ptr_t helper;
  stack_ptr_t stack;
  upd_buff_ptr_t update_buffer;

} env_t;

#ifdef __cplusplus
typedef env_t *env_ptr_t;
#else
typedef void *env_ptr_t;
#endif // __cplusplus

#ifdef __cplusplus
typedef bool (*handler_ptr_t)(env_ptr_t env, stack_ptr_t stack,
                              void **nextStates);
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_HPP_