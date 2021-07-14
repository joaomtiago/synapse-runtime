#include "listener_environment.hpp"

namespace synapse::p4runtime {

void ListenerEnvironment::read(p4_stream_message_response_t* response, tag_t* tag) {
  stream->Read(response, static_cast<void*>(tag));
}

void ListenerEnvironment::write(p4_stream_message_request_t* request, tag_t* tag) {
  stream->Write(*request, static_cast<void*>(tag));
}

}  // namespace synapse::p4runtime
