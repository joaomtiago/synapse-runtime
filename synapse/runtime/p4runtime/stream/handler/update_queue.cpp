#include "synapse/runtime/wrapper/p4runtime/stream/handler/update_queue.hpp"
#include "synapse/runtime/p4runtime/constants.hpp"

namespace synapse::runtime {

UpdateQueue::UpdateQueue(const helper_ptr_t &helper) : queue_() {
  helper_ = helper;
}

bool UpdateQueue::empty() { return queue_.empty(); }

bool UpdateQueue::flush(p4_write_request_ptr_t &request) {
  auto updates = new std::vector<p4_update_ptr_t>();
  while (!queue_.empty()) {
    updates->push_back(queue_.front());
    queue_.pop();
  }

  return nullptr != (request = helper_->p4WriteRequest(
                         SYNAPSE_DEVICE_ID, updates,
                         p4_write_request_atomicity_t::
                             WriteRequest_Atomicity_CONTINUE_ON_ERROR,
                         SYNAPSE_ELECTION_ID_LOW, SYNAPSE_ELECTION_ID_HIGH));
}

bool UpdateQueue::queue(p4_update_ptr_t update) {
  if (nullptr == update) {
    return false;
  }

  queue_.push(update);
  return true;
}

} // namespace synapse::runtime
