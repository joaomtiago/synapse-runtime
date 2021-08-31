#include "synapse/runtime/p4runtime/helper.hpp"
#include "google/protobuf/text_format.h"
#include "synapse/runtime/exception/runtime_exception.hpp"
#include "synapse/runtime/utils/file.hpp"
#include "synapse/runtime/utils/wrappers.hpp"

#define NOT_NULL(exp) assert(nullptr != (exp))

#define ITER(vec) for (auto it = vec->begin(); it != vec->end(); ++it)

namespace synapse::runtime {

RuntimeHelper::RuntimeHelper(const std::string *p4InfoFilepath) {
  p4_info_p4_info_ptr_t p4Info = new p4_info_p4_info_t();

  if (true != google::protobuf::TextFormat::MergeFromString(
                  readFile(*p4InfoFilepath), p4Info)) {
    throw RuntimeException("Could not parse P4 Info file");
  }

  p4Info_.reset(p4Info);
}

// P4Info helpers go below

p4_info_action_ptr_t
RuntimeHelper::p4InfoAction(const std::string &actionName) {
  ITER(p4Info_->mutable_actions()) {
    if (actionName == it->mutable_preamble()->name()) {
      return new p4_info_action_t(*it);
    }
  }

  throw RuntimeException("Unknown action `" + actionName + "`");
}

p4_info_action_param_ptr_t
RuntimeHelper::p4InfoActionParam(p4_info_action_ptr_t action,
                                 const std::string &paramName) {
  ITER(action->mutable_params()) {
    if (paramName == it->name()) {
      return new p4_info_action_param_t(*it);
    }
  }

  throw RuntimeException("Unknown parameter `" + paramName + "` for action `" +
                         action->mutable_preamble()->name() + "`");
}

p4_info_controller_packet_metadata_ptr_t
RuntimeHelper::p4InfoControllerPacketMetadata(
    const std::string &controllerPacketMetadataName) {
  ITER(p4Info_->mutable_controller_packet_metadata()) {
    if (controllerPacketMetadataName == it->mutable_preamble()->name()) {
      return new p4_info_controller_packet_metadata_t(*it);
    }
  }

  throw RuntimeException("Unknown controller packet metadata `" +
                         controllerPacketMetadataName + "`");
}

p4_info_controller_packet_metadata_metadata_ptr_t
RuntimeHelper::p4InfoControllerPacketMetadataMetadataById(
    p4_info_controller_packet_metadata_ptr_t controllerPacketMetadata,
    const uint32_t &metadataId) {
  ITER(controllerPacketMetadata->mutable_metadata()) {
    if (metadataId == it->id()) {
      return new p4_info_controller_packet_metadata_metadata_t(*it);
    }
  }

  throw RuntimeException(
      "Unknown controller packet metadata metadata with ID `" +
      std::to_string(metadataId) + "`");
}

p4_info_controller_packet_metadata_metadata_ptr_t
RuntimeHelper::p4InfoControllerPacketMetadataMetadataByName(
    p4_info_controller_packet_metadata_ptr_t controllerPacketMetadata,
    const std::string &metadataName) {
  ITER(controllerPacketMetadata->mutable_metadata()) {
    if (metadataName == it->name()) {
      return new p4_info_controller_packet_metadata_metadata_t(*it);
    }
  }

  throw RuntimeException(
      "Unknown controller packet metadata metadata with name `" + metadataName +
      "`");
}

p4_info_match_field_ptr_t
RuntimeHelper::p4InfoMatchField(p4_info_table_ptr_t table,
                                const std::string &matchFieldName) {
  ITER(table->mutable_match_fields()) {
    if (matchFieldName == it->name()) {
      return new p4_info_match_field_t(*it);
    }
  }

  throw RuntimeException("Unknown match field `" + matchFieldName +
                         "` for table `" + table->preamble().name() + "`");
}

p4_info_match_field_ptr_t
RuntimeHelper::p4InfoMatchFieldById(p4_info_table_ptr_t table,
                                    const uint32_t &matchFieldId) {
  ITER(table->mutable_match_fields()) {
    if (matchFieldId == it->id()) {
      return new p4_info_match_field_t(*it);
    }
  }

  throw RuntimeException("Unknown match field with ID `" +
                         std::to_string(matchFieldId) + "` for table `" +
                         table->preamble().name() + "`");
}

p4_info_p4_info_ptr_t RuntimeHelper::p4InfoP4Info() { return p4Info_.get(); }

p4_info_table_ptr_t RuntimeHelper::p4InfoTable(const std::string &tableName) {
  ITER(p4Info_->mutable_tables()) {
    if (tableName == it->mutable_preamble()->name()) {
      return new p4_info_table_t(*it);
    }
  }

  throw RuntimeException("Unknown table `" + tableName + "`");
}

p4_info_table_ptr_t RuntimeHelper::p4InfoTableById(const uint32_t &tableId) {
  ITER(p4Info_->mutable_tables()) {
    if (tableId == it->mutable_preamble()->id()) {
      return new p4_info_table_t(*it);
    }
  }

  throw RuntimeException("Unknown table with ID " + std::to_string(tableId));
}

// P4Runtime helpers go below

p4_action_ptr_t
RuntimeHelper::p4Action(const uint32_t &actionId,
                        std::vector<p4_action_param_ptr_t> *params) {
  auto obj = new p4_action_t();

  obj->set_action_id(actionId);

  NOT_NULL(params);
  ITER(params) { obj->mutable_params()->AddAllocated(*it); }

  return obj;
}

p4_action_param_ptr_t RuntimeHelper::p4ActionParam(const uint32_t &paramId,
                                                   const std::string &value) {
  auto obj = new p4_action_param_t();

  obj->set_param_id(paramId);

  obj->set_value(value);

  return obj;
}

p4_entity_ptr_t
RuntimeHelper::p4Entity(p4_packet_replication_engine_entry_ptr_t entity) {
  auto obj = new p4_entity_t();

  NOT_NULL(entity);
  obj->set_allocated_packet_replication_engine_entry(entity);

  return obj;
}

p4_entity_ptr_t RuntimeHelper::p4Entity(p4_table_entry_ptr_t entity) {
  auto obj = new p4_entity_t();

  NOT_NULL(entity);
  obj->set_allocated_table_entry(entity);

  return obj;
}

p4_field_match_ptr_t
RuntimeHelper::p4FieldMatch(const uint32_t &fieldId,
                            p4_field_match_exact_ptr_t fieldMatchType) {
  auto obj = new p4_field_match_t();

  obj->set_field_id(fieldId);

  NOT_NULL(fieldMatchType);
  obj->set_allocated_exact(fieldMatchType);

  return obj;
}

p4_field_match_ptr_t
RuntimeHelper::p4FieldMatch(const uint32_t &fieldId,
                            p4_field_match_optional_ptr_t fieldMatchType) {
  auto obj = new p4_field_match_t();

  obj->set_field_id(fieldId);

  NOT_NULL(fieldMatchType);
  obj->set_allocated_optional(fieldMatchType);

  return obj;
}

p4_field_match_ptr_t
RuntimeHelper::p4FieldMatch(const uint32_t &fieldId,
                            p4_field_match_range_ptr_t fieldMatchType) {
  auto obj = new p4_field_match_t();

  obj->set_field_id(fieldId);

  NOT_NULL(fieldMatchType);
  obj->set_allocated_range(fieldMatchType);

  return obj;
}

p4_field_match_exact_ptr_t
RuntimeHelper::p4FieldMatchExact(const std::string &value) {
  auto obj = new p4_field_match_exact_t();

  obj->set_value(value);

  return obj;
}

p4_field_match_optional_ptr_t
RuntimeHelper::p4FieldMatchOptional(const std::string &value) {
  auto obj = new p4_field_match_optional_t();

  obj->set_value(value);

  return obj;
}

p4_field_match_range_ptr_t
RuntimeHelper::p4FieldMatchRange(const std::string &low,
                                 const std::string &high) {
  auto obj = new p4_field_match_range_t();

  obj->set_low(low);

  obj->set_high(high);

  return obj;
}

p4_forwarding_pipeline_config_ptr_t
RuntimeHelper::p4ForwardingPipelineConfig(p4_info_p4_info_ptr_t p4Info,
                                          const std::string &p4DeviceConfig) {
  auto obj = new p4_forwarding_pipeline_config_t();

  NOT_NULL(p4Info);
  obj->set_allocated_p4info(p4Info);

  obj->set_p4_device_config(p4DeviceConfig);

  return obj;
}

p4_master_arbitration_update_ptr_t
RuntimeHelper::p4MasterArbitrationUpdate(const uint64_t &deviceId,
                                         const uint64_t &electionIdLow,
                                         const uint64_t &electionIdHigh) {
  auto obj = new p4_master_arbitration_update_t();

  obj->set_device_id(deviceId);

  obj->mutable_election_id()->set_low(electionIdLow);
  obj->mutable_election_id()->set_high(electionIdHigh);

  return obj;
}

p4_multicast_group_entry_ptr_t
RuntimeHelper::p4MulticastGroupEntry(const uint32_t &multicastGroupId,
                                     std::vector<p4_replica_ptr_t> *replicas) {
  auto obj = new p4_multicast_group_entry_t();

  obj->set_multicast_group_id(multicastGroupId);

  NOT_NULL(replicas);
  ITER(replicas) { obj->mutable_replicas()->AddAllocated(*it); }

  return obj;
}

p4_packet_metadata_ptr_t
RuntimeHelper::p4PacketMetadata(const uint32_t &metadataId,
                                const std::string &value) {
  auto obj = new p4_packet_metadata_t();

  obj->set_metadata_id(metadataId);

  obj->set_value(value);

  return obj;
}

p4_packet_out_ptr_t
RuntimeHelper::p4PacketOut(const std::string &payload,
                           std::vector<p4_packet_metadata_ptr_t> *metadata) {
  auto obj = new p4_packet_out_t();

  obj->set_payload(payload);

  NOT_NULL(metadata);
  ITER(metadata) { obj->mutable_metadata()->AddAllocated(*it); }

  return obj;
}

p4_packet_replication_engine_entry_ptr_t
RuntimeHelper::p4PacketReplicationEngineEntry(
    p4_multicast_group_entry_ptr_t type) {
  auto obj = new p4_packet_replication_engine_entry_t();

  NOT_NULL(type);
  obj->set_allocated_multicast_group_entry(type);

  return obj;
}

p4_read_request_ptr_t
RuntimeHelper::p4ReadRequest(const uint64_t &deviceId,
                             std::vector<p4_entity_ptr_t> *entities) {
  auto obj = new p4_read_request_t();

  obj->set_device_id(deviceId);

  NOT_NULL(entities);
  ITER(entities) { obj->mutable_entities()->AddAllocated(*it); }

  return obj;
}

p4_replica_ptr_t RuntimeHelper::p4Replica(const uint32_t &egressPort,
                                          const uint32_t &instance) {
  auto obj = new p4_replica_t();

  obj->set_egress_port(egressPort);

  obj->set_instance(instance);

  return obj;
}

p4_set_forwarding_pipeline_config_request_ptr_t
RuntimeHelper::p4SetForwardingPipelineConfigRequest(
    const uint64_t &deviceId, const uint64_t &electionIdLow,
    const uint64_t &electionIdHigh,
    p4_set_forwarding_pipeline_config_request_action_t action,
    p4_forwarding_pipeline_config_ptr_t config) {
  auto obj = new p4_set_forwarding_pipeline_config_request_t();

  obj->set_device_id(deviceId);

  obj->mutable_election_id()->set_low(electionIdLow);
  obj->mutable_election_id()->set_high(electionIdHigh);

  obj->set_action(action);

  NOT_NULL(config);
  obj->set_allocated_config(config);

  return obj;
}

p4_stream_message_request_ptr_t RuntimeHelper::p4StreamMessageRequest(
    p4_master_arbitration_update_ptr_t update) {
  auto obj = new p4_stream_message_request_t();

  NOT_NULL(update);
  obj->set_allocated_arbitration(update);

  return obj;
}

p4_stream_message_request_ptr_t
RuntimeHelper::p4StreamMessageRequest(p4_packet_out_ptr_t update) {
  auto obj = new p4_stream_message_request_t();

  NOT_NULL(update);
  obj->set_allocated_packet(update);

  return obj;
}

p4_table_action_ptr_t RuntimeHelper::p4TableAction(p4_action_ptr_t type) {
  auto obj = new p4_table_action_t();

  NOT_NULL(type);
  obj->set_allocated_action(type);

  return obj;
}

p4_table_entry_ptr_t RuntimeHelper::p4TableEntry(
    const uint32_t &tableId, std::vector<p4_field_match_ptr_t> *match,
    p4_table_action_ptr_t action, const int32_t &priority,
    const uint64_t &idleTimeoutNs) {
  auto obj = new p4_table_entry_t();

  obj->set_table_id(tableId);

  if (nullptr != match) {
    ITER(match) { obj->mutable_match()->AddAllocated(*it); }
  }

  if (nullptr != action) {
    obj->set_allocated_action(action);
  }

  obj->set_priority(priority);

  obj->set_idle_timeout_ns(idleTimeoutNs);

  return obj;
}

p4_update_ptr_t RuntimeHelper::p4Update(p4_update_type_t type,
                                        p4_entity_ptr_t entity) {
  auto obj = new p4_update_t();

  obj->set_type(type);

  NOT_NULL(entity);
  obj->set_allocated_entity(entity);

  return obj;
}

p4_write_request_ptr_t RuntimeHelper::p4WriteRequest(
    const uint64_t &deviceId, std::vector<p4_update_ptr_t> *updates,
    p4_write_request_atomicity_t atomicity, const uint64_t &electionIdLow,
    const uint64_t &electionIdHigh) {
  auto obj = new p4_write_request_t();

  obj->set_device_id(deviceId);

  NOT_NULL(updates);
  ITER(updates) { obj->mutable_updates()->AddAllocated(*it); }

  obj->set_atomicity(atomicity);

  obj->mutable_election_id()->set_low(electionIdLow);
  obj->mutable_election_id()->set_high(electionIdHigh);

  return obj;
}

} // namespace synapse::runtime
