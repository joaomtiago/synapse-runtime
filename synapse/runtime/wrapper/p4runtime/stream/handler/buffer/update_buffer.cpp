#include "synapse/runtime/wrapper/p4runtime/stream/handler/buffer/update_buffer.hpp"

namespace synapse::runtime {

bool synapse_runtime_update_buffer_buffer(upd_buff_ptr_t update_buffer,
                                          p4_update_ptr_t update) {
  return update_buffer->buffer(update);
}

} // namespace synapse::runtime