#include "synapse/runtime/p4runtime/helper.hpp"
#include "google/protobuf/text_format.h"
#include "synapse/runtime/utils/file.hpp"
#include "synapse/runtime/utils/runtime_exception.hpp"

namespace synapse::runtime {
RuntimeHelper::RuntimeHelper(const std::string &p4InfoFilepath) {
  std::string _file = readFile(p4InfoFilepath);
  auto _obj = new p4::config::v1::P4Info();

  google::protobuf::TextFormat::MergeFromString(_file, _obj);
  p4Info_.reset(_obj);
}

/**
 * P4Info helpers go below.
 */

p4_info_action_t *RuntimeHelper::p4InfoAction(std::string actionName) {
  for (auto action : p4Info_->actions()) {
    if (action.preamble().name() == actionName) {
      return new p4::config::v1::Action(action);
    }
  }

  throw RuntimeException("Unknown action `" + actionName + "`");
}

p4_info_action_param_t *
RuntimeHelper::p4InfoActionParam(p4_info_action_t action,
                                 std::string paramName) {
  for (auto param : action.params()) {
    if (param.name() == paramName) {
      return new p4::config::v1::Action_Param(param);
    }
  }

  throw RuntimeException("Unknown parameter `" + paramName + "` for action `" +
                         action.preamble().name() + "`");
}

p4_info_controller_packet_metadata_t *
RuntimeHelper::p4InfoControllerPacketMetadata(
    std::string controllerPacketMetadataName) {
  for (auto controllerPacketMetadata : p4Info_->controller_packet_metadata()) {
    if (controllerPacketMetadata.preamble().name() ==
        controllerPacketMetadataName) {
      return new p4::config::v1::ControllerPacketMetadata(
          controllerPacketMetadata);
    }
  }

  throw RuntimeException("Unknown controller packet metadata `" +
                         controllerPacketMetadataName + "`");
}

p4_info_controller_packet_metadata_metadata_t *
RuntimeHelper::p4InfoControllerPacketMetadataMetadata(
    p4_info_controller_packet_metadata_t controllerPacketMetadata,
    std::string metadataName) {
  for (auto metadata : controllerPacketMetadata.metadata()) {
    if (metadata.name() == metadataName) {
      return new p4::config::v1::ControllerPacketMetadata_Metadata(metadata);
    }
  }

  throw RuntimeException("Unknown controller packet metadata metadata `" +
                         metadataName + "`");
}

p4_info_match_field_t *
RuntimeHelper::p4InfoMatchField(p4_info_table_t table,
                                std::string matchFieldName) {
  for (auto matchField : table.match_fields()) {
    if (matchField.name() == matchFieldName) {
      return new p4::config::v1::MatchField(matchField);
    }
  }

  throw RuntimeException("Unknown match field `" + matchFieldName +
                         "` for table `" + table.preamble().name() + "`");
}

p4_info_p4_info_t *RuntimeHelper::p4InfoP4Info() { return p4Info_.get(); }

p4_info_table_t *RuntimeHelper::p4InfoTable(std::string tableName) {
  for (auto table : p4Info_->tables()) {
    if (table.preamble().name() == tableName) {
      return new p4::config::v1::Table(table);
    }
  }

  throw RuntimeException("Unknown table `" + tableName + "`");
}

/**
 * P4Runtime helpers go below.
 */

p4_action_t *RuntimeHelper::p4Action(uint32_t actionId,
                                     std::vector<p4_action_param_t *> *params) {
  auto obj = new p4_action_t();

  obj->set_action_id(actionId);

  for (auto actionParam : *params) {
    obj->mutable_params()->AddAllocated(actionParam);
  }

  return obj;
}

p4_action_param_t *RuntimeHelper::p4ActionParam(uint32_t paramId,
                                                const std::string &value) {
  auto obj = new p4_action_param_t();

  obj->set_param_id(paramId);
  obj->set_value(value);

  return obj;
}

p4_entity_t *RuntimeHelper::p4Entity(p4_table_entry_t *entry) {
  auto obj = new p4_entity_t();
  obj->set_allocated_table_entry(entry);
  return obj;
}

p4_entity_t *
RuntimeHelper::p4Entity(p4_packet_replication_engine_entry_t *entry) {
  auto obj = new p4_entity_t();
  obj->set_allocated_packet_replication_engine_entry(entry);
  return obj;
}

p4_field_match_t *
RuntimeHelper::p4FieldMatch(uint32_t fieldId,
                            p4_field_match_exact_t *fieldMatchType) {
  auto obj = new p4_field_match_t();

  obj->set_field_id(fieldId);
  obj->set_allocated_exact(fieldMatchType);

  return obj;
}

p4_field_match_exact_t *
RuntimeHelper::p4FieldMatchExact(const std::string &value) {
  auto obj = new p4_field_match_exact_t();
  obj->set_value(value);
  return obj;
}

p4_forwarding_pipeline_config_t *
RuntimeHelper::p4ForwardingPipelineConfig(p4_info_p4_info_t *p4Info,
                                          const std::string &p4DeviceConfig) {
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

p4_master_arbitration_update_t *RuntimeHelper::p4MasterArbitrationUpdate(
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

p4_multicast_group_entry_t *
RuntimeHelper::p4MulticastGroupEntry(uint32_t multicastGroupId,
                                     std::vector<p4_replica_t *> *replicas) {
  auto obj = new p4_multicast_group_entry_t();

  obj->set_multicast_group_id(multicastGroupId);

  if (replicas != nullptr) {
    for (auto replica : *replicas) {
      obj->mutable_replicas()->AddAllocated(replica);
    }
  }

  return obj;
}

p4_packet_metadata_t *
RuntimeHelper::p4PacketMetadata(uint32_t metadataId, const std::string &value) {
  auto obj = new p4_packet_metadata_t();

  obj->set_metadata_id(metadataId);
  obj->set_value(value);

  return obj;
}

p4_packet_out_t *
RuntimeHelper::p4PacketOut(const std::string &payload,
                           std::vector<p4_packet_metadata_t *> *metadata) {
  auto obj = new p4_packet_out_t();

  obj->set_payload(payload);

  if (metadata != nullptr) {
    for (auto packetMetadata : *metadata) {
      obj->mutable_metadata()->AddAllocated(packetMetadata);
    }
  }

  return obj;
}

p4_packet_replication_engine_entry_t *
RuntimeHelper::p4PacketReplicationEngineEntry(
    p4_multicast_group_entry_t *type) {
  auto obj = new p4_packet_replication_engine_entry_t();
  obj->set_allocated_multicast_group_entry(type);
  return obj;
}

p4_read_request_t *
RuntimeHelper::p4ReadRequest(uint64_t deviceId, const std::string &role,
                             std::vector<p4_entity_t *> *entities) {
  auto obj = new p4_read_request_t();

  obj->set_device_id(deviceId);
  obj->set_role(role);

  for (auto entity : *entities) {
    obj->mutable_entities()->AddAllocated(entity);
  }

  return obj;
}

p4_replica_t *RuntimeHelper::p4Replica(uint32_t egressPort, uint32_t instance) {
  auto obj = new p4_replica_t();

  obj->set_egress_port(egressPort);
  obj->set_instance(instance);

  return obj;
}

p4_set_forwarding_pipeline_config_request_t *
RuntimeHelper::p4SetForwardingPipelineConfigRequest(
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

p4_stream_message_request_t *
RuntimeHelper::p4StreamMessageRequest(p4_master_arbitration_update_t *update) {
  auto obj = new p4_stream_message_request_t();
  obj->set_allocated_arbitration(update);
  return obj;
}

p4_stream_message_request_t *
RuntimeHelper::p4StreamMessageRequest(p4_packet_out_t *update) {
  auto obj = new p4_stream_message_request_t();
  obj->set_allocated_packet(update);
  return obj;
}

p4_table_action_t *RuntimeHelper::p4TableAction(p4_action_t *type) {
  auto obj = new p4_table_action_t();
  obj->set_allocated_action(type);
  return obj;
}

p4_table_entry_t *
RuntimeHelper::p4TableEntry(uint32_t tableId,
                            std::vector<p4_field_match_t *> *match,
                            p4_table_action_t *action, uint64_t idleTimeoutNs) {
  auto obj = new p4_table_entry_t();

  obj->set_table_id(tableId);

  if (match != nullptr) {
    for (auto fieldMatch : *match) {
      obj->mutable_match()->AddAllocated(fieldMatch);
    }
  }

  obj->set_allocated_action(action);

  if (idleTimeoutNs > 0) {
    obj->set_idle_timeout_ns(idleTimeoutNs);
  }

  return obj;
}

p4_update_t *RuntimeHelper::p4Update(p4_update_type_t type,
                                     p4_entity_t *entity) {
  auto obj = new p4_update_t();

  obj->set_type(type);
  obj->set_allocated_entity(entity);

  return obj;
}

p4_write_request_t *
RuntimeHelper::p4WriteRequest(uint64_t deviceId,
                              std::vector<p4_update_t *> *updates,
                              p4_write_request_atomicity_t atomicity,
                              uint64_t electionIdLow, uint64_t electionIdHigh) {
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

} // namespace synapse::runtime
