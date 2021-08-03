#include "synapse/runtime/utils/wrappers.hpp"
#include "synapse/runtime/exception/runtime_exception.hpp"
#include <assert.h>
#include <cstring>
#include <inttypes.h>
#include <math.h>
#include <memory>
#include <sstream>

namespace synapse::runtime {

// String

String::String(std::string *string)
    : value(string->c_str()), size(string->size()) {}

String::String(const std::string &string)
    : value(string.c_str()), size(string.size()) {}

String::String(const char *value, size_t size) {
  assert(0 < size);
  this->size = size;

  this->value = (char *)std::malloc(size);
  assert(nullptr != this->value);
  std::memcpy((void *)this->value, value, size);
  assert(nullptr != this->value);
}

std::string String::toStdString() { return std::string(value, size); }

// MAC address

MACAddress::MACAddress(const char *address) {
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
  this->address = new string_t(address, 17);
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
  this->raw = raw;
}

// Decoders

mac_addr_ptr_t
synapse_runtime_wrappers_decode_mac_address(const char *encoded) {
  char buffer[17]; // 12 hex digits + 5 ':'
  char *ptr = buffer;

  for (size_t i = 0; i < 6; i++) {
    sprintf(ptr, "%02x", (uint8_t)encoded[i] & 0xff);
    ptr += 2;
    *ptr++ = ':';
  }

  return new mac_addr_t(buffer, encoded);
}

port_ptr_t synapse_runtime_wrappers_decode_port(string_ptr_t encoded) {
  uint16_t port = 0;

  for (size_t i = 0; i < encoded->size; i++) {
    port = (uint16_t)((port << 8) | ((uint8_t)encoded->value[i] & 0xff));
  }

  return new port_t(port, encoded);
}

// Stack

void Stack::push(void *el) { stack_.push(el); }

void *Stack::pop() {
  auto &el = stack_.top();
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

  return elsPopped;
}

// Pair

Pair::Pair(void *left, void *right) {
  this->left = left;
  this->right = right;
}

} // namespace synapse::runtime