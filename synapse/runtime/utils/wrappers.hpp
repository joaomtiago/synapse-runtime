#ifndef SYNAPSE_RUNTIME_UTILS_WRAPPERS_HPP_
#define SYNAPSE_RUNTIME_UTILS_WRAPPERS_HPP_

#define _CAST(cast_type, type, value) cast_type##_cast<type>(value)
#define _ASSIGNMENT(lvalue, rvalue) lvalue = rvalue

#define S_CAST(type, value) _CAST(static, type, value)
#define VAR_S_CAST(type, var, value) _ASSIGNMENT(type var, S_CAST(type, value))

#define D_CAST(type, value) _CAST(dynamic, type, value)
#define VAR_D_CAST(type, var, value) _ASSIGNMENT(type var, D_CAST(type, value))

#define R_CAST(type, value) _CAST(reinterpret, type, value)
#define VAR_R_CAST(type, var, value) _ASSIGNMENT(type var, R_CAST(type, value))

#define SYNAPSE_ENABLE_DEBUG

#ifdef SYNAPSE_ENABLE_DEBUG
#define SYNAPSE_ENABLE_INFO
#define SYNAPSE_ENABLE_ERROR
#endif // SYNAPSE_ENABLE_DEBUG

#ifdef SYNAPSE_ENABLE_INFO
#define SYNAPSE_ENABLE_ERROR
#endif // SYNAPSE_ENABLE_INFO

#ifdef SYNAPSE_ENABLE_ERROR
#define SYNAPSE_ERROR(text, ...) printf("ERROR: " text "\n", ##__VA_ARGS__)
#else // SYNAPSE_ENABLE_ERROR
#define SYNAPSE_ERROR(text, ...)
#endif // SYNAPSE_ENABLE_ERROR

#ifdef SYNAPSE_ENABLE_INFO
#define SYNAPSE_INFO(text, ...) printf("INFO: " text "\n", ##__VA_ARGS__)
#else // SYNAPSE_ENABLE_INFO
#define SYNAPSE_INFO(text, ...)
#endif // SYNAPSE_ENABLE_INFO

#ifdef SYNAPSE_ENABLE_DEBUG
#define SYNAPSE_DEBUG(text, ...) printf("DEBUG: " text "\n", ##__VA_ARGS__)
#else // SYNAPSE_ENABLE_DEBUG
#define SYNAPSE_DEBUG(text, ...)
#endif // SYNAPSE_ENABLE_DEBUG

#ifdef __cplusplus
#include <stack>
#include <string>
#endif // __cplusplus

#ifndef __cplusplus
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

// String

typedef struct String {
#ifdef __cplusplus
  String(const std::string *string);

  String(const std::string &string);

  String(const char *value, size_t value_sz);
#endif // __cplusplus

  const char *str;
  size_t sz;

#ifdef __cplusplus
  bool equals(String *cmp);

  std::string toStdString();
#endif // __cplusplus

} string_t;
typedef string_t *string_ptr_t;

// IP address

typedef struct IPAddress {
#ifdef __cplusplus
  IPAddress(const char *address);

  IPAddress(string_ptr_t address, string_ptr_t bytes);
#endif // __cplusplus

  string_ptr_t address;
  string_ptr_t bytes;

} ip_addr_t;
typedef ip_addr_t *ip_addr_ptr_t;

// MAC address

typedef struct MACAddress {
#ifdef __cplusplus
  MACAddress(const char *address);

  MACAddress(const char *address, const char *bytes);
#endif // __cplusplus

  string_ptr_t address;
  string_ptr_t bytes;

} mac_addr_t;
typedef mac_addr_t *mac_addr_ptr_t;

// Port

typedef struct Port {
#ifdef __cplusplus
  Port(const uint16_t &port);

  Port(const uint16_t &port, string_ptr_t bytes);
#endif // __cplusplus

  uint16_t port;
  string_ptr_t bytes;

} port_t;
typedef port_t *port_ptr_t;

// P4 uint32

typedef struct P4Uint32 {
#ifdef __cplusplus
  P4Uint32(const uint32_t &value);

  P4Uint32(const uint32_t &value, string_ptr_t bytes);
#endif // __cplusplus

  uint32_t value;
  string_ptr_t bytes;

} p4_uint32_t;
typedef p4_uint32_t *p4_uint32_ptr_t;

// Decoders

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

ip_addr_ptr_t synapse_runtime_wrappers_decode_ip_address(string_ptr_t encoded);

mac_addr_ptr_t
synapse_runtime_wrappers_decode_mac_address(string_ptr_t encoded);

p4_uint32_ptr_t synapse_runtime_wrappers_decode_p4_uint32(string_ptr_t encoded);

port_ptr_t synapse_runtime_wrappers_decode_port(string_ptr_t encoded);

#ifdef __cplusplus
}
#endif // __cplusplus

// Stack

#ifdef __cplusplus
typedef struct Stack {
public:
  void *push(void *el);

  void *pop();

  void *top();

  bool empty();

  size_t size();

  size_t clear();

private:
  std::stack<void *> stack_;

} stack_t;
#else  //__cplusplus
struct Stack;
typedef struct Stack stack_t;
#endif // __cplusplus
typedef stack_t *stack_ptr_t;

// Pair

typedef struct Pair {
#ifdef __cplusplus
  Pair();

  Pair(void *left, void *right);
#endif // __cplusplus

  void *left;
  void *right;

} pair_t;
typedef pair_t *pair_ptr_t;

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_UTILS_WRAPPERS_HPP_