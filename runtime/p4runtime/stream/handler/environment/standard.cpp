#include "standard.hpp"

namespace synapse::runtime {

void StandardEnvironment::read(p4_stream_message_response_t *response,
                               tag_t *tag) {
  stream->Read(response, static_cast<void *>(tag));
}

void StandardEnvironment::write(p4_stream_message_request_t *request,
                                tag_t *tag) {
  stream->Write(*request, static_cast<void *>(tag));
}

} // namespace synapse::runtime
