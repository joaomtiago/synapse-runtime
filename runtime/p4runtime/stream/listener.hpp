#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_LISTENER_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_LISTENER_HPP_

#include "handler/standard.hpp"
#include "runtime/connector.hpp"
#include "runtime/utils/logger.hpp"

namespace synapse::runtime {

class Listener {
public:
  /**
   * Prepare the environment for the creation of the asynchronous bi-directional
   * stream between the connector and the gRPC server,create the P4Runtime
   * updates, and set the first listener-space handler to be invoked on the
   * start state of the completion queue state machine.
   */
  Listener(conn_ptr_t connector, standard_handler_ptr_t handler,
           logger_ptr_t logger);

  /**
   * The thread's run function, that waits for the notification of a new stream
   * message to be processed. This function calls the handler invocation
   * mechanism.
   */
  virtual void listen();

private:
  /**
   * Based on the consumed completion queue tag, invoke the function associated
   * with that tag.
   */
  bool dispatch(tag_t *tag);

public:
  // The gRPC client context used exclusively for the stream.
  grpc_cctx_ptr_t context;
  // Queue bound to the stream that stores completion tags.
  grpc_cqueue_ptr_t queue;

  // Listener-space environment.
  standard_env_ptr_t standard_env;
  // User-space environment.
  custom_env_ptr_t custom_env;
};

} // namespace synapse::runtime

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_LISTENER_HPP_