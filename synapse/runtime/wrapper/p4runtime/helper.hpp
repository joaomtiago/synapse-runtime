#ifndef SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_HELPER_HPP_
#define SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_HELPER_HPP_

#include "synapse/runtime/p4runtime/helper.hpp"

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * P4Info helpers go below.
 */

/**
 * P4Runtime helpers go below.
 */

p4_entity_t *synapse_runtime_runtime_helper_p4_entity(
    helper_ptr_t helper, p4_packet_replication_engine_entry_t *entry);

p4_multicast_group_entry_t *
synapse_runtime_runtime_helper_p4_multicast_group_entry(helper_ptr_t helper,
                                                        int multicast_group_id,
                                                        p4_replica_t **replicas,
                                                        int replicas_card);

p4_packet_replication_engine_entry_t *
synapse_runtime_runtime_helper_p4_packet_replication_engine_entry(
    helper_ptr_t helper, p4_multicast_group_entry_t *type);

p4_replica_t *synapse_runtime_runtime_helper_p4_replica(helper_ptr_t helper,
                                                        int egress_port,
                                                        int instance);

p4_update_t *
synapse_runtime_runtime_helper_p4_update_INSERT(helper_ptr_t helper,
                                                p4_entity_t *entity);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_HELPER_HPP_