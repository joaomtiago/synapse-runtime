#ifndef SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_HPP_
#define SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_HPP_

#include "p4/v1/p4runtime.pb.h"

namespace synapse::p4runtime {

class P4RuntimeHelper {
 public:
  P4RuntimeHelper() {}

  p4::v1::WriteRequest *writeRequest(uint64_t deviceId,
                                     std::vector<p4::v1::Update *> *updates,
                                     p4::v1::WriteRequest_Atomicity atomicity,
                                     uint64_t electionIdLow,
                                     uint64_t electionIdHigh);

  p4::v1::ReadRequest *readRequest(uint64_t deviceId, const std::string &role,
                                   std::vector<p4::v1::Entity *> *entities);

  p4::v1::Update *update(p4::v1::Update_Type type, p4::v1::Entity *entity);

  // todo
  p4::v1::Entity *entity(p4::v1::ExternEntry *entry);

  p4::v1::Entity *entity(p4::v1::TableEntry *entry);

  // todo
  p4::v1::Entity *entity(p4::v1::ActionProfileMember *entry);

  // todo
  p4::v1::Entity *entity(p4::v1::ActionProfileGroup *entry);

  // todo
  p4::v1::Entity *entity(p4::v1::MeterEntry *entry);

  // todo
  p4::v1::Entity *entity(p4::v1::DirectMeterEntry *entry);

  // todo
  p4::v1::Entity *entity(p4::v1::CounterEntry *entry);

  // todo
  p4::v1::Entity *entity(p4::v1::DirectCounterEntry *entry);

  p4::v1::Entity *entity(p4::v1::PacketReplicationEngineEntry *entry);

  // todo
  p4::v1::Entity *entity(p4::v1::ValueSetEntry *entry);

  // todo
  p4::v1::Entity *entity(p4::v1::RegisterEntry *entry);

  // todo
  p4::v1::Entity *entity(p4::v1::DigestEntry *entry);

  // todo
  p4::v1::ExternEntry *externEntry();

  p4::v1::TableEntry *tableEntry(
      uint32_t tableId, std::vector<p4::v1::FieldMatch *> *match,
      bool isDefaultAction, p4::v1::TableAction *action = nullptr,
      uint64_t idleTimeoutNs = 0, int32_t priority = -1,
      p4::v1::MeterConfig *meterConfig = nullptr,
      p4::v1::CounterData *counterData = nullptr,
      p4::v1::TableEntry_IdleTimeout *timeSinceLastHit = nullptr,
      const std::string &metadata = nullptr);

  p4::v1::FieldMatch_Exact *fieldMatchExact(const std::string &value);

  p4::v1::FieldMatch *fieldMatch(uint32_t fieldId,
                                 p4::v1::FieldMatch_Exact *fieldMatchType);

  // todo
  p4::v1::FieldMatch *fieldMatch(uint32_t fieldId,
                                 p4::v1::FieldMatch_Ternary *fieldMatchType);

  // todo
  p4::v1::FieldMatch *fieldMatch(uint32_t fieldId,
                                 p4::v1::FieldMatch_LPM *fieldMatchType);

  // todo
  p4::v1::FieldMatch *fieldMatch(uint32_t fieldId,
                                 p4::v1::FieldMatch_Range *fieldMatchType);

  p4::v1::FieldMatch *fieldMatch(uint32_t fieldId,
                                 p4::v1::FieldMatch_Optional *fieldMatchType);

  // todo
  p4::v1::FieldMatch *fieldMatch(uint32_t fieldId,
                                 google::protobuf::Any *fieldMatchType);

  p4::v1::TableAction *tableAction(p4::v1::Action *type);

  p4::v1::TableAction *tableAction(p4::v1::ActionProfileActionSet *type);

  p4::v1::Action *action(uint32_t actionId,
                         std::vector<p4::v1::Action_Param *> *params);

  p4::v1::Action_Param *actionParam(uint32_t paramId, const std::string &value);

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

  p4::v1::PacketReplicationEngineEntry *packetReplicationEngineEntry(
      p4::v1::MulticastGroupEntry *type);

  p4::v1::PacketReplicationEngineEntry *packetReplicationEngineEntry(
      p4::v1::CloneSessionEntry *type);

  p4::v1::Replica *replica(uint32_t egressPort, uint32_t instance);

  p4::v1::MulticastGroupEntry *multicastGroupEntry(
      uint32_t multicastGroupId, std::vector<p4::v1::Replica *> *replicas);

  // todo
  p4::v1::CloneSessionEntry *cloneSessionEntry();

  // todo
  p4::v1::ValueSetMember *valueSetMember();

  // todo
  p4::v1::ValueSetEntry *valueSetEntry();

  // todo
  p4::v1::RegisterEntry *registerEntry();

  // todo
  p4::v1::DigestEntry *digestEntry();

  p4::v1::StreamMessageRequest *streamMessageRequest(
      p4::v1::MasterArbitrationUpdate *update);

  p4::v1::StreamMessageRequest *streamMessageRequest(p4::v1::PacketOut *update);

  p4::v1::StreamMessageRequest *streamMessageRequest(
      p4::v1::DigestListAck *update);

  p4::v1::StreamMessageRequest *streamMessageRequest(
      google::protobuf::Any *update);

  p4::v1::PacketOut *packetOut(const std::string &payload,
                               std::vector<p4::v1::PacketMetadata *> *metadata);

  // todo
  p4::v1::DigestListAck *digestListAck();

  // todo
  p4::v1::StreamMessageResponse *streamMessageResponse();

  // todo
  p4::v1::PacketIn *packetIn();

  // todo
  p4::v1::DigestList *digestList();

  p4::v1::PacketMetadata *packetMetadata(uint32_t metadataId,
                                         const std::string &value);

  p4::v1::MasterArbitrationUpdate *masterArbitrationUpdate(
      uint64_t deviceId, uint64_t electionIdLow, uint64_t electionIdHigh);

  // todo
  p4::v1::Role *role();

  // todo
  p4::v1::IdleTimeoutNotification *idleTimeoutNotification();

  // todo include role
  p4::v1::SetForwardingPipelineConfigRequest *
  setForwardingPipelineConfigRequest(
      uint64_t deviceId, uint64_t electionIdLow, uint64_t electionIdHigh,
      p4::v1::SetForwardingPipelineConfigRequest_Action action,
      p4::v1::ForwardingPipelineConfig *config);

  p4::v1::ForwardingPipelineConfig *forwardingPipelineConfig(
      p4::config::v1::P4Info *p4Info, const std::string &p4DeviceConfig);
};

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_PROTOBUF_P4RUNTIME_HPP_
