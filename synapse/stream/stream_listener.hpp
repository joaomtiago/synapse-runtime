#ifndef SYNAPSE_P4RUNTIME_STREAM_STREAM_LISTENER_HPP_
#define SYNAPSE_P4RUNTIME_STREAM_STREAM_LISTENER_HPP_

#include "synapse/connector/connector.hpp"
#include "synapse/stream/handler/environment/listener_environment.hpp"
#include "synapse/stream/handler/environment/user_environment.hpp"

namespace synapse::p4runtime {

class StreamListener {
 public:
  /**
   * Prepare the environment for the creation of the asynchronous bi-directional
   * stream between the connector and the gRPC server,create the P4Runtime
   * updates, and set the first listener-space handler to be invoked on the
   * start state of the completion queue state machine.
   */
  StreamListener(conn_ptr_t connector, listener_handler_ptr_t handler);

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
  bool dispatch(tag_t* tag);

 public:
  // The gRPC client context used exclusively for the stream.
  ctx_ptr_t context;
  // Queue bound to the stream that stores completion tags.
  queue_ptr_t queue;

  // Listener-space environment.
  listener_env_t* env;
  // User-space environment.
  user_env_t* user_env;
};

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_STREAM_STREAM_LISTENER_HPP_