#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_STANDARD_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_STANDARD_HPP_

#include "synapse/runtime/p4runtime/stream/handler/environment/custom.hpp"
#include "synapse/runtime/utils/logger.hpp"
#include "synapse/runtime/wrapper/connector.hpp"
#include "synapse/runtime/wrapper/p4runtime/stream/handler/environment/buffer/update_buffer.hpp"

namespace synapse::runtime {

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
 * you consume a tag from the queue, you have to tell gRPC which tag you expect
 * next to exist on the queue for you to consume. In other words, you are
 * telling gRPC the next state you transition to after consuming that tag.
 *
 * Each tag has an associated handler. In the example above, we would invoke
 * `handleReadCompleted`. This is a standard handler (i.e. it is a raw handler
 * meant to process messages that come directly from the completion queue).
 * Custom handlers, in turn, are controller-defined, and, thus, implement the
 * controller logic, and drive the controller program. These handlers have
 * limited access to the modifiable state. That's why we keep two separate
 * environments: a standard environment (full control over the mutable state),
 * and custom environment (to narrow down the scope of controller-defined
 * operations).
 */

// The states of the state machine
typedef enum {
  Connected = 1,
  MakePrimarySent = 2,
  MakePrimaryReceived = 3,
  MessageReceived = 4

} tag_state_t;

struct StandardEnvironment;
typedef bool (*standard_handler_ptr_t)(StandardEnvironment *);

typedef struct {
  // Current state of the tag
  tag_state_t state;
  // A pointer to the next handler in the state machine
  standard_handler_ptr_t next_handler;

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

typedef struct StandardEnvironment {
  void read(p4_stream_message_response_t *response, tag_t *tag);
  void write(p4_stream_message_request_t *request, tag_t *tag);

  tags_t tags;

  p4runtime_stream_ptr_t stream;
  p4_stream_message_request_t *request;
  p4_stream_message_response_t *response;

  conn_ptr_t connector;
  upd_buff_ptr_t update_buffer;

  logger_ptr_t logger;

  // Keep a pointer to the custom environment
  custom_env_ptr_t custom_env;

} standard_env_t;

typedef standard_env_t *standard_env_ptr_t;
typedef bool (*standard_handler_ptr_t)(standard_env_ptr_t);

} // namespace synapse::runtime

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_STANDARD_HPP_