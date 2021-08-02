#ifndef SYNAPSE_RUNTIME_P4RUNTIME_HELPER_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_HELPER_HPP_

#include "synapse/runtime/p4runtime/typedefs.hpp"

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
class RuntimeHelper {
public:
  RuntimeHelper(const std::string *p4InfoFilepath);

  ~RuntimeHelper() {}

  /**
   * P4Info helpers go below.
   */

  p4_info_action_ptr_t p4InfoAction(std::string actionName);

  p4_info_action_param_ptr_t p4InfoActionParam(p4_info_action_ptr_t action,
                                               std::string paramName);

  p4_info_controller_packet_metadata_ptr_t
  p4InfoControllerPacketMetadata(std::string controllerPacketMetadataName);

  p4_info_controller_packet_metadata_metadata_ptr_t
  p4InfoControllerPacketMetadataMetadata(
      p4_info_controller_packet_metadata_ptr_t controllerPacketMetadata,
      std::string metadataName);

  p4_info_match_field_ptr_t p4InfoMatchField(p4_info_table_ptr_t table,
                                             std::string matchFieldName);

  p4_info_p4_info_ptr_t p4InfoP4Info();

  p4_info_table_ptr_t p4InfoTable(std::string tableName);

  /**
   * P4Runtime helpers go below.
   */

  p4_action_ptr_t p4Action(uint32_t actionId,
                           std::vector<p4_action_param_ptr_t> *params);

  p4_action_param_ptr_t p4ActionParam(uint32_t paramId,
                                      const std::string &value);

  p4_entity_ptr_t p4Entity(p4_table_entry_ptr_t entry);

  p4_entity_ptr_t p4Entity(p4_packet_replication_engine_entry_ptr_t entry);

  p4_field_match_ptr_t p4FieldMatch(uint32_t fieldId,
                                    p4_field_match_exact_ptr_t fieldMatchType);

  p4_field_match_exact_ptr_t p4FieldMatchExact(const std::string &value);

  p4_forwarding_pipeline_config_ptr_t
  p4ForwardingPipelineConfig(p4_info_p4_info_ptr_t p4Info,
                             const std::string &p4DeviceConfig);

  p4_master_arbitration_update_ptr_t
  p4MasterArbitrationUpdate(uint64_t deviceId, uint64_t electionIdLow,
                            uint64_t electionIdHigh);

  p4_multicast_group_entry_ptr_t
  p4MulticastGroupEntry(uint32_t multicastGroupId,
                        std::vector<p4_replica_ptr_t> *replicas);

  p4_packet_metadata_ptr_t p4PacketMetadata(uint32_t metadataId,
                                            const std::string &value);

  p4_packet_out_ptr_t
  p4PacketOut(const std::string &payload,
              std::vector<p4_packet_metadata_ptr_t> *metadata);

  p4_packet_replication_engine_entry_ptr_t
  p4PacketReplicationEngineEntry(p4_multicast_group_entry_ptr_t type);

  p4_read_request_ptr_t p4ReadRequest(uint64_t deviceId,
                                      const std::string &role,
                                      std::vector<p4_entity_ptr_t> *entities);

  p4_replica_ptr_t p4Replica(uint32_t egressPort, uint32_t instance);

  p4_set_forwarding_pipeline_config_request_ptr_t
  p4SetForwardingPipelineConfigRequest(
      uint64_t deviceId, uint64_t electionIdLow, uint64_t electionIdHigh,
      p4_set_forwarding_pipeline_config_request_action_t action,
      p4_forwarding_pipeline_config_ptr_t config);

  p4_stream_message_request_ptr_t
  p4StreamMessageRequest(p4_master_arbitration_update_ptr_t update);

  p4_stream_message_request_ptr_t
  p4StreamMessageRequest(p4_packet_out_ptr_t update);

  p4_table_action_ptr_t p4TableAction(p4_action_ptr_t type);

  p4_table_entry_ptr_t p4TableEntry(uint32_t tableId,
                                    std::vector<p4_field_match_ptr_t> *match,
                                    p4_table_action_ptr_t action,
                                    uint64_t idleTimeoutNs);

  p4_update_ptr_t p4Update(p4_update_type_t type, p4_entity_ptr_t entity);

  p4_write_request_ptr_t p4WriteRequest(uint64_t deviceId,
                                        std::vector<p4_update_ptr_t> *updates,
                                        p4_write_request_atomicity_t atomicity,
                                        uint64_t electionIdLow,
                                        uint64_t electionIdHigh);

private:
  std::shared_ptr<p4_info_p4_info_t> p4Info_;
};
#endif // __cplusplus

#ifdef __cplusplus
typedef RuntimeHelper helper_t;
typedef helper_t *helper_ptr_t;
#else
struct helper;
typedef struct helper helper_t;
typedef void *helper_ptr_t;
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_P4RUNTIME_HELPER_HPP_