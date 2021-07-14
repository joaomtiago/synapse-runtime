#ifndef SYNAPSE_P4RUNTIME_STREAM_BUFFER_UPDATE_HPP_
#define SYNAPSE_P4RUNTIME_STREAM_BUFFER_UPDATE_HPP_

#ifdef __cplusplus
#include <queue>
#endif  // __cplusplus

#include "synapse/protobuf/p4runtime_info_wrapper.h"
#include "synapse/protobuf/p4runtime_wrapper.h"
#include "synapse/utils/typedefs_wrapper.h"

#ifdef __cplusplus
namespace synapse::p4runtime {

class UpdateBuffer {
 public:
  UpdateBuffer(const helper_ptr_t& helper);

  bool buffer(p4_update_t* update);

  p4_write_request_t* flush();

 private:
  std::queue<p4_update_t*> buffer_;
  helper_ptr_t helper_;
};

typedef UpdateBuffer upd_buff_t;
typedef upd_buff_t* upd_buff_ptr_t;

}  // namespace synapse::p4runtime
#endif  // __cplusplus

#endif  // SYNAPSE_P4RUNTIME_STREAM_BUFFER_UPDATE_HPP_