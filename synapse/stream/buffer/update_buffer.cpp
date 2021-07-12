#include "update_buffer.hpp"

namespace synapse::p4runtime {

UpdateBuffer::UpdateBuffer(const helper_ptr_t& helper) : buffer_() {
  helper_ = helper;
}

void UpdateBuffer::buffer(p4::v1::Update* update) {
  if (update) {
    std::cout << "[*] buffering update:" << std::endl;
    update->PrintDebugString();
    buffer_.push(update);
  }
}

p4::v1::WriteRequest* UpdateBuffer::flush() {
  auto _updates = new std::vector<p4::v1::Update*>();

  while (!buffer_.empty()) {
    _updates->push_back(buffer_.front());
    buffer_.pop();
  }

  return helper_->writeRequest(
      0, _updates, p4::v1::WriteRequest_Atomicity_CONTINUE_ON_ERROR, 1, 0);
}

}  // namespace synapse::p4runtime
