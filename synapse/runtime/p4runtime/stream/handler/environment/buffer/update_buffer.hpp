#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_BUFFER_UPDATE_BUFFER_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_BUFFER_UPDATE_BUFFER_HPP_

#ifdef __cplusplus
#include <queue>
#endif // __cplusplus

#include "synapse/runtime/wrapper/p4runtime/helper.hpp"

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
class UpdateBuffer {
public:
  UpdateBuffer(const helper_ptr_t &helper);

  bool buffer(p4_update_t *update);

  p4_write_request_t *flush();

private:
  std::queue<p4_update_t *> buffer_;
  helper_ptr_t helper_;
};
#endif // __cplusplus

#ifdef __cplusplus
typedef UpdateBuffer upd_buff_t;
typedef upd_buff_t *upd_buff_ptr_t;
#else
struct upd_buff;
typedef struct upd_buff upd_buff_t;
typedef void *upd_buff_ptr_t;
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_ENVIRONMENT_BUFFER_UPDATE_BUFFER_HPP_