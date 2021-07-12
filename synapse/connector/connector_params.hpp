#ifndef SYNAPSE_P4RUNTIME_CONNECTOR_CONNECTOR_PARAMS_HPP_
#define SYNAPSE_P4RUNTIME_CONNECTOR_CONNECTOR_PARAMS_HPP_

#include <string>
#include <unordered_map>

namespace synapse::p4runtime {

class ConnectorParams {
  /**
   * This is very basic key-value dictionary.
   * It quashes the need for passing string parameters in the tag.
   * Just add them to the dictionary, and retrieve them when needed.
   */

 public:
  void put(const std::string &key, const std::string &value);

  std::string find(const std::string &key);

  void erase(const std::string &key);

 private:
  std::unordered_map<std::string, std::string> dict_;
};

typedef ConnectorParams *params_ptr_t;

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_CONNECTOR_CONNECTOR_PARAMS_HPP_