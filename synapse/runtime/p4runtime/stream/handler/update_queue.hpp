#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_UPDATE_QUEUE_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_UPDATE_QUEUE_HPP_

#ifdef __cplusplus
#include <queue>
#endif // __cplusplus

#include "synapse/runtime/p4runtime/helper.hpp"

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
typedef class UpdateQueue {
public:
  UpdateQueue(const helper_ptr_t &helper);

  bool empty();

  bool flush(p4_write_request_ptr_t &request);

  bool queue(p4_update_ptr_t update);

private:
  std::queue<p4_update_ptr_t> queue_;
  helper_ptr_t helper_;

} update_queue_t;
#endif // __cplusplus

#ifdef __cplusplus
typedef update_queue_t *update_queue_ptr_t;
#else
struct update_queue;
typedef struct update_queue update_queue_t;
typedef update_queue_t *update_queue_ptr_t;
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_UPDATE_QUEUE_HPP_