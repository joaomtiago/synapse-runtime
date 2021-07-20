#include "synapse/runtime/p4runtime/stream/handler/environment/buffer/update_buffer.hpp"
#include "synapse/runtime/utils/constants.hpp"

namespace synapse::runtime {

UpdateBuffer::UpdateBuffer(const helper_ptr_t &helper) : buffer_() {
  helper_ = helper;
}

bool UpdateBuffer::buffer(p4_update_t *update) {
  if (nullptr == update) {
    return false;
  }

  buffer_.push(update);
  return true;
}

p4_write_request_t *UpdateBuffer::flush() {
  auto updates = new std::vector<p4_update_t *>();

  while (!buffer_.empty()) {
    updates->push_back(buffer_.front());
    buffer_.pop();
  }

  return helper_->p4WriteRequest(
      DEVICE_ID, updates,
      p4_write_request_atomicity_t::WriteRequest_Atomicity_CONTINUE_ON_ERROR,
      ELECTION_ID_LOW, ELECTION_ID_HIGH);
}

} // namespace synapse::runtime
