#ifndef SYNAPSE_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_LISTENER_ENVIRONMENT_HPP_
#define SYNAPSE_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_LISTENER_ENVIRONMENT_HPP_

#include "synapse/connector/connector.hpp"
#include "synapse/stream/buffer/update_buffer.hpp"
#include "synapse/stream/handler/environment/user_environment.hpp"
#include "synapse/utils/typedefs.hpp"

namespace synapse::p4runtime {

/**
 * The asynchronous bi-directional gRPC stream offers two asynchronous methods:
 * read and write. Because the stream is asynchronous, these calls are not
 * blocking: they return immediately. To cope with this, gRPC implements a
 * completion queue that keeps `tags` linked to events that have been completed.
 *
 * E.g.: After a read operation has been successful, gRPC places a
 * `ReadCompleted` tag (illustrative name) on the completion queue to notify you
 * of the event. That tag may be stateful, and store metadata such as the
 * message that was sent, for instance.
 *
 * The mechanism is similar to a finite state machine, in the sense that, when
 * you consume a tag from the queue, if you have to tell gRPC which tag you
 * expect next to exist on the queue for you to consume. In other words, you are
 * telling gRPC the next state you transition to after consuming that tag.
 *
 * Each tag has an associated handler. In the example above, we would invoke
 * `handleReadCompleted`. This is a listener-space handler (i.e. it is a raw
 * handler meant to process messages that come directly from the completion
 * queue). Listener-space handlers, in turn, may invoke user-space handlers
 * (i.e. user-defined handlers that deal with stream messages that have been
 * previously processed by a listener-space handler). By definition, user-space
 * handlers have limited access to the modifiadble state. That's why we keep two
 * separate environments: a listener-space environment (full control over the
 * mutable state), and user-space environment (to narrow down the scope of
 * user-defined operations).
 */

// The states of the state machine.
typedef enum {
  Connected = 1,
  MakePrimarySent = 2,
  MakePrimaryReceived = 3,
  MessageReceived = 4

} tag_state_t;

struct ListenerEnvironment;
typedef bool (*listener_handler_ptr_t)(ListenerEnvironment*);

typedef struct {
  // Current state of the tag.
  tag_state_t state;
  // A pointer to the next handler in the state machine.
  listener_handler_ptr_t next_handler;

} tag_t;

/**
 * We avoid creating a new `Tag` every time we transition to a different state.
 * Instead, we instantiate, a priori, one `Tag` for every `TagState` in a new
 * structure kept by a stream listener, and update pointers.
 */
typedef struct {
  tag_t tagConnected = {tag_state_t::Connected, nullptr};
  tag_t tagMakePrimarySent = {tag_state_t::MakePrimarySent, nullptr};
  tag_t tagMakePrimaryReceived = {tag_state_t::MakePrimaryReceived, nullptr};
  tag_t tagMessageReceived = {tag_state_t::MessageReceived, nullptr};

} tags_t;

typedef struct ListenerEnvironment {
  void read(p4_stream_message_response_t* response, tag_t* tag);
  void write(p4_stream_message_request_t* request, tag_t* tag);

  tags_t tags;

  stream_ptr_t stream;
  p4_stream_message_request_t* request;
  p4_stream_message_response_t* response;

  conn_ptr_t connector;
  upd_buff_ptr_t update_buffer;

  // Keep a pointer to the user-space environment.
  user_env_ptr_t user_env;

} listener_env_t;

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_LISTENER_ENVIRONMENT_HPP_