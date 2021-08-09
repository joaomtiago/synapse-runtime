#include "synapse/runtime/p4runtime/stream/listener.hpp"

#define NOT_NULL(ptr) assert(nullptr != ptr)

#define ALLOC_TAGS(tag, size)                                                  \
  tag.next_tags = (void **)std::malloc((size) * sizeof(void *));               \
  NOT_NULL(tag.next_tags)

namespace synapse::runtime {

Listener::Listener(conn_ptr_t conn) {
  env_ = new env_t();
  env_->helper = new helper_t(S_CAST(std::string *, conn->stack->pop()));
  env_->stack = new stack_t();
  env_->update_buffer = new upd_buff_t(env_->helper);

  ALLOC_TAGS(tags_.tagConnected, 1);
  tags_.tagConnected.next_tags[0] = &tags_.tagMakePrimarySent;

  ALLOC_TAGS(tags_.tagMakePrimarySent, 1);
  tags_.tagMakePrimarySent.next_tags[0] = &tags_.tagMakePrimaryReceived;

  ALLOC_TAGS(tags_.tagMakePrimaryReceived, 1);
  tags_.tagMakePrimaryReceived.next_tags[0] = &tags_.tagMessageReceived;

  ALLOC_TAGS(tags_.tagMessageReceived, 2);
  tags_.tagMessageReceived.next_tags[0] = &tags_.tagMessageReceived;
  tags_.tagMessageReceived.next_tags[1] = &tags_.tagMessageSent;

  ALLOC_TAGS(tags_.tagMessageSent, 1);
  tags_.tagMessageSent.next_tags[0] = &tags_.tagMessageReceived;

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
  SYNAPSE_DEBUG("Connected to the stream");

  void *rawTag = nullptr;
  bool ok = false;

  while (queue_->Next(&rawTag, &ok)) {
    if (!ok) {
      SYNAPSE_ERROR("The stream closed unexpectedly");
      break;
    }

    if (!dispatch(S_CAST(tag_t *, rawTag))) {
      SYNAPSE_INFO("Handler caused the stream to be closed");
      break;
    }
  }

  SYNAPSE_DEBUG("Disconnected from the stream");
}

bool Listener::dispatch(tag_t *tag) {
  SYNAPSE_DEBUG("Dispatching tag `%d`", S_CAST(int, tag->state));
  return nullptr == tag->handler || tag->handler(env_, stack_, tag->next_tags);
}

} // namespace synapse::runtime
