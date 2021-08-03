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
  String(std::string *string);

  String(const std::string &string);

  String(const char *value, size_t value_sz);
#endif // __cplusplus

  const char *value;
  size_t size;

#ifdef __cplusplus
  std::string toStdString();
#endif // __cplusplus

} string_t;
typedef string_t *string_ptr_t;

// MAC address

typedef struct MACAddress {
#ifdef __cplusplus
  MACAddress(const char *address);

  MACAddress(const char *address, const char *raw);
#endif // __cplusplus

  string_ptr_t address;
  string_ptr_t raw;

} mac_addr_t;
typedef mac_addr_t *mac_addr_ptr_t;

// Port

typedef struct Port {
#ifdef __cplusplus
  Port(const uint16_t &port);

  Port(const uint16_t &port, string_ptr_t raw);
#endif // __cplusplus

  uint16_t port;
  string_ptr_t raw;

} port_t;
typedef port_t *port_ptr_t;

// Decoders

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

mac_addr_ptr_t synapse_runtime_wrappers_decode_mac_address(const char *encoded);

port_ptr_t synapse_runtime_wrappers_decode_port(string_ptr_t encoded);

#ifdef __cplusplus
}
#endif // __cplusplus

// Stack

#ifdef __cplusplus
typedef struct Stack {
public:
  void push(void *el);

  void *pop();

  void *top();

  bool empty();

  size_t size();

  size_t clear();

private:
  std::stack<void *> stack_;

} stack_t;
#else
struct Stack;
typedef struct Stack stack_t;
#endif // __cplusplus
typedef stack_t *stack_ptr_t;

// Pair

typedef struct Pair {
#ifdef __cplusplus
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