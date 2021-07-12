#ifndef SYNAPSE_P4RUNTIME_CONNECTOR_CONNECTOR_HPP_
#define SYNAPSE_P4RUNTIME_CONNECTOR_CONNECTOR_HPP_

#include <thread>

#include "synapse/connector/connector_params.hpp"
#include "synapse/utils/typedefs.hpp"

namespace synapse::p4runtime {

class Connector {
 public:
  /**
   * Create the gRPC channel, using insecure channel credentials, wait for a
   * successful connection to be established, create the key-value dictionary,
   * build the Protobuf helpers, and create the stub.
   */
  Connector(const std::string &grpcAddr, const std::string &p4InfoFilepath);

  /**
   * Create the asycnhronous bi-directional stream handler, launch the gRPC
   * thread that dispatches stream messages to the respective handlers, and wait
   * for the gRPC thread to join.
   */
  bool startAndWait(const std::string &bmv2JsonFilepath);

 public:
  params_ptr_t params;

  info_helper_ptr_t info_helper;
  helper_ptr_t helper;

  stub_ptr_t stub;

 private:
  std::thread stream_listener_;
};

typedef Connector conn_t;
typedef conn_t *conn_ptr_t;

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_CONNECTOR_CONNECTOR_HPP_