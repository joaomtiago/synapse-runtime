#include "listener_environment.hpp"

namespace synapse::p4runtime {

void ListenerEnvironment::read(stream_response_t* response, tag_t* tag) {
  stream->Read(response, static_cast<void*>(tag));
}

void ListenerEnvironment::write(stream_request_t* request, tag_t* tag) {
  stream->Write(*request, static_cast<void*>(tag));
}

}  // namespace synapse::p4runtime
