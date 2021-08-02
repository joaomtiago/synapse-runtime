#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_TAGS_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_TAGS_HPP_

#include "synapse/runtime/p4runtime/stream/handler/environment.hpp"
#include "synapse/runtime/p4runtime/stream/handler/standard.hpp"

namespace synapse::runtime {

typedef enum {
  Connected = 1,
  MakePrimarySent = 2,
  MakePrimaryReceived = 3,
  MessageReceived = 4,
  MessageSent = 5,
  MessageSentDone = 6

} tag_state_t;

typedef struct {
  std::string identifier;
  handler_ptr_t handler;
  void **next_tags;

} tag_t;

typedef struct {
  tag_t tagConnected = {"Connected", &handleConnected, nullptr};
  tag_t tagMakePrimarySent = {"MakePrimarySent", &handleMakePrimarySent,
                              nullptr};
  tag_t tagMakePrimaryReceived = {"MakePrimaryReceived",
                                  &handleMakePrimaryReceived, nullptr};
  tag_t tagMessageReceived = {"MessageReceived", &handleMessageReceived,
                              nullptr};
  tag_t tagMessageSent = {"MessageSent", &handleMessageSent, nullptr};

} tags_t;

} // namespace synapse::runtime

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_TAGS_HPP_
