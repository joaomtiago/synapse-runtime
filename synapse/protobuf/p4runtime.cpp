#include <stdexcept>

#include "p4/v1/p4runtime.pb.h"
#include "p4runtime_wrapper.h"

namespace synapse::p4runtime {

p4_write_request_t *P4RuntimeHelper::writeRequest(
    uint64_t deviceId, std::vector<p4_update_t *> *updates,
    p4_write_request_atomicity_t atomicity, uint64_t electionIdLow,
    uint64_t electionIdHigh) {
  auto obj = new p4_write_request_t();

  obj->set_device_id(deviceId);

  for (auto update : *updates) {
    obj->mutable_updates()->AddAllocated(update);
  }

  obj->set_atomicity(atomicity);
  obj->mutable_election_id()->set_low(electionIdLow);
  obj->mutable_election_id()->set_high(electionIdHigh);

  return obj;
}

p4_read_request_t *P4RuntimeHelper::readRequest(
    uint64_t deviceId, const std::string &role,
    std::vector<p4_entity_t *> *entities) {
  auto obj = new p4_read_request_t();

  obj->set_device_id(deviceId);
  obj->set_role(role);

  for (auto entity : *entities) {
    obj->mutable_entities()->AddAllocated(entity);
  }

  return obj;
}

p4_update_t *P4RuntimeHelper::update(p4_update_type_t type,
                                     p4_entity_t *entity) {
  auto obj = new p4_update_t();

  obj->set_type(type);
  obj->set_allocated_entity(entity);

  return obj;
}

p4_entity_t *P4RuntimeHelper::entity(p4_table_entry_t *entry) {
  auto obj = new p4_entity_t();
  obj->set_allocated_table_entry(entry);
  return obj;
}

p4_entity_t *P4RuntimeHelper::entity(
    p4_packet_replication_engine_entry_t *entry) {
  auto obj = new p4_entity_t();
  obj->set_allocated_packet_replication_engine_entry(entry);
  return obj;
}

p4_table_entry_t *P4RuntimeHelper::tableEntry(
    uint32_t tableId, std::vector<p4_field_match_t *> *match,
    bool isDefaultAction, p4_table_action_t *action /*=nullptr*/,
    uint64_t idleTimeoutNs /*=0*/, int32_t priority /*=-1*/,
    p4::v1::MeterConfig *meterConfig /*=nullptr*/,
    p4::v1::CounterData *counterData /*=nullptr*/,
    p4::v1::TableEntry_IdleTimeout *timeSinceLastHit /*=nullptr*/,
    const std::string &metadata /*=nullptr*/) {
  auto obj = new p4_table_entry_t();

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

p4_field_match_exact_t *P4RuntimeHelper::fieldMatchExact(
    const std::string &value) {
  auto obj = new p4_field_match_exact_t();
  obj->set_value(value);
  return obj;
}

p4_field_match_t *P4RuntimeHelper::fieldMatch(
    uint32_t fieldId, p4_field_match_exact_t *fieldMatchType) {
  auto obj = new p4_field_match_t();

  // todo refactor this ("set_field_id" is common to all ops)
  obj->set_field_id(fieldId);
  obj->set_allocated_exact(fieldMatchType);

  return obj;
}

p4_table_action_t *P4RuntimeHelper::tableAction(p4_action_t *type) {
  auto obj = new p4_table_action_t();
  obj->set_allocated_action(type);
  return obj;
}

p4_table_action_t *P4RuntimeHelper::tableAction(
    p4::v1::ActionProfileActionSet *type) {
  auto obj = new p4_table_action_t();
  obj->set_allocated_action_profile_action_set(type);
  return obj;
}

p4_action_t *P4RuntimeHelper::action(uint32_t actionId,
                                     std::vector<p4_action_param_t *> *params) {
  auto obj = new p4_action_t();

  obj->set_action_id(actionId);

  for (auto actionParam : *params) {
    obj->mutable_params()->AddAllocated(actionParam);
  }

  return obj;
}

p4_action_param_t *P4RuntimeHelper::actionParam(uint32_t paramId,
                                                const std::string &value) {
  auto obj = new p4_action_param_t();

  obj->set_param_id(paramId);
  obj->set_value(value);

  return obj;
}

p4_packet_replication_engine_entry_t *
P4RuntimeHelper::packetReplicationEngineEntry(
    p4_multicast_group_entry_t *type) {
  auto obj = new p4_packet_replication_engine_entry_t();
  obj->set_allocated_multicast_group_entry(type);
  return obj;
}

p4_packet_replication_engine_entry_t *
P4RuntimeHelper::packetReplicationEngineEntry(p4_clone_session_entry_t *type) {
  auto obj = new p4_packet_replication_engine_entry_t();
  obj->set_allocated_clone_session_entry(type);
  return obj;
}

p4_replica_t *P4RuntimeHelper::replica(uint32_t egressPort, uint32_t instance) {
  auto obj = new p4_replica_t();

  obj->set_egress_port(egressPort);
  obj->set_instance(instance);

  return obj;
}

p4_multicast_group_entry_t *P4RuntimeHelper::multicastGroupEntry(
    uint32_t multicastGroupId, std::vector<p4_replica_t *> *replicas) {
  auto obj = new p4_multicast_group_entry_t();

  obj->set_multicast_group_id(multicastGroupId);

  if (replicas != nullptr) {
    for (auto replica : *replicas) {
      obj->mutable_replicas()->AddAllocated(replica);
    }
  }

  return obj;
}

p4_stream_message_request_t *P4RuntimeHelper::streamMessageRequest(
    p4_master_arbitration_update_t *update) {
  auto obj = new p4_stream_message_request_t();
  obj->set_allocated_arbitration(update);
  return obj;
}

p4_stream_message_request_t *P4RuntimeHelper::streamMessageRequest(
    p4_packet_out_t *update) {
  auto obj = new p4_stream_message_request_t();
  obj->set_allocated_packet(update);
  return obj;
}

p4_stream_message_request_t *P4RuntimeHelper::streamMessageRequest(
    p4::v1::DigestListAck *update) {
  auto obj = new p4_stream_message_request_t();
  obj->set_allocated_digest_ack(update);
  return obj;
}

p4_stream_message_request_t *P4RuntimeHelper::streamMessageRequest(
    google::protobuf::Any *update) {
  auto obj = new p4_stream_message_request_t();
  obj->set_allocated_other(update);
  return obj;
}

p4_packet_out_t *P4RuntimeHelper::packetOut(
    const std::string &payload, std::vector<p4_packet_metadata_t *> *metadata) {
  auto obj = new p4_packet_out_t();

  obj->set_payload(payload);

  if (metadata != nullptr) {
    for (auto packetMetadata : *metadata) {
      obj->mutable_metadata()->AddAllocated(packetMetadata);
    }
  }

  return obj;
}

p4_packet_metadata_t *P4RuntimeHelper::packetMetadata(
    uint32_t metadataId, const std::string &value) {
  auto obj = new p4_packet_metadata_t();

  obj->set_metadata_id(metadataId);
  obj->set_value(value);

  return obj;
}

p4_master_arbitration_update_t *P4RuntimeHelper::masterArbitrationUpdate(
    uint64_t deviceId, uint64_t electionIdLow, uint64_t electionIdHigh) {
  auto obj = new p4_master_arbitration_update_t();

  obj->set_device_id(deviceId);

  // the role for which the primary client is being arbitrated. for use-cases
  // where multiple roles are not needed, the controller can leave this unset,
  // implying default role and full pipeline access.
  // obj->mutable_role()->CopyFrom(role);

  obj->mutable_election_id()->set_low(electionIdLow);
  obj->mutable_election_id()->set_high(electionIdHigh);

  return obj;
}

p4_set_forwarding_pipeline_config_request_t *
P4RuntimeHelper::setForwardingPipelineConfigRequest(
    uint64_t deviceId, uint64_t electionIdLow, uint64_t electionIdHigh,
    p4_set_forwarding_pipeline_config_request_action_t action,
    p4_forwarding_pipeline_config_t *config) {
  auto obj = new p4_set_forwarding_pipeline_config_request_t();

  obj->set_device_id(deviceId);

  obj->mutable_election_id()->set_low(electionIdLow);
  obj->mutable_election_id()->set_high(electionIdHigh);
  obj->set_action(action);
  obj->set_allocated_config(config);

  return obj;
}

p4_forwarding_pipeline_config_t *P4RuntimeHelper::forwardingPipelineConfig(
    p4::config::v1::P4Info *p4Info, const std::string &p4DeviceConfig) {
  auto obj = new p4_forwarding_pipeline_config_t();

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