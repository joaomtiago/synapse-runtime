#ifndef SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_HELPER_HPP_
#define SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_HELPER_HPP_

#include "synapse/runtime/p4runtime/helper.hpp"
#include "synapse/runtime/wrapper/utils/wrappers.hpp"

#ifndef __cplusplus
#include <stdint.h>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * P4Info helpers go below.
 */

p4_info_action_ptr_t
synapse_runtime_p4_info_action_new(helper_ptr_t helper,
                                   string_ptr_t action_name);

// After `synapse_runtime_p4_info_action_new`
p4_info_preamble_ptr_t
synapse_runtime_p4_info_action_preamble(p4_info_action_ptr_t action);

p4_info_action_param_ptr_t synapse_runtime_p4_info_action_param_new(
    helper_ptr_t helper, p4_info_action_ptr_t action, string_ptr_t param_name);

// After `synapse_runtime_p4_info_action_param_new`
uint32_t synapse_runtime_p4_info_action_param_id(
    p4_info_action_param_ptr_t action_param);

p4_info_controller_packet_metadata_ptr_t
synapse_runtime_p4_info_controller_packet_metadata_new(
    helper_ptr_t helper, string_ptr_t controller_packet_metadata_name);

p4_info_controller_packet_metadata_metadata_ptr_t
synapse_runtime_p4_info_controller_packet_metadata_metadata_new(
    helper_ptr_t helper,
    p4_info_controller_packet_metadata_ptr_t controller_packet_metadata,
    string_ptr_t metadata_name);

// After `synapse_runtime_p4_info_controller_packet_metadata_metadata_new`
uint32_t synapse_runtime_p4_info_controller_packet_metadata_metadata_id(
    p4_info_controller_packet_metadata_metadata_ptr_t metadata);

p4_info_match_field_ptr_t
synapse_runtime_p4_info_match_field_new(helper_ptr_t helper,
                                        p4_info_table_ptr_t table,
                                        string_ptr_t match_field_name);

// After `synapse_runtime_p4_info_match_field_new`
uint32_t
synapse_runtime_p4_info_match_field_id(p4_info_match_field_ptr_t match_field);

p4_info_p4_info_ptr_t synapse_runtime_p4_info_p4_info_new(helper_ptr_t helper);

p4_info_table_ptr_t synapse_runtime_p4_info_table_new(helper_ptr_t helper,
                                                      string_ptr_t table_name);

// After `synapse_runtime_p4_info_table_new`
p4_info_preamble_ptr_t
synapse_runtime_p4_info_table_preamble(p4_info_table_ptr_t table);

uint32_t synapse_runtime_p4_preamble_id(p4_info_preamble_ptr_t preamble);

/**
 * P4Runtime helpers go below.
 */

p4_action_ptr_t synapse_runtime_p4_action_new(helper_ptr_t helper,
                                              uint32_t action_id,
                                              p4_action_param_ptr_t *params,
                                              size_t params_size);

p4_action_param_ptr_t synapse_runtime_p4_action_param_new(helper_ptr_t helper,
                                                          uint32_t param_id,
                                                          string_ptr_t value);

p4_entity_ptr_t
synapse_runtime_p4_entity_table_entry_new(helper_ptr_t helper,
                                          p4_table_entry_ptr_t entry);

p4_entity_ptr_t synapse_runtime_p4_entity_packet_replication_engine_entry_new(
    helper_ptr_t helper, p4_packet_replication_engine_entry_ptr_t entry);

p4_field_match_ptr_t
synapse_runtime_p4_field_match_new(helper_ptr_t helper, uint32_t field_id,
                                   p4_field_match_exact_ptr_t field_match_type);

p4_field_match_exact_ptr_t
synapse_runtime_p4_field_match_exact_new(helper_ptr_t helper,
                                         string_ptr_t value);

p4_forwarding_pipeline_config_ptr_t
synapse_runtime_p4_forwarding_pipeline_config_new(
    helper_ptr_t helper, p4_info_p4_info_ptr_t p4_info,
    string_ptr_t p4_device_config);

p4_master_arbitration_update_ptr_t
synapse_runtime_p4_master_arbitration_update_new(helper_ptr_t helper,
                                                 uint64_t device_id,
                                                 uint64_t election_id_low,
                                                 uint64_t election_id_high);

p4_multicast_group_entry_ptr_t synapse_runtime_p4_multicast_group_entry_new(
    helper_ptr_t helper, uint32_t multicast_group_id,
    p4_replica_ptr_t *replicas, size_t replicas_size);

p4_packet_metadata_ptr_t synapse_runtime_p4_packet_metadata_new(
    helper_ptr_t helper, uint32_t metadata_id, string_ptr_t value);

p4_packet_out_ptr_t
synapse_runtime_p4_packet_out_new(helper_ptr_t helper, string_ptr_t payload,
                                  p4_packet_metadata_ptr_t *metadata,
                                  size_t metadata_size);

p4_packet_replication_engine_entry_ptr_t
synapse_runtime_p4_packet_replication_engine_entry_new(
    helper_ptr_t helper, p4_multicast_group_entry_ptr_t type);

p4_read_request_ptr_t synapse_runtime_p4_read_request_new(
    helper_ptr_t helper, uint64_t device_id, string_ptr_t role,
    p4_entity_ptr_t *entities, size_t entities_size);

p4_replica_ptr_t synapse_runtime_p4_replica_new(helper_ptr_t helper,
                                                uint32_t egress_port,
                                                uint32_t instance);

p4_set_forwarding_pipeline_config_request_ptr_t
synapse_runtime_p4_set_forwarding_pipeline_config_request_new(
    helper_ptr_t helper, uint64_t device_id, uint64_t election_id_low,
    uint64_t election_id_high,
    p4_set_forwarding_pipeline_config_request_action_t action,
    p4_forwarding_pipeline_config_ptr_t config);

p4_stream_message_request_ptr_t
synapse_runtime_p4_stream_message_request_master_arbitration_update_new(
    helper_ptr_t helper, p4_master_arbitration_update_ptr_t update);

p4_stream_message_request_ptr_t
synapse_runtime_p4_stream_message_request_packet_out_new(
    helper_ptr_t helper, p4_packet_out_ptr_t update);

p4_table_action_ptr_t synapse_runtime_p4_table_action_new(helper_ptr_t helper,
                                                          p4_action_ptr_t type);

p4_table_entry_ptr_t synapse_runtime_p4_table_entry_new(
    helper_ptr_t helper, uint32_t table_id, p4_field_match_ptr_t *match,
    size_t match_size, p4_table_action_ptr_t action, uint64_t idle_timeout_ns);

p4_update_ptr_t synapse_runtime_p4_update_new(helper_ptr_t helper,
                                              p4_update_type_t type,
                                              p4_entity_ptr_t entity);

p4_write_request_ptr_t synapse_runtime_p4_write_request_new(
    helper_ptr_t helper, uint64_t device_id, p4_update_ptr_t *updates,
    size_t updates_size, p4_write_request_atomicity_t atomicity,
    uint64_t election_id_low, uint64_t election_id_high);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_P4RUNTIME_HELPER_HPP_