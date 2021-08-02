#include "synapse/runtime/p4runtime/stream/handler/custom.hpp"
#include "synapse/runtime/wrapper/connector.hpp"
#include "synapse/runtime/wrapper/p4runtime/stream/handler/environment.hpp"
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// Constants

#define REPLICAS_SIZE 4

// Helpers

#define S_STRING(var_name, var_value, var_size)                                \
  static string_t var_name = {.value = var_value, .value_sz = var_size}

#define NEW_STRING(var_value, var_size)                                        \
  synapse_runtime_wrappers_string_new(var_value, var_size)

#define REALLOC(ptr, var_type, new_size)                                       \
  (var_type *)realloc(ptr, (new_size) * sizeof(var_type));

string_ptr_t get_packet_metadata_by_name(helper_ptr_t helper,
                                         pair_ptr_t *metadata,
                                         size_t metadata_size,
                                         string_ptr_t metadata_name) {
  assert(NULL != helper);
  assert(NULL != metadata);

  uint32_t metadata_id =
      synapse_runtime_p4_info_controller_packet_metadata_metadata_id(
          synapse_runtime_p4_info_controller_packet_metadata_metadata_new(
              helper,
              synapse_runtime_p4_info_controller_packet_metadata_new(
                  helper, synapse_runtime_wrappers_string_new("packet_in", 9)),
              metadata_name));

  for (uint32_t i = 0; i < metadata_size; i++) {
    if (metadata_id == *((uint32_t *)metadata[i]->left)) {
      return metadata[i]->right;
    }
  }

  return NULL;
}

bool insert_table_entry(env_ptr_t env, string_ptr_t table_name,
                        pair_ptr_t *key_matches, size_t key_matches_size,
                        string_ptr_t action_name, pair_ptr_t *action_params,
                        size_t action_params_size, uint64_t idle_timeout_ns) {
  assert(NULL != env);
  helper_ptr_t helper = synapse_runtime_environment_helper(env);
  size_t i = 0;

  // Get the P4 context
  p4_info_table_ptr_t table_info =
      synapse_runtime_p4_info_table_new(helper, table_name);
  p4_info_action_ptr_t action_info =
      synapse_runtime_p4_info_action_new(helper, action_name);

  p4_field_match_ptr_t *matches = NULL;
  do {
    matches = REALLOC(matches, p4_field_match_ptr_t, i + 1);
    assert(NULL != matches);

    pair_ptr_t key_match = *(key_matches + i);
    string_ptr_t field = key_match->left;
    string_ptr_t value = key_match->right;

    // Retrieve field ID
    uint32_t field_id = synapse_runtime_p4_info_match_field_id(
        synapse_runtime_p4_info_match_field_new(helper, table_info, field));

    // Append to the key matches
    *(matches + i) = synapse_runtime_p4_field_match_new(
        helper, field_id,
        synapse_runtime_p4_field_match_exact_new(helper, value));

  } while (++i < key_matches_size);

  p4_action_param_ptr_t *params = NULL;
  for (i = 0; i < action_params_size; i++) {
    params = REALLOC(params, p4_action_param_ptr_t, i + 1);
    assert(NULL != params);

    pair_ptr_t action_param = *(action_params + i);
    string_ptr_t field = action_param->left;
    string_ptr_t value = action_param->right;

    // Retrieve field ID
    uint32_t field_id = synapse_runtime_p4_info_action_param_id(
        synapse_runtime_p4_info_action_param_new(helper, action_info, field));

    *(params + i) =
        synapse_runtime_p4_action_param_new(helper, field_id, value);
  }

  p4_info_preamble_ptr_t table_preamble =
      synapse_runtime_p4_info_table_preamble(table_info);
  uint32_t table_preamble_id = synapse_runtime_p4_preamble_id(table_preamble);

  p4_info_preamble_ptr_t action_preamble =
      synapse_runtime_p4_info_action_preamble(action_info);
  uint32_t action_preamble_id = synapse_runtime_p4_preamble_id(action_preamble);

  p4_action_ptr_t action = synapse_runtime_p4_action_new(
      helper, action_preamble_id, params, action_params_size);

  p4_table_action_ptr_t table_action =
      synapse_runtime_p4_table_action_new(helper, action);

  p4_table_entry_ptr_t table_entry = synapse_runtime_p4_table_entry_new(
      helper, table_preamble_id, matches, key_matches_size, table_action,
      idle_timeout_ns);

  p4_entity_ptr_t entity =
      synapse_runtime_p4_entity_table_entry_new(helper, table_entry);

  p4_update_ptr_t update =
      synapse_runtime_p4_update_new(helper, Update_Type_INSERT, entity);

  return synapse_runtime_update_buffer_buffer(
      synapse_runtime_environment_update_buffer(env), update);
}

bool install_multicast_group(env_ptr_t env, uint32_t multicast_group_id,
                             pair_ptr_t *replicas, size_t replicas_size) {
  helper_ptr_t helper = synapse_runtime_environment_helper(env);

  p4_replica_ptr_t *p4_replicas = NULL;
  for (size_t i = 0; i < replicas_size; i++) {
    p4_replicas = REALLOC(p4_replicas, p4_replica_ptr_t, i + 1);
    assert(NULL != p4_replicas);

    pair_ptr_t pair = *(replicas + i);
    uint32_t egress_port = (uint32_t) * (size_t *)pair->left;
    uint32_t instance = (uint32_t) * (size_t *)pair->right;

    *(p4_replicas + i) =
        synapse_runtime_p4_replica_new(helper, egress_port, instance);
  }

  p4_multicast_group_entry_ptr_t mcast_group_entry =
      synapse_runtime_p4_multicast_group_entry_new(helper, multicast_group_id,
                                                   p4_replicas, replicas_size);

  p4_packet_replication_engine_entry_ptr_t pre_entry =
      synapse_runtime_p4_packet_replication_engine_entry_new(helper,
                                                             mcast_group_entry);

  p4_entity_ptr_t entity =
      synapse_runtime_p4_entity_packet_replication_engine_entry_new(helper,
                                                                    pre_entry);

  p4_update_ptr_t update =
      synapse_runtime_p4_update_new(helper, Update_Type_INSERT, entity);

  return synapse_runtime_update_buffer_buffer(
      synapse_runtime_environment_update_buffer(env), update);
}

bool synapse_runtime_handle_pre_configure(env_ptr_t env) {
  printf("Preconfiguring the switch...\n");

  pair_ptr_t *replicas =
      (pair_ptr_t *)malloc(REPLICAS_SIZE * sizeof(pair_ptr_t));

  for (size_t i = 0; i < REPLICAS_SIZE; i++) {
    uint32_t *ptr = malloc(sizeof(uint32_t));
    assert(NULL != ptr);
    *ptr = (uint32_t)i + 1;

    *(replicas + i) = synapse_runtime_wrappers_pair_new(ptr, ptr);
  }

  // Clear the stack since we are not sending a packet to the switch
  synapse_runtime_wrappers_stack_clear(synapse_runtime_environment_stack(env));
  return install_multicast_group(env, 1, replicas, REPLICAS_SIZE);
}

bool synapse_runtime_handle_packet_received(env_ptr_t env) {
  printf("Received a packet...\n");

  // Tables
  S_STRING(mac_src_exact_str, "MyIngress.mac_src_exact", 23);
  S_STRING(mac_dst_exact_str, "MyIngress.mac_dst_exact", 23);

  // Field matches
  S_STRING(eth_src_addr_str, "hdr.ethernet.srcAddr", 20);
  S_STRING(eth_dst_addr_str, "hdr.ethernet.dstAddr", 20);

  // Actions
  S_STRING(forward_str, "MyIngress.forward", 17);
  S_STRING(no_action_str, "NoAction", 8);

  // Actions params
  S_STRING(ingress_port_str, "ingress_port", 12);
  S_STRING(egress_port_str, "egress_port", 11);

  // Other constants
  static uint64_t idle_timeout_ns = 14400000000000;

  /**
   * The environment stack should look like this:
   * ^
   * | payload size + payload
   * | metadata size
   * | metadata
   * |-----------------------
   */

  stack_ptr_t stack = synapse_runtime_environment_stack(env);
  assert(3 == synapse_runtime_wrappers_stack_size(stack));

  string_ptr_t payload = synapse_runtime_wrappers_stack_pop(stack);
  size_t metadata_size = *(size_t *)synapse_runtime_wrappers_stack_pop(stack);
  pair_ptr_t *metadata = synapse_runtime_wrappers_stack_pop(stack);
  printf("Received a new packet of size %lu B...\n", payload->value_sz);

  // Get source address from the packet, and ingress port from the metadata
  port_ptr_t ingress_port = synapse_runtime_wrappers_parse_port(
      get_packet_metadata_by_name(synapse_runtime_environment_helper(env),
                                  metadata, metadata_size, &ingress_port_str));
  assert(NULL != ingress_port);

  payload->value += 6; // Skip destination address
  mac_addr_ptr_t src_mac_addr =
      synapse_runtime_wrappers_parse_mac_address(payload);
  assert(NULL != src_mac_addr);

  // Write to `MyIngress.mac_src_exact`
  size_t mac_src_exact_key_matches_size = 1;
  pair_ptr_t *mac_src_exact_key_matches = (pair_ptr_t *)malloc(
      mac_src_exact_key_matches_size * sizeof(mac_src_exact_key_matches));

  *mac_src_exact_key_matches =
      synapse_runtime_wrappers_pair_new(&eth_src_addr_str, src_mac_addr->value);

  if (!insert_table_entry(env, &mac_src_exact_str, mac_src_exact_key_matches,
                          mac_src_exact_key_matches_size, &no_action_str, NULL,
                          0, 0)) {
    return false;
  };

  // Write to `MyIngress.mac_dst_exact`
  size_t mac_dst_exact_key_matches_size = 1;
  pair_ptr_t *mac_dst_exact_key_matches = (pair_ptr_t *)malloc(
      mac_dst_exact_key_matches_size * sizeof(mac_dst_exact_key_matches));

  *mac_dst_exact_key_matches =
      synapse_runtime_wrappers_pair_new(&eth_dst_addr_str, src_mac_addr->value);

  size_t mac_dst_exact_action_params_size = 1;
  pair_ptr_t *mac_dst_exact_action_params = (pair_ptr_t *)malloc(
      mac_dst_exact_action_params_size * sizeof(mac_dst_exact_action_params));

  *mac_dst_exact_action_params =
      synapse_runtime_wrappers_pair_new(&egress_port_str, ingress_port->value);

  // Send the packet back to the switch
  synapse_runtime_wrappers_stack_push(stack, NULL);

  size_t *packet_out_metadata_size = malloc(sizeof(size_t));
  *packet_out_metadata_size = 0;
  synapse_runtime_wrappers_stack_push(stack, packet_out_metadata_size);

  synapse_runtime_wrappers_stack_push(stack, payload);

  return insert_table_entry(env, &mac_dst_exact_str, mac_dst_exact_key_matches,
                            mac_dst_exact_key_matches_size, &forward_str,
                            mac_dst_exact_action_params,
                            mac_dst_exact_action_params_size, idle_timeout_ns);
}

bool synapse_runtime_handle_idle_timeout_notification_received(env_ptr_t env) {
  printf("Received an idle timeout notification...\n");
  return true;
}

int main(int argc, char const *argv[]) {
  if (argc != 4) {
    printf("usage: ./c_controller <grpcAddr> <p4InfoFilepath> "
           "<bmv2JsonFilepath>\n");
    exit(1);
  }

  conn_ptr_t connector = synapse_runtime_connector_new(argv[1], LoggerDebug);
  if (synapse_runtime_connector_configure(connector, argv[3], argv[2])) {
    synapse_runtime_connector_start_and_wait(connector);
  }

  synapse_runtime_connector_destroy(connector);
  return 0;
}
