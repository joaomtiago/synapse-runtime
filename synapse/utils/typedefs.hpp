#ifndef SYNAPSE_P4RUNTIME_UTILS_TYPEDEFS_HPP_
#define SYNAPSE_P4RUNTIME_UTILS_TYPEDEFS_HPP_

#ifdef __cplusplus
#include <memory>

#include "p4/config/v1/p4info.grpc.pb.h"
#include "p4/v1/p4runtime.grpc.pb.h"
#endif  // __cplusplus

#ifdef __cplusplus
namespace synapse::p4runtime {
#endif  // __cplusplus

/**
 * Protobuf messages-related typedefs (wrappers to P4Runtime Protobuf messages,
 * such as Updates, WriteRequests, etc).
 */

// P4Info

#ifdef __cplusplus
typedef p4::config::v1::Extern p4_config_extern_t;
#else
struct p4_config_extern;
typedef struct p4_config_extern p4_config_extern_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::ExternInstance p4_config_extern_instance_t;
#else
struct p4_config_extern_instance;
typedef struct p4_config_extern_instance p4_config_extern_instance_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::MatchField p4_config_match_field_t;
#else
struct p4_config_match_field;
typedef struct p4_config_match_field p4_config_match_field_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::Table p4_config_table_t;
#else
struct p4_config_table;
typedef struct p4_config_table p4_config_table_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::ActionRef p4_config_action_ref_t;
#else
struct p4_config_action_ref;
typedef struct p4_config_action_ref p4_config_action_ref_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::Action p4_config_action_t;
#else
struct p4_config_action;
typedef struct p4_config_action p4_config_action_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::Action_Param p4_config_action_param_t;
#else
struct p4_config_action_param;
typedef struct p4_config_action_param p4_config_action_param_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::ActionProfile p4_config_action_profile_t;
#else
struct p4_config_action_profile;
typedef struct p4_config_action_profile p4_config_action_profile_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::CounterSpec p4_config_counter_spec_t;
#else
struct p4_config_counter_spec;
typedef struct p4_config_counter_spec p4_config_counter_spec_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::Counter p4_config_counter_t;
#else
struct p4_config_counter;
typedef struct p4_config_counter p4_config_counter_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::DirectCounter p4_config_direct_counter_t;
#else
struct p4_config_direct_counter;
typedef struct p4_config_direct_counter p4_config_direct_counter_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::MeterSpec p4_config_meter_spec_t;
#else
struct p4_config_meter_spec;
typedef struct p4_config_meter_spec p4_config_meter_spec_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::DirectMeter p4_config_direct_meter_t;
#else
struct p4_config_direct_meter;
typedef struct p4_config_direct_meter p4_config_direct_meter_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::ControllerPacketMetadata
    p4_config_controller_packet_metadata_t;
#else
struct p4_config_controller_packet_metadata;
typedef struct p4_config_controller_packet_metadata
    p4_config_controller_packet_metadata_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::ControllerPacketMetadata_Metadata
    p4_config_controller_packet_metadata_metadata_t;
#else
struct p4_config_controller_packet_metadata_metadata;
typedef struct p4_config_controller_packet_metadata_metadata
    p4_config_controller_packet_metadata_metadata_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::ValueSet p4_config_value_set_t;
#else
struct p4_config_value_set;
typedef struct p4_config_value_set p4_config_value_set_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::Register p4_config_register_t;
#else
struct p4_config_register;
typedef struct p4_config_register p4_config_register_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::config::v1::Digest p4_config_digest_t;
#else
struct p4_config_digest;
typedef struct p4_config_digest p4_config_digest_t;
#endif  // __cplusplus

// P4Runtime

#ifdef __cplusplus
typedef p4::v1::WriteResponse p4_write_response_t;
#else
struct p4_write_response;
typedef struct p4_write_response p4_write_response_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::WriteRequest p4_write_request_t;
#else
struct p4_write_request;
typedef struct p4_write_request p4_write_request_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::WriteRequest_Atomicity p4_write_request_atomicity_t;
#else
struct p4_write_request_atomicity;
typedef struct p4_write_request_atomicity p4_write_request_atomicity_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::ReadRequest p4_read_request_t;
#else
struct p4_read_request;
typedef struct p4_read_request p4_read_request_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::Update p4_update_t;
#else
struct p4_update;
typedef struct p4_update p4_update_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::Update_Type p4_update_type_t;
#else
struct p4_update_type;
typedef struct p4_update_type p4_update_type_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::Entity p4_entity_t;
#else
struct p4_entity;
typedef struct p4_entity p4_entity_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::TableEntry p4_table_entry_t;
#else
struct p4_table_entry;
typedef struct p4_table_entry p4_table_entry_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::PacketReplicationEngineEntry
    p4_packet_replication_engine_entry_t;
#else
struct p4_packet_replication_engine_entry;
typedef struct p4_packet_replication_engine_entry
    p4_packet_replication_engine_entry_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::FieldMatch_Exact p4_field_match_exact_t;
#else
struct p4_field_match_exact;
typedef struct p4_field_match_exact p4_field_match_exact_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::FieldMatch p4_field_match_t;
#else
struct p4_field_match;
typedef struct p4_field_match p4_field_match_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::TableAction p4_table_action_t;
#else
struct p4_table_action;
typedef struct p4_table_action p4_table_action_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::Action p4_action_t;
#else
struct p4_action;
typedef struct p4_action p4_action_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::Action_Param p4_action_param_t;
#else
struct p4_action_param;
typedef struct p4_action_param p4_action_param_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::MulticastGroupEntry p4_multicast_group_entry_t;
#else
struct p4_multicast_group_entry;
typedef struct p4_multicast_group_entry p4_multicast_group_entry_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::CloneSessionEntry p4_clone_session_entry_t;
#else
struct p4_clone_session_entry;
typedef struct p4_clone_session_entry p4_clone_session_entry_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::Replica p4_replica_t;
#else
struct p4_replica;
typedef struct p4_replica p4_replica_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::StreamMessageRequest p4_stream_message_request_t;
#else
struct p4_stream_message_request;
typedef struct p4_stream_message_request p4_stream_message_request_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::StreamMessageResponse p4_stream_message_response_t;
#else
struct p4_stream_message_response;
typedef struct p4_stream_message_response p4_stream_message_response_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::MasterArbitrationUpdate p4_master_arbitration_update_t;
#else
struct p4_master_arbitration_update;
typedef struct p4_master_arbitration_update p4_master_arbitration_update_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::PacketOut p4_packet_out_t;
#else
struct p4_packet_out;
typedef struct p4_packet_out p4_packet_out_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::PacketMetadata p4_packet_metadata_t;
#else
struct p4_packet_metadata;
typedef struct p4_packet_metadata p4_packet_metadata_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::SetForwardingPipelineConfigRequest
    p4_set_forwarding_pipeline_config_request_t;
#else
struct p4_set_forwarding_pipeline_config_request;
typedef struct p4_set_forwarding_pipeline_config_request
    p4_set_forwarding_pipeline_config_request_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::SetForwardingPipelineConfigRequest_Action
    p4_set_forwarding_pipeline_config_request_action_t;
#else
struct p4_set_forwarding_pipeline_config_request_action;
typedef struct p4_set_forwarding_pipeline_config_request_action
    p4_set_forwarding_pipeline_config_request_action_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::ForwardingPipelineConfig p4_forwarding_pipeline_config_t;
#else
struct p4_forwarding_pipeline_config;
typedef struct p4_forwarding_pipeline_config p4_forwarding_pipeline_config_t;
#endif  // __cplusplus

#ifdef __cplusplus
typedef p4::v1::IdleTimeoutNotification p4_idle_timeout_notification_t;
#else
struct p4_idle_timeout_notification;
typedef struct p4_idle_timeout_notification p4_idle_timeout_notification_t;
#endif  // __cplusplus

/**
 * gRPC-related typedefs in the scope of P4Runtime (wrappers to a P4Runtime gRPC
 * stub, to gRPC client contexts and completion queues - used by the
 * asynchronous bi-directional stream).
 */

#ifdef __cplusplus
typedef p4::v1::P4Runtime::Stub stub_t;
typedef std::unique_ptr<stub_t> stub_ptr_t;

typedef grpc::ClientContext ctx_t;
typedef std::shared_ptr<ctx_t> ctx_ptr_t;

typedef grpc::CompletionQueue queue_t;
typedef std::shared_ptr<queue_t> queue_ptr_t;
#endif  // __cplusplus

/**
 * Stream-related typedefs (request and response messages bound to the stream,
 * as as well as the reader/writer interface provided by gRPC that let's us send
 * and receive messages using the same stream - and, thus, the same client
 * context).
 */

#ifdef __cplusplus
typedef grpc::ClientAsyncReaderWriter<p4_stream_message_request_t,
                                      p4_stream_message_response_t>
    stream_t;
typedef std::shared_ptr<stream_t> stream_ptr_t;
#endif  // __cplusplus

#ifdef __cplusplus
}  // namespace synapse::p4runtime
#endif  // __cplusplus

#endif  // SYNAPSE_P4RUNTIME_UTILS_TYPEDEFS_HPP_