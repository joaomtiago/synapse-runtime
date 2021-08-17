#include "synapse/runtime/p4runtime/stream/listener.hpp"

#define NOT_NULL(ptr) assert(nullptr != ptr)

#define ALLOC_TAGS(tag, size)                                                  \
  tag.next_tags = (void **)std::malloc((size) * sizeof(void *));               \
  NOT_NULL(tag.next_tags)

namespace synapse::runtime {

void configureTags(tags_t &tags) {
  ALLOC_TAGS(tags.tagConnected, 1);
  tags.tagConnected.next_tags[0] = &tags.tagMakePrimarySent;

  ALLOC_TAGS(tags.tagMakePrimarySent, 1);
  tags.tagMakePrimarySent.next_tags[0] = &tags.tagMakePrimaryReceived;

  ALLOC_TAGS(tags.tagMakePrimaryReceived, 1);
  tags.tagMakePrimaryReceived.next_tags[0] = &tags.tagMessageReceived;

  ALLOC_TAGS(tags.tagMessageReceived, 2);
  tags.tagMessageReceived.next_tags[0] = &tags.tagMessageReceived;
  tags.tagMessageReceived.next_tags[1] = &tags.tagMessageSent;

  ALLOC_TAGS(tags.tagMessageSent, 1);
  tags.tagMessageSent.next_tags[0] = &tags.tagMessageReceived;
}

Listener::Listener(conn_ptr_t conn) {
  env_ = new env_t();
  env_->helper = new helper_t(S_CAST(std::string *, conn->stack->pop()));
  env_->queue = new update_queue_t(env_->helper);
  env_->stack = new stack_t();

  configureTags(tags_);

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
