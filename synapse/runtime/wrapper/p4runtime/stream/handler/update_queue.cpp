#include "synapse/runtime/wrapper/p4runtime/stream/handler/update_queue.hpp"

#define NOT_NULL(ptr) assert(nullptr != ptr)

namespace synapse::runtime {

bool synapse_runtime_update_queue_queue(update_queue_ptr_t queue,
                                        p4_update_ptr_t update) {
  NOT_NULL(queue);
  return queue->queue(update);
}

} // namespace synapse::runtime