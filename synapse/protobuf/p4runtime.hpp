#ifndef SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_HPP_
#define SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_HPP_

#include "synapse/utils/typedefs.hpp"

#ifdef __cplusplus
namespace synapse::p4runtime {

class P4RuntimeHelper {
 public:
  p4_write_request_t *writeRequest(uint64_t deviceId,
                                   std::vector<p4_update_t *> *updates,
                                   p4_write_request_atomicity_t atomicity,
                                   uint64_t electionIdLow,
                                   uint64_t electionIdHigh);

  p4_read_request_t *readRequest(uint64_t deviceId, const std::string &role,
                                 std::vector<p4_entity_t *> *entities);

  p4_update_t *update(p4_update_type_t type, p4_entity_t *entity);

  // todo
  p4_entity_t *entity(p4::v1::ExternEntry *entry);

  p4_entity_t *entity(p4::v1::TableEntry *entry);

  // todo
  p4_entity_t *entity(p4::v1::ActionProfileMember *entry);

  // todo
  p4_entity_t *entity(p4::v1::ActionProfileGroup *entry);

  // todo
  p4_entity_t *entity(p4::v1::MeterEntry *entry);

  // todo
  p4_entity_t *entity(p4::v1::DirectMeterEntry *entry);

  // todo
  p4_entity_t *entity(p4::v1::CounterEntry *entry);

  // todo
  p4_entity_t *entity(p4::v1::DirectCounterEntry *entry);

  p4_entity_t *entity(p4_packet_replication_engine_entry_t *entry);

  // todo
  p4_entity_t *entity(p4::v1::ValueSetEntry *entry);

  // todo
  p4_entity_t *entity(p4::v1::RegisterEntry *entry);

  // todo
  p4_entity_t *entity(p4::v1::DigestEntry *entry);

  // todo
  p4::v1::ExternEntry *externEntry();

  p4_table_entry_t *tableEntry(
      uint32_t tableId, std::vector<p4_field_match_t *> *match,
      bool isDefaultAction, p4_table_action_t *action = nullptr,
      uint64_t idleTimeoutNs = 0, int32_t priority = -1,
      p4::v1::MeterConfig *meterConfig = nullptr,
      p4::v1::CounterData *counterData = nullptr,
      p4::v1::TableEntry_IdleTimeout *timeSinceLastHit = nullptr,
      const std::string &metadata = nullptr);

  p4_field_match_exact_t *fieldMatchExact(const std::string &value);

  p4_field_match_t *fieldMatch(uint32_t fieldId,
                               p4_field_match_exact_t *fieldMatchType);

  // todo
  p4_field_match_t *fieldMatch(uint32_t fieldId,
                               p4::v1::FieldMatch_Ternary *fieldMatchType);

  // todo
  p4_field_match_t *fieldMatch(uint32_t fieldId,
                               p4::v1::FieldMatch_LPM *fieldMatchType);

  // todo
  p4_field_match_t *fieldMatch(uint32_t fieldId,
                               p4::v1::FieldMatch_Range *fieldMatchType);

  p4_field_match_t *fieldMatch(uint32_t fieldId,
                               p4::v1::FieldMatch_Optional *fieldMatchType);

  // todo
  p4_field_match_t *fieldMatch(uint32_t fieldId,
                               google::protobuf::Any *fieldMatchType);

  p4_table_action_t *tableAction(p4_action_t *type);

  p4_table_action_t *tableAction(p4::v1::ActionProfileActionSet *type);

  p4_action_t *action(uint32_t actionId,
                      std::vector<p4_action_param_t *> *params);

  p4_action_param_t *actionParam(uint32_t paramId, const std::string &value);

  // todo
  p4::v1::ActionProfileActionSet *actionProfileActionSet();

  // todo
  p4::v1::ActionProfileAction *ActionProfileAction();

  // todo
  p4::v1::ActionProfileMember *actionProfileMember();

  // todo
  p4::v1::ActionProfileGroup *actionProfileGroup();

  // todo
  p4::v1::Index *index();

  // todo
  p4::v1::MeterEntry *meterEntry();

  // todo
  p4::v1::DirectMeterEntry *directMeterEntry();

  // todo
  p4::v1::MeterConfig *meterConfig();

  // todo
  p4::v1::CounterEntry *counterEntry();

  // todo
  p4::v1::DirectCounterEntry *directCounterEntry();

  // todo
  p4::v1::CounterData *counterData();

  p4_packet_replication_engine_entry_t *packetReplicationEngineEntry(
      p4_multicast_group_entry_t *type);

  p4_packet_replication_engine_entry_t *packetReplicationEngineEntry(
      p4_clone_session_entry_t *type);

  p4_replica_t *replica(uint32_t egressPort, uint32_t instance);

  p4_multicast_group_entry_t *multicastGroupEntry(
      uint32_t multicastGroupId, std::vector<p4_replica_t *> *replicas);

  // todo
  p4_clone_session_entry_t *cloneSessionEntry();

  // todo
  p4::v1::ValueSetMember *valueSetMember();

  // todo
  p4::v1::ValueSetEntry *valueSetEntry();

  // todo
  p4::v1::RegisterEntry *registerEntry();

  // todo
  p4::v1::DigestEntry *digestEntry();

  p4::v1::StreamMessageRequest *streamMessageRequest(
      p4_master_arbitration_update_t *update);

  p4::v1::StreamMessageRequest *streamMessageRequest(p4::v1::PacketOut *update);

  p4::v1::StreamMessageRequest *streamMessageRequest(
      p4::v1::DigestListAck *update);

  p4::v1::StreamMessageRequest *streamMessageRequest(
      google::protobuf::Any *update);

  p4::v1::PacketOut *packetOut(const std::string &payload,
                               std::vector<p4_packet_metadata_t *> *metadata);

  // todo
  p4::v1::DigestListAck *digestListAck();

  // todo
  p4_stream_message_response_t *streamMessageResponse();

  // todo
  p4::v1::PacketIn *packetIn();

  // todo
  p4::v1::DigestList *digestList();

  p4_packet_metadata_t *packetMetadata(uint32_t metadataId,
                                       const std::string &value);

  p4_master_arbitration_update_t *masterArbitrationUpdate(
      uint64_t deviceId, uint64_t electionIdLow, uint64_t electionIdHigh);

  // todo
  p4::v1::Role *role();

  // todo
  p4_idle_timeout_notification_t *idleTimeoutNotification();

  // todo include role
  p4_set_forwarding_pipeline_config_request_t *
  setForwardingPipelineConfigRequest(
      uint64_t deviceId, uint64_t electionIdLow, uint64_t electionIdHigh,
      p4_set_forwarding_pipeline_config_request_action_t action,
      p4_forwarding_pipeline_config_t *config);

  p4_forwarding_pipeline_config_t *forwardingPipelineConfig(
      p4::config::v1::P4Info *p4Info, const std::string &p4DeviceConfig);
};

typedef P4RuntimeHelper helper_t;
typedef helper_t *helper_ptr_t;

}  // namespace synapse::p4runtime
#endif  // __cplusplus

#endif  // SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_HPP_
