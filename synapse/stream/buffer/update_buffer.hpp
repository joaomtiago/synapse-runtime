#ifndef SYNAPSE_P4RUNTIME_STREAM_BUFFER_UPDATE_HPP_
#define SYNAPSE_P4RUNTIME_STREAM_BUFFER_UPDATE_HPP_

#include <queue>

#include "synapse/utils/typedefs.hpp"

namespace synapse::p4runtime {

class UpdateBuffer {
 public:
  UpdateBuffer(const helper_ptr_t& helper);

  void buffer(p4::v1::Update* update);

  p4::v1::WriteRequest* flush();

 private:
  std::queue<p4::v1::Update*> buffer_;
  helper_ptr_t helper_;
};

typedef UpdateBuffer upd_buff_t;
typedef upd_buff_t* upd_buff_ptr_t;

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_STREAM_BUFFER_UPDATE_HPP_