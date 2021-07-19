#include "connector.hpp"
#include "synapse/runtime/p4runtime/stream/listener.hpp"
#include "synapse/runtime/utils/runtime_exception.hpp"
#include <grpcpp/grpcpp.h>
#include <thread>

#define NOW gpr_now(GPR_CLOCK_REALTIME)
#define TIMEOUT gpr_time_from_seconds(30, GPR_TIMESPAN)
#define DEADLINE gpr_time_add(NOW, TIMEOUT)

namespace synapse::runtime {

/**
 * Connector::ConnectorParams below
 */

void Connector::ConnectorParams::put(const std::string &key,
                                     const std::string &value) {
  dict_[key] = value;
}

std::string Connector::ConnectorParams::find(const std::string &key) {
  return dict_.find(key)->second;
}

void Connector::ConnectorParams::erase(const std::string &key) {
  dict_.erase(key);
}

/**
 * Connector below
 */

Connector::Connector(const std::string &grpcAddr,
                     const std::string &p4InfoFilepath,
                     logging_level_t loggingLevel) {
  // Instantiate logger with specified loggin level.
  logger_ = new logger_t(loggingLevel);

  auto credentials = grpc::InsecureChannelCredentials();
  auto channel = grpc::CreateChannel(grpcAddr, credentials);

  // Wait 30 s for the connection to be established.
  if (!channel->WaitForConnected(DEADLINE)) {
    throw RuntimeException("Waited 30 s for the connection to be established");
  }

  helper = new helper_t(p4InfoFilepath);

  // The stub may have to be used before the listener starts.
  stub = p4::v1::P4Runtime::NewStub(channel);
}

bool Connector::startAndWait(const std::string &bmv2JsonFilepath) {
  auto listener = new Listener(this, &handleConnected, logger_);
  params.put("bmv2", bmv2JsonFilepath);

  listener_thread_ = std::thread(&Listener::listen, listener);
  if (!listener_thread_.joinable()) {
    return false;
  }

  listener_thread_.join();
  return true;
}

} // namespace synapse::runtime