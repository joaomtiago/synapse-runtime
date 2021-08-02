#include "synapse/runtime/wrapper/utils/wrappers.hpp"

namespace synapse::runtime {

// String

string_ptr_t synapse_runtime_wrappers_string_new(const char *value,
                                                 size_t value_sz) {
  return new string_t(value, value_sz);
}

// MAC address

mac_addr_ptr_t synapse_runtime_wrappers_mac_address_new(string_ptr_t address) {
  return new mac_addr_t(address->toStdString());
}

// Port

port_ptr_t synapse_runtime_wrappers_port_new(uint16_t port) {
  return new port_t(port);
}

// Stack

void synapse_runtime_wrappers_stack_push(stack_ptr_t stack, void *el) {
  return stack->push(el);
}

void *synapse_runtime_wrappers_stack_pop(stack_ptr_t stack) {
  return stack->pop();
}

void *synapse_runtime_wrappers_stack_top(stack_ptr_t stack) {
  return stack->top();
}

bool synapse_runtime_wrappers_stack_empty(stack_ptr_t stack) {
  return stack->empty();
}

size_t synapse_runtime_wrappers_stack_size(stack_ptr_t stack) {
  return stack->size();
}

size_t synapse_runtime_wrappers_stack_clear(stack_ptr_t stack) {
  return stack->clear();
}

// Pair

pair_ptr_t synapse_runtime_wrappers_pair_new(void *left, void *right) {
  return new pair_t(left, right);
}

} // namespace synapse::runtime
