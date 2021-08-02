#include "synapse/runtime/wrapper/p4runtime/stream/handler/buffer/update_buffer.hpp"
#include "synapse/runtime/p4runtime/constants.hpp"

namespace synapse::runtime {

UpdateBuffer::UpdateBuffer(const helper_ptr_t &helper) : buffer_() {
  helper_ = helper;
}

bool UpdateBuffer::buffer(p4_update_ptr_t update) {
  if (nullptr == update) {
    return false;
  }

  buffer_.push(update);
  return true;
}

bool UpdateBuffer::flush(p4_write_request_ptr_t &request) {
  if (buffer_.empty()) {
    return false;
  }

  std::vector<p4_update_ptr_t> *updates = new std::vector<p4_update_ptr_t>();
  while (!buffer_.empty()) {
    updates->push_back(buffer_.front());
    buffer_.pop();
  }

  request = helper_->p4WriteRequest(
      SYNAPSE_DEVICE_ID, updates,
      p4_write_request_atomicity_t::WriteRequest_Atomicity_CONTINUE_ON_ERROR,
      SYNAPSE_ELECTION_ID_LOW, SYNAPSE_ELECTION_ID_HIGH);
  return true;
}

} // namespace synapse::runtime
