#ifndef SYNAPSE_RUNTIME_P4RUNTIME_TYPEDEFS_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_TYPEDEFS_HPP_

/**
 * All P4Runtime-related type definitions (typedef) should be declared here.
 * This file is required by the majority of the project, and makes it easier to
 * dynamically add new typedefs on-the-fly.
 *
 * Typedefs are prefixed according to the `.proto` file they refer to:
 *  - `p4/config/v1/p4info.proto` (Protobuf)    => p4_info_?_t
 *  - `p4/v1/p4runtime.proto` (Protobuf)        => p4_?_t
 *  - `p4/v1/p4runtime.proto` (gRPC)            => p4runtime_?_t
 *  - gRPC entities                             => grpc_?_t
 */

#ifdef __cplusplus
#include <memory>
#endif // __cplusplus

#ifdef __cplusplus
#include "p4/config/v1/p4info.grpc.pb.h"
#include "p4/v1/p4runtime.grpc.pb.h"
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

/**
 * P4Info (Protobuf) typedefs go below.
 */

// Action

#ifdef __cplusplus
typedef p4::config::v1::Action p4_info_action_t;
typedef p4_info_action_t *p4_info_action_ptr_t;
#else
struct p4_info_action;
typedef struct p4_info_action p4_info_action_t;
typedef void *p4_info_action_ptr_t;
#endif // __cplusplus

// Action_Param

#ifdef __cplusplus
typedef p4::config::v1::Action_Param p4_info_action_param_t;
typedef p4_info_action_param_t *p4_info_action_param_ptr_t;
#else
struct p4_info_action_param;
typedef struct p4_info_action_param p4_info_action_param_t;
typedef void *p4_info_action_param_ptr_t;
#endif // __cplusplus

// ControllerPacketMetadata

#ifdef __cplusplus
typedef p4::config::v1::ControllerPacketMetadata
    p4_info_controller_packet_metadata_t;
typedef p4_info_controller_packet_metadata_t
    *p4_info_controller_packet_metadata_ptr_t;
#else
struct p4_info_controller_packet_metadata;
typedef struct p4_info_controller_packet_metadata
    p4_info_controller_packet_metadata_t;
typedef void *p4_info_controller_packet_metadata_ptr_t;
#endif // __cplusplus

// ControllerPacketMetadata_Metadata

#ifdef __cplusplus
typedef p4::config::v1::ControllerPacketMetadata_Metadata
    p4_info_controller_packet_metadata_metadata_t;
typedef p4_info_controller_packet_metadata_metadata_t
    *p4_info_controller_packet_metadata_metadata_ptr_t;
#else
struct p4_info_controller_packet_metadata_metadata;
typedef struct p4_info_controller_packet_metadata_metadata
    p4_info_controller_packet_metadata_metadata_t;
typedef void *p4_info_controller_packet_metadata_metadata_ptr_t;
#endif // __cplusplus

// MatchField

#ifdef __cplusplus
typedef p4::config::v1::MatchField p4_info_match_field_t;
typedef p4_info_match_field_t *p4_info_match_field_ptr_t;
#else
struct p4_info_match_field;
typedef struct p4_info_match_field p4_info_match_field_t;
typedef void *p4_info_match_field_ptr_t;
#endif // __cplusplus

// P4Info

#ifdef __cplusplus
typedef p4::config::v1::P4Info p4_info_p4_info_t;
typedef p4_info_p4_info_t *p4_info_p4_info_ptr_t;
#else
struct p4_info_p4_info;
typedef struct p4_info_p4_info p4_info_p4_info_t;
typedef void *p4_info_p4_info_ptr_t;
#endif // __cplusplus

// Preamble

#ifdef __cplusplus
typedef p4::config::v1::Preamble p4_info_preamble_t;
typedef p4_info_preamble_t *p4_info_preamble_ptr_t;
#else
struct p4_info_preamble;
typedef struct p4_info_preamble p4_info_preamble_t;
typedef void *p4_info_preamble_ptr_t;
#endif // __cplusplus

// Table

#ifdef __cplusplus
typedef p4::config::v1::Table p4_info_table_t;
typedef p4_info_table_t *p4_info_table_ptr_t;
#else
struct p4_info_table;
typedef struct p4_info_table p4_info_table_t;
typedef void *p4_info_table_ptr_t;
#endif // __cplusplus

/**
 * P4Runtime (Protobuf) typedefs go below.
 */

// Action

#ifdef __cplusplus
typedef p4::v1::Action p4_action_t;
typedef p4_action_t *p4_action_ptr_t;
#else
struct p4_action;
typedef struct p4_action p4_action_t;
typedef void *p4_action_ptr_t;
#endif // __cplusplus

// Action_Param

#ifdef __cplusplus
typedef p4::v1::Action_Param p4_action_param_t;
typedef p4_action_param_t *p4_action_param_ptr_t;
#else
struct p4_action_param;
typedef struct p4_action_param p4_action_param_t;
typedef void *p4_action_param_ptr_t;
#endif // __cplusplus

// Entity

#ifdef __cplusplus
typedef p4::v1::Entity p4_entity_t;
typedef p4_entity_t *p4_entity_ptr_t;
#else
struct p4_entity;
typedef struct p4_entity p4_entity_t;
typedef void *p4_entity_ptr_t;
#endif // __cplusplus

// FieldMatch

#ifdef __cplusplus
typedef p4::v1::FieldMatch p4_field_match_t;
typedef p4_field_match_t *p4_field_match_ptr_t;
#else
struct p4_field_match;
typedef struct p4_field_match p4_field_match_t;
typedef void *p4_field_match_ptr_t;
#endif // __cplusplus

// FieldMatch_Exact

#ifdef __cplusplus
typedef p4::v1::FieldMatch_Exact p4_field_match_exact_t;
typedef p4_field_match_exact_t *p4_field_match_exact_ptr_t;
#else
struct p4_field_match_exact;
typedef struct p4_field_match_exact p4_field_match_exact_t;
typedef void *p4_field_match_exact_ptr_t;
#endif // __cplusplus

// ForwardingPipelineConfig

#ifdef __cplusplus
typedef p4::v1::ForwardingPipelineConfig p4_forwarding_pipeline_config_t;
typedef p4_forwarding_pipeline_config_t *p4_forwarding_pipeline_config_ptr_t;
#else
struct p4_forwarding_pipeline_config;
typedef struct p4_forwarding_pipeline_config p4_forwarding_pipeline_config_t;
typedef void *p4_forwarding_pipeline_config_ptr_t;
#endif // __cplusplus

// IdleTimeoutNotification

#ifdef __cplusplus
typedef p4::v1::IdleTimeoutNotification p4_idle_timeout_notification_t;
typedef p4_idle_timeout_notification_t *p4_idle_timeout_notification_ptr_t;
#else
struct p4_idle_timeout_notification;
typedef struct p4_idle_timeout_notification p4_idle_timeout_notification_t;
typedef void *p4_idle_timeout_notification_ptr_t;
#endif // __cplusplus

// MasterArbitrationUpdate

#ifdef __cplusplus
typedef p4::v1::MasterArbitrationUpdate p4_master_arbitration_update_t;
typedef p4_master_arbitration_update_t *p4_master_arbitration_update_ptr_t;
#else
struct p4_master_arbitration_update;
typedef struct p4_master_arbitration_update p4_master_arbitration_update_t;
typedef void *p4_master_arbitration_update_ptr_t;
#endif // __cplusplus

// MulticastGroupEntry

#ifdef __cplusplus
typedef p4::v1::MulticastGroupEntry p4_multicast_group_entry_t;
typedef p4_multicast_group_entry_t *p4_multicast_group_entry_ptr_t;
#else
struct p4_multicast_group_entry;
typedef struct p4_multicast_group_entry p4_multicast_group_entry_t;
typedef void *p4_multicast_group_entry_ptr_t;
#endif // __cplusplus

// PacketIn

#ifdef __cplusplus
typedef p4::v1::PacketIn p4_packet_in_t;
typedef p4_packet_in_t *p4_packet_in_ptr_t;
#else
struct p4_packet_in;
typedef struct p4_packet_in p4_packet_in_t;
typedef void *p4_packet_in_ptr_t;
#endif // __cplusplus

// PacketMetadata

#ifdef __cplusplus
typedef p4::v1::PacketMetadata p4_packet_metadata_t;
typedef p4_packet_metadata_t *p4_packet_metadata_ptr_t;
#else
struct p4_packet_metadata;
typedef struct p4_packet_metadata p4_packet_metadata_t;
typedef void *p4_packet_metadata_ptr_t;
#endif // __cplusplus

// PacketReplicationEngineEntry

#ifdef __cplusplus
typedef p4::v1::PacketReplicationEngineEntry
    p4_packet_replication_engine_entry_t;
typedef p4_packet_replication_engine_entry_t
    *p4_packet_replication_engine_entry_ptr_t;
#else
struct p4_packet_replication_engine_entry;
typedef struct p4_packet_replication_engine_entry
    p4_packet_replication_engine_entry_t;
typedef void *p4_packet_replication_engine_entry_ptr_t;
#endif // __cplusplus

// PacketOut

#ifdef __cplusplus
typedef p4::v1::PacketOut p4_packet_out_t;
typedef p4_packet_out_t *p4_packet_out_ptr_t;
#else
struct p4_packet_out;
typedef struct p4_packet_out p4_packet_out_t;
typedef void *p4_packet_out_ptr_t;
#endif // __cplusplus

// ReadResponse

#ifdef __cplusplus
typedef p4::v1::ReadResponse p4_read_response_t;
typedef p4_read_response_t *p4_read_response_ptr_t;
#else
struct p4_read_response;
typedef struct p4_read_response p4_read_response_t;
typedef void *p4_read_response_ptr_t;
#endif // __cplusplus

// ReadRequest

#ifdef __cplusplus
typedef p4::v1::ReadRequest p4_read_request_t;
typedef p4_read_request_t *p4_read_request_ptr_t;
#else
struct p4_read_request;
typedef struct p4_read_request p4_read_request_t;
typedef void *p4_read_request_ptr_t;
#endif // __cplusplus

// Replica

#ifdef __cplusplus
typedef p4::v1::Replica p4_replica_t;
typedef p4_replica_t *p4_replica_ptr_t;
#else
struct p4_replica;
typedef struct p4_replica p4_replica_t;
typedef void *p4_replica_ptr_t;
#endif // __cplusplus

// SetForwardingPipelineConfigResponse

#ifdef __cplusplus
typedef p4::v1::SetForwardingPipelineConfigResponse
    p4_set_forwarding_pipeline_config_response_t;
typedef p4_set_forwarding_pipeline_config_response_t
    *p4_set_forwarding_pipeline_config_response_ptr_t;
#else
struct p4_set_forwarding_pipeline_config_response;
typedef struct p4_set_forwarding_pipeline_config_response
    p4_set_forwarding_pipeline_config_response_t;
typedef void *p4_set_forwarding_pipeline_config_response_ptr_t;
#endif // __cplusplus

// SetForwardingPipelineConfigRequest

#ifdef __cplusplus
typedef p4::v1::SetForwardingPipelineConfigRequest
    p4_set_forwarding_pipeline_config_request_t;
typedef p4_set_forwarding_pipeline_config_request_t
    *p4_set_forwarding_pipeline_config_request_ptr_t;
#else
struct p4_set_forwarding_pipeline_config_request;
typedef struct p4_set_forwarding_pipeline_config_request
    p4_set_forwarding_pipeline_config_request_t;
typedef void *p4_set_forwarding_pipeline_config_request_ptr_t;
#endif // __cplusplus

// SetForwardingPipelineConfigRequest_Action

#ifdef __cplusplus
typedef p4::v1::SetForwardingPipelineConfigRequest_Action
    p4_set_forwarding_pipeline_config_request_action_t;
typedef p4_set_forwarding_pipeline_config_request_action_t
    *p4_set_forwarding_pipeline_config_request_action_ptr_t;
#else
struct p4_set_forwarding_pipeline_config_request_action;
typedef struct p4_set_forwarding_pipeline_config_request_action
    p4_set_forwarding_pipeline_config_request_action_t;
typedef void *p4_set_forwarding_pipeline_config_request_action_ptr_t;
#endif // __cplusplus

// StreamMessageResponse

#ifdef __cplusplus
typedef p4::v1::StreamMessageResponse p4_stream_message_response_t;
typedef p4_stream_message_response_t *p4_stream_message_response_ptr_t;
#else
struct p4_stream_message_response;
typedef struct p4_stream_message_response p4_stream_message_response_t;
typedef void *p4_stream_message_response_ptr_t;
#endif // __cplusplus

// StreamMessageRequest

#ifdef __cplusplus
typedef p4::v1::StreamMessageRequest p4_stream_message_request_t;
typedef p4_stream_message_request_t *p4_stream_message_request_ptr_t;
#else
struct p4_stream_message_request;
typedef struct p4_stream_message_request p4_stream_message_request_t;
typedef void *p4_stream_message_request_ptr_t;
#endif // __cplusplus

// TableAction

#ifdef __cplusplus
typedef p4::v1::TableAction p4_table_action_t;
typedef p4_table_action_t *p4_table_action_ptr_t;
#else
struct p4_table_action;
typedef struct p4_table_action p4_table_action_t;
typedef void *p4_table_action_ptr_t;
#endif // __cplusplus

// TableEntry

#ifdef __cplusplus
typedef p4::v1::TableEntry p4_table_entry_t;
typedef p4_table_entry_t *p4_table_entry_ptr_t;
#else
struct p4_table_entry;
typedef struct p4_table_entry p4_table_entry_t;
typedef void *p4_table_entry_ptr_t;
#endif // __cplusplus

// Update

#ifdef __cplusplus
typedef p4::v1::Update p4_update_t;
typedef p4_update_t *p4_update_ptr_t;
#else
struct p4_update;
typedef struct p4_update p4_update_t;
typedef void *p4_update_ptr_t;
#endif // __cplusplus

// Update_Type

#ifdef __cplusplus
typedef p4::v1::Update_Type p4_update_type_t;
typedef p4_update_type_t *p4_update_type_ptr_t;
#else
typedef enum Update_Type {
  Update_Type_INSERT = 1,
  Update_Type_MODIFY = 2,
  Update_Type_DELETE = 3
} p4_update_type_t;
typedef void *p4_update_type_ptr_t;
#endif // __cplusplus

// WriteResponse

#ifdef __cplusplus
typedef p4::v1::WriteResponse p4_write_response_t;
typedef p4_write_response_t *p4_write_response_ptr_t;
#else
struct p4_write_response;
typedef struct p4_write_response p4_write_response_t;
typedef void *p4_write_response_ptr_t;
#endif // __cplusplus

// WriteRequest

#ifdef __cplusplus
typedef p4::v1::WriteRequest p4_write_request_t;
typedef p4_write_request_t *p4_write_request_ptr_t;
#else
struct p4_write_request;
typedef struct p4_write_request p4_write_request_t;
typedef void *p4_write_request_ptr_t;
#endif // __cplusplus

// WriteRequest_Atomicity

#ifdef __cplusplus
typedef p4::v1::WriteRequest_Atomicity p4_write_request_atomicity_t;
typedef p4_write_request_atomicity_t *p4_write_request_atomicity_ptr_t;
#else
struct p4_write_request_atomicity;
typedef struct p4_write_request_atomicity p4_write_request_atomicity_t;
typedef void *p4_write_request_atomicity_ptr_t;
#endif // __cplusplus

/**
 * P4Runtime (gRPC) typedefs go below.
 */

// ClientAsyncReaderWriter

#ifdef __cplusplus
typedef grpc::ClientAsyncReaderWriter<p4_stream_message_request_t,
                                      p4_stream_message_response_t>
    p4runtime_stream_t;
typedef p4runtime_stream_t *p4runtime_stream_ptr_t; // fixme
#else
struct p4runtime_stream;
typedef struct p4runtime_stream p4runtime_stream_t;
typedef void *p4runtime_stream_ptr_t;
#endif // __cplusplus

// Stub

#ifdef __cplusplus
typedef p4::v1::P4Runtime::Stub p4runtime_stub_t;
typedef p4runtime_stub_t *p4runtime_stub_ptr_t; // fixme
#else
struct p4runtime_stub;
typedef struct p4runtime_stub p4runtime_stub_t;
typedef void *p4runtime_stub_ptr_t;
#endif // __cplusplus

/**
 * gRPC entities typedefs go below.
 */

// ClientContext

#ifdef __cplusplus
typedef grpc::ClientContext grpc_cctx_t;
typedef std::shared_ptr<grpc_cctx_t> grpc_cctx_ptr_t;
#else
struct grpc_cctx;
typedef struct grpc_cctx grpc_cctx_t;
typedef void *grpc_cctx_ptr_t;
#endif // __cplusplus

// CompletionQueue

#ifdef __cplusplus
typedef grpc::CompletionQueue grpc_cqueue_t;
typedef std::shared_ptr<grpc_cqueue_t> grpc_cqueue_ptr_t;
#else
struct grpc_cqueue;
typedef struct grpc_cqueue grpc_cqueue_t;
typedef void *grpc_cqueue_ptr_t;
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_P4RUNTIME_TYPEDEFS_HPP_