#ifndef SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_WRAPPER_H_
#define SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_WRAPPER_H_

#include "p4runtime.hpp"

#ifndef __cplusplus
typedef void *helper_ptr_t;
#endif  // __cplusplus

#ifdef __cplusplus
namespace synapse::p4runtime {
#endif  // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

p4_replica_t *synapse_p4runtime_helper_replica(helper_ptr_t helper,
                                               int egress_port, int instance);

p4_multicast_group_entry_t *synapse_p4runtime_helper_multicast_group_entry(
    helper_ptr_t helper, int multicast_group_id, p4_replica_t **replicas,
    size_t replicas_size);

p4_packet_replication_engine_entry_t *
synapse_p4runtime_helper_packet_replication_engine_entry(
    helper_ptr_t helper, p4_multicast_group_entry_t *type);

p4_entity_t *synapse_p4runtime_helper_entity(
    helper_ptr_t helper, p4_packet_replication_engine_entry_t *entry);

p4_update_t *synapse_p4runtime_helper_update_INSERT(helper_ptr_t helper,
                                                    p4_entity_t *entity);

#ifdef __cplusplus
}
#endif  // __cplusplus

#ifdef __cplusplus
}  // namespace synapse::p4runtime
#endif  // __cplusplus

#endif  // SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_WRAPPER_H_