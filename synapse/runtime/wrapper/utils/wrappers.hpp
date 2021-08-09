#ifndef SYNAPSE_RUNTIME_WRAPPER_UTILS_WRAPPERS_HPP_
#define SYNAPSE_RUNTIME_WRAPPER_UTILS_WRAPPERS_HPP_

#include "synapse/runtime/utils/wrappers.hpp"

#ifndef __cplusplus
#include <stdint.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// String

string_ptr_t synapse_runtime_wrappers_string_new(const char *value,
                                                 size_t value_sz);

// MAC address

mac_addr_ptr_t synapse_runtime_wrappers_mac_address_new(const char *value);

// Port

port_ptr_t synapse_runtime_wrappers_port_new(uint16_t port);

// P4 uint32

p4_uint32_ptr_t synapse_runtime_wrappers_p4_uint32_new(uint32_t value);

// Stack

void synapse_runtime_wrappers_stack_push(stack_ptr_t stack, void *el);

void *synapse_runtime_wrappers_stack_pop(stack_ptr_t stack);

void *synapse_runtime_wrappers_stack_top(stack_ptr_t stack);

bool synapse_runtime_wrappers_stack_empty(stack_ptr_t stack);

size_t synapse_runtime_wrappers_stack_size(stack_ptr_t stack);

size_t synapse_runtime_wrappers_stack_clear(stack_ptr_t stack);

// Pair

pair_ptr_t synapse_runtime_wrappers_pair_new(void *left, void *right);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_UTILS_WRAPPERS_HPP_