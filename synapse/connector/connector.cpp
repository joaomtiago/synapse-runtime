#include "connector.hpp"

#include <grpcpp/grpcpp.h>

#include <thread>

#include "synapse/stream/handler/listener_handlers.hpp"
#include "synapse/stream/stream_listener.hpp"

#define NOW gpr_now(GPR_CLOCK_REALTIME)
#define TIMEOUT gpr_time_from_seconds(30, GPR_TIMESPAN)
#define DEADLINE gpr_time_add(NOW, TIMEOUT)

namespace synapse::p4runtime {

Connector::Connector(const std::string &grpcAddr,
                     const std::string &p4InfoFilepath) {
  auto credentials = grpc::InsecureChannelCredentials();
  auto channel = grpc::CreateChannel(grpcAddr, credentials);

  // Wait 30 s for the connection to be established.
  if (!channel->WaitForConnected(DEADLINE)) {
    std::cerr << "waited 30 s for the connection to be established"
              << std::endl;
    exit(1);
  }

  helper = new helper_t();
  info_helper = new info_helper_t(p4InfoFilepath);

  params = new ConnectorParams();

  // The stub may have to be used before the listener starts.
  stub = p4::v1::P4Runtime::NewStub(channel);
}

bool Connector::startAndWait(const std::string &bmv2JsonFilepath) {
  StreamListener *listener = new StreamListener(this, &handleConnected);
  params->put("bmv2", bmv2JsonFilepath);

  stream_listener_ = std::thread(&StreamListener::listen, listener);
  if (!stream_listener_.joinable()) {
    return 1;
  }

  stream_listener_.join();
  return 0;
}

}  // namespace synapse::p4runtime