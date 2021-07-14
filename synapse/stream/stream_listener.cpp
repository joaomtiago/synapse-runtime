#include "synapse/stream/stream_listener.hpp"

namespace synapse::p4runtime {

StreamListener::StreamListener(conn_ptr_t connector,
                               listener_handler_ptr_t handler) {
  // Create gRPC client context, and completion queue.
  context = std::make_shared<ctx_t>();
  queue = std::make_shared<queue_t>();

  // Instantiate the listener-space environment.
  env = new listener_env_t();
  env->tags.tagConnected.next_handler = handler;
  env->stream = std::move(connector->stub->AsyncStreamChannel(
      context.get(), queue.get(),
      static_cast<void *>(&env->tags.tagConnected)));
  env->request = nullptr;
  env->response = nullptr;
  env->connector = connector;
  env->update_buffer = new upd_buff_t(connector->helper);

  // Instantiate the user-space environment.
  auto userEnv = new user_env_t();
  userEnv->helper = connector->helper;
  userEnv->info_helper = connector->info_helper;
  userEnv->update_buffer = env->update_buffer;
  env->user_env = userEnv;
}

void StreamListener::listen() {
  std::cout << "[+] started stream handler" << std::endl;
  void *rawTag = nullptr;
  bool ok = false;
  while (queue->Next(&rawTag, &ok) && ok) {
    if (!dispatch(static_cast<tag_t *>(rawTag))) {
      break;
    }
  }

  std::cout << "[*] stream handler stopped" << std::endl;
}

bool StreamListener::dispatch(tag_t *tag) {
  if (tag->next_handler != nullptr) {
    return tag->next_handler(env);
  }

  // there's no function to run, assume everything went fine
  return true;
}

}  // namespace synapse::p4runtime
