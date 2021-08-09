#include "synapse/runtime/utils/wrappers.hpp"
#include "synapse/runtime/exception/runtime_exception.hpp"
#include <assert.h>
#include <cstring>
#include <inttypes.h>
#include <math.h>
#include <memory>
#include <sstream>

#define ASSIGN_BYTE(var_type, var, buffer, i)                                  \
  var = (var_type)((var << 8) | ((uint8_t)buffer->str[i] & 0xff));

#define NOT_NULL(exp) assert(nullptr != (exp))

namespace synapse::runtime {

// String

String::String(const std::string *string) {
  this->str = string->c_str();
  this->sz = string->size();
}

String::String(const std::string &string)
    : String(string.c_str(), string.size()) {}

String::String(const char *value, size_t size) {
  this->sz = size;

  if (0 < size) {
    this->str = (char *)std::malloc(size);
    NOT_NULL(this->str);
    std::memcpy((void *)this->str, value, size);
    NOT_NULL(this->str);
  }
}

std::string String::toStdString() { return std::string(str, sz); }

// MAC address

MACAddress::MACAddress(const char *address) {
  NOT_NULL(address);

  uint32_t byte1, byte2, byte3, byte4, byte5, byte6;
  if (6 != sscanf(address, "%02x:%02x:%02x:%02x:%02x:%02x", &byte1, &byte2,
                  &byte3, &byte4, &byte5, &byte6)) {
    throw RuntimeException("Could not parse MAC address");
  }

  std::stringstream stream;
  stream << (uint8_t)byte1 && 0xff;
  stream << (uint8_t)byte2 && 0xff;
  stream << (uint8_t)byte3 && 0xff;
  stream << (uint8_t)byte4 && 0xff;
  stream << (uint8_t)byte5 && 0xff;
  stream << (uint8_t)byte6 && 0xff;
  std::string streamStr = stream.str();

  this->address = new string_t(address, 17);
  this->raw = new string_t(streamStr.c_str(), 6);
}

MACAddress::MACAddress(const char *address, const char *raw) {
  NOT_NULL(address);
  this->address = new string_t(address, 17);

  NOT_NULL(raw);
  this->raw = new string_t(raw, 6);
}

// Port

Port::Port(const uint16_t &port) {
  if (port > 511) {
    throw RuntimeException("A port must fit within 9 bits");
  }

  /**
   * Get the string representation of the port, and store it in a buffer.
   * Because a port must fit within 9 bits, we only consider 3 hex chars (+ the
   * null-terminated string).
   */
  char buffer[4];
  snprintf(buffer, 4, "%03x", port);

  /**
   * Get the least significant bit of the most significant byte, and all bits of
   * the least significant byte.
   */

  uint32_t byte1, byte2;
  if (2 != sscanf(buffer, "%01x%02x", &byte1, &byte2)) {
    throw RuntimeException("Could not parse hexadecimal port");
  }

  std::stringstream stream;
  stream << (uint8_t)byte1 && 0x01;
  stream << (uint8_t)byte2 && 0xff;
  std::string streamStr = stream.str();

  this->port = port;
  this->raw = new string_t(streamStr.c_str(), streamStr.size());
}

Port::Port(const uint16_t &port, string_ptr_t raw) {
  this->port = port;

  NOT_NULL(raw);
  this->raw = raw;
}

// P4 uint32

P4Uint32::P4Uint32(const uint32_t &value) {
  char buffer[9];
  snprintf(buffer, 9, "%08x", value);

  uint32_t byte1, byte2, byte3, byte4;
  if (4 != sscanf(buffer, "%02x%02x%02x%02x", &byte1, &byte2, &byte3, &byte4)) {
    throw RuntimeException("Could not parse uint32");
  }

  std::stringstream stream;
  stream << (uint8_t)byte1 && 0xff;
  stream << (uint8_t)byte2 && 0xff;
  stream << (uint8_t)byte3 && 0xff;
  stream << (uint8_t)byte4 && 0xff;
  std::string streamStr = stream.str();

  this->value = value;
  this->raw = new string_t(streamStr.c_str(), streamStr.size());
}

P4Uint32::P4Uint32(const uint32_t &value, string_ptr_t raw) {
  this->value = value;

  NOT_NULL(raw);
  this->raw = raw;
}

// Decoders

mac_addr_ptr_t
synapse_runtime_wrappers_decode_mac_address(string_ptr_t encoded) {
  NOT_NULL(encoded);
  assert(6 <= encoded->sz);

  char buffer[17]; // 12 hex digits + 5 ':'
  char *ptr = buffer;

  for (size_t i = 0; i < 6; i++) {
    sprintf(ptr, "%02x", (uint8_t)encoded->str[i] & 0xff);
    ptr += 2;
    *ptr++ = ':';
  }

  return new mac_addr_t(buffer, encoded->str);
}

port_ptr_t synapse_runtime_wrappers_decode_port(string_ptr_t encoded) {
  NOT_NULL(encoded);

  size_t size = encoded->sz;
  if (2 < size) {
    size = 2;
  }

  uint16_t port = 0;
  for (size_t i = 0; i < size; i++) {
    ASSIGN_BYTE(uint16_t, port, encoded, i);
  }

  return new port_t(port, encoded);
}

uint32_t synapse_runtime_wrappers_decode_p4_uint32(string_ptr_t encoded) {
  NOT_NULL(encoded);

  uint32_t value = 0;
  for (size_t i = 0; i < encoded->sz; i++) {
    ASSIGN_BYTE(uint32_t, value, encoded, i);
  }

  return value;
}

// Stack

void Stack::push(void *el) {
  if (nullptr == el) {
    SYNAPSE_INFO("Pushing a null pointer to the stack");
  }

  stack_.push(el);
}

void *Stack::pop() {
  void *&el = stack_.top();
  if (nullptr == el) {
    SYNAPSE_INFO("Popping a null pointer from the stack");
  }

  stack_.pop();
  return el;
}

void *Stack::top() { return stack_.top(); }

bool Stack::empty() { return stack_.empty(); }

size_t Stack::size() { return stack_.size(); }

size_t Stack::clear() {
  size_t elsPopped = 0;
  while (!stack_.empty()) {
    stack_.pop();
    elsPopped++;
  }

  SYNAPSE_DEBUG("Cleared %lu element(s) from the stack", elsPopped);
  return elsPopped;
}

// Pair

Pair::Pair(void *left, void *right) {
  if (nullptr == left) {
    SYNAPSE_INFO("Provided a null pointer as left element");
  }
  this->left = left;

  if (nullptr == right) {
    SYNAPSE_INFO("Provided a null pointer as right element");
  }
  this->right = right;
}

} // namespace synapse::runtime