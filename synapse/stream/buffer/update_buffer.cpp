#include "update_buffer.hpp"

namespace synapse::p4runtime {

UpdateBuffer::UpdateBuffer(const helper_ptr_t& helper) : buffer_() {
  helper_ = helper;
}

bool UpdateBuffer::buffer(p4_update_t* update) {
  if (nullptr == update) {
    return false;
  }

  buffer_.push(update);
  return true;
}

p4_write_request_t* UpdateBuffer::flush() {
  auto updates = new std::vector<p4_update_t*>();

  while (!buffer_.empty()) {
    updates->push_back(buffer_.front());
    buffer_.pop();
  }

  return helper_->writeRequest(
      0, updates, p4::v1::WriteRequest_Atomicity_CONTINUE_ON_ERROR, 1, 0);
}

}  // namespace synapse::p4runtime
