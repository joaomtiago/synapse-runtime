#include "synapse/runtime/wrapper/utils/wrappers.hpp"
#include <assert.h>

#define NOT_NULL(exp) assert(nullptr != (exp))

namespace synapse::runtime {

// String

bool synapse_runtime_wrappers_string_equals(string_ptr_t str1,
                                            string_ptr_t str2) {
  return str1->equals(str2);
}

string_ptr_t synapse_runtime_wrappers_string_new(const char *value,
                                                 size_t value_sz) {
  return new string_t(value, value_sz);
}

// IP address

ip_addr_ptr_t synapse_runtime_wrappers_ip_address_new(const char *value) {
  return new ip_addr_t(value);
}

// MAC address

mac_addr_ptr_t synapse_runtime_wrappers_mac_address_new(const char *value) {
  return new mac_addr_t(value);
}

// Port

port_ptr_t synapse_runtime_wrappers_port_new(uint16_t port) {
  return new port_t(port);
}

// P4 uint32

p4_uint32_ptr_t synapse_runtime_wrappers_p4_uint32_new(uint32_t value) {
  return new p4_uint32_t(value);
}

// Stack

void *synapse_runtime_wrappers_stack_push(stack_ptr_t stack, void *el) {
  NOT_NULL(stack);
  return stack->push(el);
}

void *synapse_runtime_wrappers_stack_pop(stack_ptr_t stack) {
  NOT_NULL(stack);
  return stack->pop();
}

void *synapse_runtime_wrappers_stack_top(stack_ptr_t stack) {
  NOT_NULL(stack);
  return stack->top();
}

bool synapse_runtime_wrappers_stack_empty(stack_ptr_t stack) {
  NOT_NULL(stack);
  return stack->empty();
}

size_t synapse_runtime_wrappers_stack_size(stack_ptr_t stack) {
  NOT_NULL(stack);
  return stack->size();
}

size_t synapse_runtime_wrappers_stack_clear(stack_ptr_t stack) {
  NOT_NULL(stack);
  return stack->clear();
}

// Pair

pair_ptr_t synapse_runtime_wrappers_pair_new(void *left, void *right) {
  return new pair_t(left, right);
}

} // namespace synapse::runtime
