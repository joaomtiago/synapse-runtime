#include "update_buffer_wrapper.h"

namespace synapse::p4runtime {

bool synapse_update_buffer_buffer(upd_buff_ptr_t update_buffer,
                                  p4_update_t* update) {
  return nullptr != update_buffer && update_buffer->buffer(update);
}

p4_write_request_t* synapse_update_buffer_flush(upd_buff_ptr_t update_buffer) {
  if (update_buffer == nullptr) {
    return nullptr;
  }

  return update_buffer->flush();
}

}  // namespace synapse::p4runtime
