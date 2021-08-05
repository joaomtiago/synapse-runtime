#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_LISTENER_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_LISTENER_HPP_

#include "synapse/runtime/connector.hpp"
#include "synapse/runtime/p4runtime/stream/handler/environment.hpp"
#include "synapse/runtime/p4runtime/stream/tags.hpp"

namespace synapse::runtime {

class Listener {
public:
  Listener(conn_ptr_t conn);

  virtual void listen();

private:
  /**
   * Based on the consumed completion queue tag, invoke the function associated
   * with that tag.
   */
  bool dispatch(tag_t *tag);

private:
  // Environment passed to handlers
  env_ptr_t env_;

  // Singleton structure to reuse tags
  tags_t tags_;

  // The gRPC client context used exclusively for the stream
  grpc_cctx_ptr_t context_;

  // Queue bound to the stream that stores completion tags
  grpc_cqueue_ptr_t queue_;

  stack_ptr_t stack_;
};

} // namespace synapse::runtime

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_LISTENER_HPP_