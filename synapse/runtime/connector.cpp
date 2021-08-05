#include "synapse/runtime/connector.hpp"
#include "synapse/runtime/exception/runtime_exception.hpp"
#include "synapse/runtime/p4runtime/stream/handler/standard.hpp"
#include "synapse/runtime/p4runtime/stream/listener.hpp"
#include <grpcpp/grpcpp.h>
#include <thread>

#define NOW gpr_now(GPR_CLOCK_REALTIME)
#define TIMEOUT gpr_time_from_seconds(30, GPR_TIMESPAN)
#define DEADLINE gpr_time_add(NOW, TIMEOUT)

namespace synapse::runtime {

Connector::Connector(const std::string &grpcAddr) {
  auto credentials = grpc::InsecureChannelCredentials();
  auto channel = grpc::CreateChannel(grpcAddr, credentials);

  // Wait 30 s for the connection to be established
  if (!channel->WaitForConnected(DEADLINE)) {
    throw RuntimeException("Waited 30 s for the connection to be established");
  }

  // The stub may have to be used before the listener starts
  stub = p4::v1::P4Runtime::NewStub(channel).release();

  // Finally, create the arguments stack
  stack = new stack_t();
}

bool Connector::configure(const std::string &bmv2JsonFilepath,
                          const std::string &p4InfoFilepath) {
  stack->empty();
  stack->push(new std::string(bmv2JsonFilepath));
  stack->push(new std::string(p4InfoFilepath));

  return 2 == stack->size();
}

bool Connector::startAndWait() {
  auto listener = std::thread(&Listener::listen, new Listener(this));
  if (!listener.joinable()) {
    return false;
  }

  listener.join();
  return true;
}

} // namespace synapse::runtime