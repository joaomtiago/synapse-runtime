#include "synapse/runtime/wrapper/p4runtime/helper.hpp"

namespace synapse::runtime {

p4_entity_t *synapse_runtime_runtime_helper_p4_entity(
    helper_ptr_t helper, p4_packet_replication_engine_entry_t *entry) {
  return helper->p4Entity(entry);
}

p4_multicast_group_entry_t *
synapse_runtime_runtime_helper_p4_multicast_group_entry(helper_ptr_t helper,
                                                        int multicast_group_id,
                                                        p4_replica_t **replicas,
                                                        int replicas_card) {
  return helper->p4MulticastGroupEntry(
      static_cast<uint32_t>(multicast_group_id),
      new std::vector<p4_replica_t *>(replicas, replicas + replicas_card));
}

p4_packet_replication_engine_entry_t *
synapse_runtime_runtime_helper_p4_packet_replication_engine_entry(
    helper_ptr_t helper, p4_multicast_group_entry_t *type) {
  return helper->p4PacketReplicationEngineEntry(type);
}

p4_replica_t *synapse_runtime_runtime_helper_p4_replica(helper_ptr_t helper,
                                                        int egress_port,
                                                        int instance) {
  return helper->p4Replica(static_cast<uint32_t>(egress_port),
                           static_cast<uint32_t>(instance));
}

p4_update_t *
synapse_runtime_runtime_helper_p4_update_INSERT(helper_ptr_t helper,
                                                p4_entity_t *entity) {
  return helper->p4Update(p4_update_type_t::Update_Type_INSERT, entity);
}

} // namespace synapse::runtime
