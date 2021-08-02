#include "synapse/runtime/wrapper/p4runtime/helper.hpp"

#define TO_STD_STRING(str) std::string(str->value, str->value_sz)

#define NOT_NULL(ptr) assert(nullptr != ptr)

namespace synapse::runtime {

/**
 * P4Info helpers go below.
 */

p4_info_action_ptr_t
synapse_runtime_p4_info_action_new(helper_ptr_t helper,
                                   string_ptr_t action_name) {
  NOT_NULL(helper);
  return helper->p4InfoAction(TO_STD_STRING(action_name));
}

// After `synapse_runtime_p4_info_action`
p4_info_preamble_ptr_t
synapse_runtime_p4_info_action_preamble(p4_info_action_ptr_t action) {
  NOT_NULL(action);
  return action->mutable_preamble();
}

p4_info_action_param_ptr_t synapse_runtime_p4_info_action_param_new(
    helper_ptr_t helper, p4_info_action_ptr_t action, string_ptr_t param_name) {
  NOT_NULL(helper);
  return helper->p4InfoActionParam(action, TO_STD_STRING(param_name));
}

// After `synapse_runtime_p4_info_action_param_new`
uint32_t synapse_runtime_p4_info_action_param_id(
    p4_info_action_param_ptr_t action_param) {
  NOT_NULL(action_param);
  return action_param->id();
}

p4_info_controller_packet_metadata_ptr_t
synapse_runtime_p4_info_controller_packet_metadata_new(
    helper_ptr_t helper, string_ptr_t controller_packet_metadata_name) {
  NOT_NULL(helper);
  return helper->p4InfoControllerPacketMetadata(
      TO_STD_STRING(controller_packet_metadata_name));
}

p4_info_controller_packet_metadata_metadata_ptr_t
synapse_runtime_p4_info_controller_packet_metadata_metadata_new(
    helper_ptr_t helper,
    p4_info_controller_packet_metadata_ptr_t controller_packet_metadata,
    string_ptr_t metadata_name) {
  NOT_NULL(helper);
  return helper->p4InfoControllerPacketMetadataMetadata(
      controller_packet_metadata, TO_STD_STRING(metadata_name));
}

// After `synapse_runtime_p4_info_controller_packet_metadata_metadata_new`
uint32_t synapse_runtime_p4_info_controller_packet_metadata_metadata_id(
    p4_info_controller_packet_metadata_metadata_ptr_t metadata) {
  NOT_NULL(metadata);
  return metadata->id();
}

p4_info_match_field_ptr_t
synapse_runtime_p4_info_match_field_new(helper_ptr_t helper,
                                        p4_info_table_ptr_t table,
                                        string_ptr_t match_field_name) {
  NOT_NULL(helper);
  return helper->p4InfoMatchField(table, TO_STD_STRING(match_field_name));
}

// After `synapse_runtime_p4_info_match_field_new`
uint32_t
synapse_runtime_p4_info_match_field_id(p4_info_match_field_ptr_t match_field) {
  NOT_NULL(match_field);
  return match_field->id();
}

p4_info_p4_info_ptr_t synapse_runtime_p4_info_p4_info_new(helper_ptr_t helper) {
  NOT_NULL(helper);
  return helper->p4InfoP4Info();
}

p4_info_table_ptr_t synapse_runtime_p4_info_table_new(helper_ptr_t helper,
                                                      string_ptr_t table_name) {
  NOT_NULL(helper);
  return helper->p4InfoTable(TO_STD_STRING(table_name));
}

// After `synapse_runtime_p4_info_table_new`
p4_info_preamble_ptr_t
synapse_runtime_p4_info_table_preamble(p4_info_table_ptr_t table) {
  NOT_NULL(table);
  return table->mutable_preamble();
}

uint32_t synapse_runtime_p4_preamble_id(p4_info_preamble_ptr_t preamble) {
  NOT_NULL(preamble);
  return preamble->id();
}

/**
 * P4Runtime helpers go below.
 */

p4_action_ptr_t synapse_runtime_p4_action_new(helper_ptr_t helper,
                                              uint32_t action_id,
                                              p4_action_param_ptr_t *params,
                                              size_t params_size) {
  NOT_NULL(helper);

  if (params_size > 0) {
    NOT_NULL(params);
  }

  auto paramsVector = new std::vector<p4_action_param_ptr_t>();
  for (size_t i = 0; i < params_size; i++) {
    paramsVector->push_back(params[i]);
  }

  return helper->p4Action(action_id, paramsVector);
}

p4_action_param_ptr_t synapse_runtime_p4_action_param_new(helper_ptr_t helper,
                                                          uint32_t param_id,
                                                          string_ptr_t value) {
  NOT_NULL(helper);
  return helper->p4ActionParam(param_id, TO_STD_STRING(value));
}

p4_entity_ptr_t
synapse_runtime_p4_entity_table_entry_new(helper_ptr_t helper,
                                          p4_table_entry_ptr_t entry) {
  NOT_NULL(helper);
  return helper->p4Entity(entry);
}

p4_entity_ptr_t synapse_runtime_p4_entity_packet_replication_engine_entry_new(
    helper_ptr_t helper, p4_packet_replication_engine_entry_ptr_t entry) {
  NOT_NULL(helper);
  return helper->p4Entity(entry);
}

p4_field_match_ptr_t synapse_runtime_p4_field_match_new(
    helper_ptr_t helper, uint32_t field_id,
    p4_field_match_exact_ptr_t field_match_type) {
  NOT_NULL(helper);
  return helper->p4FieldMatch(field_id, field_match_type);
}

p4_field_match_exact_ptr_t
synapse_runtime_p4_field_match_exact_new(helper_ptr_t helper,
                                         string_ptr_t value) {
  NOT_NULL(helper);
  return helper->p4FieldMatchExact(TO_STD_STRING(value));
}

p4_forwarding_pipeline_config_ptr_t
synapse_runtime_p4_forwarding_pipeline_config_new(
    helper_ptr_t helper, p4_info_p4_info_ptr_t p4_info,
    string_ptr_t p4_device_config) {
  NOT_NULL(helper);
  return helper->p4ForwardingPipelineConfig(p4_info,
                                            TO_STD_STRING(p4_device_config));
}

p4_master_arbitration_update_ptr_t
synapse_runtime_p4_master_arbitration_update_new(helper_ptr_t helper,
                                                 uint64_t device_id,
                                                 uint64_t election_id_low,
                                                 uint64_t election_id_high) {
  NOT_NULL(helper);
  return helper->p4MasterArbitrationUpdate(device_id, election_id_low,
                                           election_id_high);
}

p4_multicast_group_entry_ptr_t synapse_runtime_p4_multicast_group_entry_new(
    helper_ptr_t helper, uint32_t multicast_group_id,
    p4_replica_ptr_t *replicas, size_t replicas_size) {
  NOT_NULL(helper);

  if (replicas_size > 0) {
    NOT_NULL(replicas);
  }

  auto replicasVector = new std::vector<p4_replica_ptr_t>();
  for (size_t i = 0; i < replicas_size; i++) {
    replicasVector->push_back(replicas[i]);
  }

  return helper->p4MulticastGroupEntry(multicast_group_id, replicasVector);
}

p4_packet_metadata_ptr_t synapse_runtime_p4_packet_metadata_new(
    helper_ptr_t helper, uint32_t metadata_id, string_ptr_t value) {
  NOT_NULL(helper);
  return helper->p4PacketMetadata(metadata_id, TO_STD_STRING(value));
}

p4_packet_out_ptr_t
synapse_runtime_p4_packet_out_new(helper_ptr_t helper, string_ptr_t payload,
                                  p4_packet_metadata_ptr_t *metadata,
                                  size_t metadata_size) {
  NOT_NULL(helper);

  if (metadata_size > 0) {
    NOT_NULL(metadata);
  }

  auto metadataVector = new std::vector<p4_packet_metadata_ptr_t>();
  for (size_t i = 0; i < metadata_size; i++) {
    metadataVector->push_back(metadata[i]);
  }

  return helper->p4PacketOut(TO_STD_STRING(payload), metadataVector);
}

p4_packet_replication_engine_entry_ptr_t
synapse_runtime_p4_packet_replication_engine_entry_new(
    helper_ptr_t helper, p4_multicast_group_entry_ptr_t type) {
  NOT_NULL(helper);
  return helper->p4PacketReplicationEngineEntry(type);
}

p4_read_request_ptr_t synapse_runtime_p4_read_request_new(
    helper_ptr_t helper, uint64_t device_id, string_ptr_t role,
    p4_entity_ptr_t *entities, size_t entities_size) {
  NOT_NULL(helper);

  if (entities_size > 0) {
    NOT_NULL(entities);
  }

  auto entitiesVector = new std::vector<p4_entity_ptr_t>();
  for (size_t i = 0; i < entities_size; i++) {
    entitiesVector->push_back(entities[i]);
  }

  return helper->p4ReadRequest(device_id, TO_STD_STRING(role), entitiesVector);
}

p4_replica_ptr_t synapse_runtime_p4_replica_new(helper_ptr_t helper,
                                                uint32_t egress_port,
                                                uint32_t instance) {
  NOT_NULL(helper);
  return helper->p4Replica(egress_port, instance);
}

p4_set_forwarding_pipeline_config_request_ptr_t
synapse_runtime_p4_set_forwarding_pipeline_config_request_new(
    helper_ptr_t helper, uint64_t device_id, uint64_t election_id_low,
    uint64_t election_id_high,
    p4_set_forwarding_pipeline_config_request_action_t action,
    p4_forwarding_pipeline_config_ptr_t config) {
  NOT_NULL(helper);
  return helper->p4SetForwardingPipelineConfigRequest(
      device_id, election_id_low, election_id_high, action, config);
}

p4_stream_message_request_ptr_t
synapse_runtime_p4_stream_message_request_master_arbitration_update_new(
    helper_ptr_t helper, p4_master_arbitration_update_ptr_t update) {
  NOT_NULL(helper);
  return helper->p4StreamMessageRequest(update);
}

p4_stream_message_request_ptr_t
synapse_runtime_p4_stream_message_request_packet_out_new(
    helper_ptr_t helper, p4_packet_out_ptr_t update) {
  NOT_NULL(helper);
  return helper->p4StreamMessageRequest(update);
}

p4_table_action_ptr_t
synapse_runtime_p4_table_action_new(helper_ptr_t helper, p4_action_ptr_t type) {
  NOT_NULL(helper);
  return helper->p4TableAction(type);
}

p4_table_entry_ptr_t synapse_runtime_p4_table_entry_new(
    helper_ptr_t helper, uint32_t table_id, p4_field_match_ptr_t *match,
    size_t match_size, p4_table_action_ptr_t action, uint64_t idle_timeout_ns) {
  NOT_NULL(helper);

  if (match_size > 0) {
    NOT_NULL(match);
  }

  auto matchVector = new std::vector<p4_field_match_ptr_t>();
  for (size_t i = 0; i < match_size; i++) {
    matchVector->push_back(match[i]);
  }

  return helper->p4TableEntry(table_id, matchVector, action, idle_timeout_ns);
}

p4_update_ptr_t synapse_runtime_p4_update_new(helper_ptr_t helper,
                                              p4_update_type_t type,
                                              p4_entity_ptr_t entity) {
  NOT_NULL(helper);
  return helper->p4Update(type, entity);
}

p4_write_request_ptr_t synapse_runtime_p4_write_request_new(
    helper_ptr_t helper, uint64_t device_id, p4_update_ptr_t *updates,
    size_t updates_size, p4_write_request_atomicity_t atomicity,
    uint64_t election_id_low, uint64_t election_id_high) {
  NOT_NULL(helper);

  if (updates_size > 0) {
    NOT_NULL(updates);
  }

  auto updatesVector = new std::vector<p4_update_ptr_t>();
  for (size_t i = 0; i < updates_size; i++) {
    updatesVector->push_back(updates[i]);
  }

  return helper->p4WriteRequest(device_id, updatesVector, atomicity,
                                election_id_low, election_id_high);
}

} // namespace synapse::runtime