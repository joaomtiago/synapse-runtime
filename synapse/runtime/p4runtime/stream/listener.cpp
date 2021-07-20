#include "synapse/runtime/p4runtime/stream/listener.hpp"

namespace synapse::runtime {

Listener::Listener(conn_ptr_t connector, standard_handler_ptr_t handler,
                   logger_ptr_t logger) {
  // Create gRPC client context, and completion queue.
  context = std::make_shared<grpc_cctx_t>();
  queue = std::make_shared<grpc_cqueue_t>();

  // Instantiate the standard environment.
  standard_env = new standard_env_t();
  standard_env->tags.tagConnected.next_handler = handler;
  standard_env->stream = std::move(connector->stub->AsyncStreamChannel(
      context.get(), queue.get(),
      static_cast<void *>(&standard_env->tags.tagConnected)));
  standard_env->request = nullptr;
  standard_env->response = nullptr;
  standard_env->connector = connector;
  standard_env->update_buffer = new upd_buff_t(connector->helper);
  standard_env->logger = logger;

  // Instantiate the user-space environment.
  custom_env = new custom_env_t();
  custom_env->helper = connector->helper;
  custom_env->update_buffer = standard_env->update_buffer;
  custom_env->logger = logger;
  standard_env->custom_env = custom_env;
}

void Listener::listen() {
  standard_env->logger->info("Started stream listener");
  void *rawTag = nullptr;
  bool ok = false;
  while (queue->Next(&rawTag, &ok) && ok) {
    if (!dispatch(static_cast<tag_t *>(rawTag))) {
      break;
    }
  }

  standard_env->logger->debug("Stream listener has stopped");
}

bool Listener::dispatch(tag_t *tag) {
  if (tag->next_handler != nullptr) {
    return tag->next_handler(standard_env);
  }

  // there's no function to run, assume everything went fine
  return true;
}

} // namespace synapse::runtime
