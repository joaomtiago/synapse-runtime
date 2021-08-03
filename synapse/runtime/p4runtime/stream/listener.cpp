#include "synapse/runtime/p4runtime/stream/listener.hpp"

namespace synapse::runtime {

Listener::Listener(conn_ptr_t conn, logger_ptr_t logger) {
  env_ = new env_t();
  env_->helper = new helper_t(S_CAST(std::string *, conn->stack->pop()));
  env_->logger = logger;
  env_->stack = new stack_t();
  env_->update_buffer = new upd_buff_t(env_->helper);

  logger_ = logger;

  tags_.tagConnected.next_tags = (void **)std::malloc(sizeof(void *));
  *tags_.tagConnected.next_tags = &tags_.tagMakePrimarySent;

  tags_.tagMakePrimarySent.next_tags = (void **)std::malloc(sizeof(void *));
  *tags_.tagMakePrimarySent.next_tags = &tags_.tagMakePrimaryReceived;

  tags_.tagMakePrimaryReceived.next_tags = (void **)std::malloc(sizeof(void *));
  *tags_.tagMakePrimaryReceived.next_tags = &tags_.tagMessageReceived;

  tags_.tagMessageReceived.next_tags = (void **)std::malloc(2 * sizeof(void *));
  *tags_.tagMessageReceived.next_tags = &tags_.tagMessageReceived;
  *(tags_.tagMessageReceived.next_tags + 1) = &tags_.tagMessageSent;

  tags_.tagMessageSent.next_tags = (void **)std::malloc(sizeof(void *));
  *tags_.tagMessageSent.next_tags = &tags_.tagMessageReceived;

  context_ = std::make_shared<grpc_cctx_t>();
  queue_ = std::make_shared<grpc_cqueue_t>();
  p4runtime_stream_ptr_t stream =
      conn->stub
          ->AsyncStreamChannel(context_.get(), queue_.get(),
                               &tags_.tagConnected)
          .release();

  stack_ = new stack_t();
  stack_->push(conn->stack->pop());
  stack_->push(conn->stub);
  stack_->push(stream);
}

void Listener::listen() {
  logger_->debug("Listener: start");

  void *rawTag = nullptr;
  bool ok = false;

  while (queue_->Next(&rawTag, &ok) && ok) {
    if (!dispatch(S_CAST(tag_t *, rawTag))) {
      break;
    }
  }

  logger_->debug("Listener: stop");
}

bool Listener::dispatch(tag_t *tag) {
  logger_->debug("Listener: dispatching tag " + tag->identifier);
  return nullptr == tag->handler || tag->handler(env_, stack_, tag->next_tags);
}

} // namespace synapse::runtime
