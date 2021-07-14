#include "p4runtime_wrapper.h"

namespace synapse::p4runtime {

p4_replica_t *synapse_p4runtime_helper_replica(helper_ptr_t helper,
                                               int egress_port, int instance) {
  return helper->replica(static_cast<uint32_t>(egress_port),
                         static_cast<uint32_t>(instance));
}

p4_multicast_group_entry_t *synapse_p4runtime_helper_multicast_group_entry(
    helper_ptr_t helper, int multicast_group_id, p4_replica_t **replicas,
    size_t replicas_card) {
  return helper->multicastGroupEntry(
      static_cast<uint32_t>(multicast_group_id),
      new std::vector<p4_replica_t *>(replicas, replicas + replicas_card));
}

p4_packet_replication_engine_entry_t *
synapse_p4runtime_helper_packet_replication_engine_entry(
    helper_ptr_t helper, p4_multicast_group_entry_t *type) {
  return helper->packetReplicationEngineEntry(type);
}

p4_entity_t *synapse_p4runtime_helper_entity(
    helper_ptr_t helper, p4_packet_replication_engine_entry_t *entry) {
  return helper->entity(entry);
}

p4_update_t *synapse_p4runtime_helper_update_INSERT(helper_ptr_t helper,
                                                    p4_entity_t *entity) {
  return helper->update(p4_update_type_t::Update_Type_INSERT, entity);
}

}  // namespace synapse::p4runtime
