#ifndef SYNAPSE_P4RUNTIME_STREAM_BUFFER_UPDATE_BUFFER_WRAPPER_H_
#define SYNAPSE_P4RUNTIME_STREAM_BUFFER_UPDATE_BUFFER_WRAPPER_H_

#include "update_buffer.hpp"

#ifndef __cplusplus
typedef void* upd_buff_ptr_t;
#endif  // __cplusplus

#ifdef __cplusplus
namespace synapse::p4runtime {
#endif  // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

bool synapse_update_buffer_buffer(upd_buff_ptr_t update_buffer,
                                  p4_update_t* update);

p4_write_request_t* synapse_update_buffer_flush(upd_buff_ptr_t update_buffer);

#ifdef __cplusplus
}  // namespace synapse::p4runtime
#endif  // __cplusplus

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // SYNAPSE_P4RUNTIME_STREAM_BUFFER_UPDATE_BUFFER_WRAPPER_H_
