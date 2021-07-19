#ifndef SYNAPSE_RUNTIME_UTILS_RUNTIME_EXCEPTION_HPP_
#define SYNAPSE_RUNTIME_UTILS_RUNTIME_EXCEPTION_HPP_

#include <stdexcept>

namespace synapse::runtime {

class RuntimeException : public std::runtime_error {
public:
  inline RuntimeException(const std::string &message)
      : std::runtime_error(message) {}
};

} // namespace synapse::runtime

#endif // SYNAPSE_RUNTIME_UTILS_RUNTIME_EXCEPTION_HPP_