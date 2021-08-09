#include "synapse/runtime/wrapper/p4runtime/stream/handler/buffer/update_buffer.hpp"

#define NOT_NULL(ptr) assert(nullptr != ptr)

namespace synapse::runtime {

bool synapse_runtime_update_buffer_buffer(upd_buff_ptr_t update_buffer,
                                          p4_update_ptr_t update) {
  NOT_NULL(update_buffer);
  return update_buffer->buffer(update);
}

} // namespace synapse::runtime