#include "p4runtime.hpp"

#include <stdexcept>

#include "p4/v1/p4runtime.pb.h"

namespace synapse::p4runtime {

p4::v1::WriteRequest *P4RuntimeHelper::writeRequest(
    uint64_t deviceId, std::vector<p4::v1::Update *> *updates,
    p4::v1::WriteRequest_Atomicity atomicity, uint64_t electionIdLow,
    uint64_t electionIdHigh) {
  auto obj = new p4::v1::WriteRequest();

  obj->set_device_id(deviceId);

  for (auto update : *updates) {
    obj->mutable_updates()->AddAllocated(update);
  }

  obj->set_atomicity(atomicity);
  obj->mutable_election_id()->set_low(electionIdLow);
  obj->mutable_election_id()->set_high(electionIdHigh);

  return obj;
}

p4::v1::ReadRequest *P4RuntimeHelper::readRequest(
    uint64_t deviceId, const std::string &role,
    std::vector<p4::v1::Entity *> *entities) {
  auto obj = new p4::v1::ReadRequest();

  obj->set_device_id(deviceId);
  obj->set_role(role);

  for (auto entity : *entities) {
    obj->mutable_entities()->AddAllocated(entity);
  }

  return obj;
}

p4::v1::Update *P4RuntimeHelper::update(p4::v1::Update_Type type,
                                        p4::v1::Entity *entity) {
  auto obj = new p4::v1::Update();

  obj->set_type(type);
  obj->set_allocated_entity(entity);

  return obj;
}

p4::v1::Entity *P4RuntimeHelper::entity(p4::v1::TableEntry *entry) {
  auto obj = new p4::v1::Entity();
  obj->set_allocated_table_entry(entry);
  return obj;
}

p4::v1::Entity *P4RuntimeHelper::entity(
    p4::v1::PacketReplicationEngineEntry *entry) {
  auto obj = new p4::v1::Entity();
  obj->set_allocated_packet_replication_engine_entry(entry);
  return obj;
}

p4::v1::TableEntry *P4RuntimeHelper::tableEntry(
    uint32_t tableId, std::vector<p4::v1::FieldMatch *> *match,
    bool isDefaultAction, p4::v1::TableAction *action /*=nullptr*/,
    uint64_t idleTimeoutNs /*=0*/, int32_t priority /*=-1*/,
    p4::v1::MeterConfig *meterConfig /*=nullptr*/,
    p4::v1::CounterData *counterData /*=nullptr*/,
    p4::v1::TableEntry_IdleTimeout *timeSinceLastHit /*=nullptr*/,
    const std::string &metadata /*=nullptr*/) {
  auto obj = new p4::v1::TableEntry();

  obj->set_table_id(tableId);

  if (match != nullptr) {
    for (auto fieldMatch : *match) {
      obj->mutable_match()->AddAllocated(fieldMatch);
    }
  }

  if (isDefaultAction && action != nullptr) {
    throw std::invalid_argument(
        "No action should be provided if `isDefaultAction` is set to true");
  }

  if (!isDefaultAction && action == nullptr) {
    throw std::invalid_argument(
        "An action should be provided if `isDefaultAction` is set to false");
  }

  obj->set_is_default_action(isDefaultAction);
  obj->set_allocated_action(action);

  if (idleTimeoutNs > 0) {
    obj->set_idle_timeout_ns(idleTimeoutNs);
  }

  if (priority > -1) {
    obj->set_priority(priority);
  }

  if (meterConfig != nullptr) {
    obj->set_allocated_meter_config(meterConfig);
  }

  if (counterData != nullptr) {
    obj->set_allocated_counter_data(counterData);
  }

  if (timeSinceLastHit != nullptr) {
    obj->set_allocated_time_since_last_hit(timeSinceLastHit);
  }

  obj->set_metadata(metadata);
  return obj;
}

p4::v1::FieldMatch_Exact *P4RuntimeHelper::fieldMatchExact(
    const std::string &value) {
  auto obj = new p4::v1::FieldMatch_Exact();
  obj->set_value(value);
  return obj;
}

p4::v1::FieldMatch *P4RuntimeHelper::fieldMatch(
    uint32_t fieldId, p4::v1::FieldMatch_Exact *fieldMatchType) {
  auto obj = new p4::v1::FieldMatch();

  // todo refactor this ("set_field_id" is common to all ops)
  obj->set_field_id(fieldId);
  obj->set_allocated_exact(fieldMatchType);

  return obj;
}

p4::v1::TableAction *P4RuntimeHelper::tableAction(p4::v1::Action *type) {
  auto obj = new p4::v1::TableAction();
  obj->set_allocated_action(type);
  return obj;
}

p4::v1::TableAction *P4RuntimeHelper::tableAction(
    p4::v1::ActionProfileActionSet *type) {
  auto obj = new p4::v1::TableAction();
  obj->set_allocated_action_profile_action_set(type);
  return obj;
}

p4::v1::Action *P4RuntimeHelper::action(
    uint32_t actionId, std::vector<p4::v1::Action_Param *> *params) {
  auto obj = new p4::v1::Action();

  obj->set_action_id(actionId);

  for (auto actionParam : *params) {
    obj->mutable_params()->AddAllocated(actionParam);
  }

  return obj;
}

p4::v1::Action_Param *P4RuntimeHelper::actionParam(uint32_t paramId,
                                                   const std::string &value) {
  auto obj = new p4::v1::Action_Param();

  obj->set_param_id(paramId);
  obj->set_value(value);

  return obj;
}

p4::v1::PacketReplicationEngineEntry *
P4RuntimeHelper::packetReplicationEngineEntry(
    p4::v1::MulticastGroupEntry *type) {
  auto obj = new p4::v1::PacketReplicationEngineEntry();
  obj->set_allocated_multicast_group_entry(type);
  return obj;
}

p4::v1::PacketReplicationEngineEntry *
P4RuntimeHelper::packetReplicationEngineEntry(p4::v1::CloneSessionEntry *type) {
  auto obj = new p4::v1::PacketReplicationEngineEntry();
  obj->set_allocated_clone_session_entry(type);
  return obj;
}

p4::v1::Replica *P4RuntimeHelper::replica(uint32_t egressPort,
                                          uint32_t instance) {
  auto obj = new p4::v1::Replica();

  obj->set_egress_port(egressPort);
  obj->set_instance(instance);

  return obj;
}

p4::v1::MulticastGroupEntry *P4RuntimeHelper::multicastGroupEntry(
    uint32_t multicastGroupId, std::vector<p4::v1::Replica *> *replicas) {
  auto obj = new p4::v1::MulticastGroupEntry();

  obj->set_multicast_group_id(multicastGroupId);

  if (replicas != nullptr) {
    for (auto replica : *replicas) {
      obj->mutable_replicas()->AddAllocated(replica);
    }
  }

  return obj;
}

p4::v1::StreamMessageRequest *P4RuntimeHelper::streamMessageRequest(
    p4::v1::MasterArbitrationUpdate *update) {
  auto obj = new p4::v1::StreamMessageRequest();
  obj->set_allocated_arbitration(update);
  return obj;
}

p4::v1::StreamMessageRequest *P4RuntimeHelper::streamMessageRequest(
    p4::v1::PacketOut *update) {
  auto obj = new p4::v1::StreamMessageRequest();
  obj->set_allocated_packet(update);
  return obj;
}

p4::v1::StreamMessageRequest *P4RuntimeHelper::streamMessageRequest(
    p4::v1::DigestListAck *update) {
  auto obj = new p4::v1::StreamMessageRequest();
  obj->set_allocated_digest_ack(update);
  return obj;
}

p4::v1::StreamMessageRequest *P4RuntimeHelper::streamMessageRequest(
    google::protobuf::Any *update) {
  auto obj = new p4::v1::StreamMessageRequest();
  obj->set_allocated_other(update);
  return obj;
}

p4::v1::PacketOut *P4RuntimeHelper::packetOut(
    const std::string &payload,
    std::vector<p4::v1::PacketMetadata *> *metadata) {
  auto obj = new p4::v1::PacketOut();

  obj->set_payload(payload);

  if (metadata != nullptr) {
    for (auto packetMetadata : *metadata) {
      obj->mutable_metadata()->AddAllocated(packetMetadata);
    }
  }

  return obj;
}

p4::v1::PacketMetadata *P4RuntimeHelper::packetMetadata(
    uint32_t metadataId, const std::string &value) {
  auto obj = new p4::v1::PacketMetadata();

  obj->set_metadata_id(metadataId);
  obj->set_value(value);

  return obj;
}

p4::v1::MasterArbitrationUpdate *P4RuntimeHelper::masterArbitrationUpdate(
    uint64_t deviceId, uint64_t electionIdLow, uint64_t electionIdHigh) {
  auto obj = new p4::v1::MasterArbitrationUpdate();

  obj->set_device_id(deviceId);

  // the role for which the primary client is being arbitrated. for use-cases
  // where multiple roles are not needed, the controller can leave this unset,
  // implying default role and full pipeline access.
  // obj->mutable_role()->CopyFrom(role);

  obj->mutable_election_id()->set_low(electionIdLow);
  obj->mutable_election_id()->set_high(electionIdHigh);

  return obj;
}

p4::v1::SetForwardingPipelineConfigRequest *
P4RuntimeHelper::setForwardingPipelineConfigRequest(
    uint64_t deviceId, uint64_t electionIdLow, uint64_t electionIdHigh,
    p4::v1::SetForwardingPipelineConfigRequest_Action action,
    p4::v1::ForwardingPipelineConfig *config) {
  auto obj = new p4::v1::SetForwardingPipelineConfigRequest();

  obj->set_device_id(deviceId);

  obj->mutable_election_id()->set_low(electionIdLow);
  obj->mutable_election_id()->set_high(electionIdHigh);
  obj->set_action(action);
  obj->set_allocated_config(config);

  return obj;
}

p4::v1::ForwardingPipelineConfig *P4RuntimeHelper::forwardingPipelineConfig(
    p4::config::v1::P4Info *p4Info, const std::string &p4DeviceConfig) {
  auto obj = new p4::v1::ForwardingPipelineConfig();

  obj->set_allocated_p4info(p4Info);
  obj->set_p4_device_config(p4DeviceConfig);

  /**
   * Metadata (cookie) opaque to the target. A control plane may use this field
   * to uniquely identify this config. There are no restrictions on how such
   * value is computed, or where this is stored on the target, as long as it is
   * returned with a GetForwardingPipelineConfig RPC. When reading the cookie,
   * we need to distinguish those cases where a cookie is NOT present (e.g. not
   * set in the SetForwardingPipelineConfigRequest, therefore we wrap the actual
   * uint64 value in a protobuf message.
   */

  return obj;
}

}  // namespace synapse::p4runtime