#include "connector_params.hpp"

namespace synapse::p4runtime {

void ConnectorParams::put(const std::string &key, const std::string &value) {
  dict_[key] = value;
}

std::string ConnectorParams::find(const std::string &key) {
  return dict_.find(key)->second;
}

void ConnectorParams::erase(const std::string &key) { dict_.erase(key); }

}  // namespace synapse::p4runtime
