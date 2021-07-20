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
#else
struct p4_info_action;
typedef struct p4_info_action p4_info_action_t;
#endif // __cplusplus

// Action_Param

#ifdef __cplusplus
typedef p4::config::v1::Action_Param p4_info_action_param_t;
#else
struct p4_info_action_param;
typedef struct p4_info_action_param p4_info_action_param_t;
#endif // __cplusplus

// ControllerPacketMetadata

#ifdef __cplusplus
typedef p4::config::v1::ControllerPacketMetadata
    p4_info_controller_packet_metadata_t;
#else
struct p4_info_controller_packet_metadata;
typedef struct p4_info_controller_packet_metadata
    p4_info_controller_packet_metadata_t;
#endif // __cplusplus

// ControllerPacketMetadata_Metadata

#ifdef __cplusplus
typedef p4::config::v1::ControllerPacketMetadata_Metadata
    p4_info_controller_packet_metadata_metadata_t;
#else
struct p4_info_controller_packet_metadata_metadata;
typedef struct p4_info_controller_packet_metadata_metadata
    p4_info_controller_packet_metadata_metadata_t;
#endif // __cplusplus

// MatchField

#ifdef __cplusplus
typedef p4::config::v1::MatchField p4_info_match_field_t;
#else
struct p4_info_match_field;
typedef struct p4_info_match_field p4_info_match_field_t;
#endif // __cplusplus

// P4Info

#ifdef __cplusplus
typedef p4::config::v1::P4Info p4_info_p4_info_t;
#else
struct p4_info_p4_info;
typedef struct p4_info_p4_info p4_info_p4_info_t;
#endif // __cplusplus

// Table

#ifdef __cplusplus
typedef p4::config::v1::Table p4_info_table_t;
#else
struct p4_info_table;
typedef struct p4_info_table p4_info_table_t;
#endif // __cplusplus

/**
 * P4Runtime (Protobuf) typedefs go below.
 */

// Action

#ifdef __cplusplus
typedef p4::v1::Action p4_action_t;
#else
struct p4_action;
typedef struct p4_action p4_action_t;
#endif // __cplusplus

// Action_Param

#ifdef __cplusplus
typedef p4::v1::Action_Param p4_action_param_t;
#else
struct p4_action_param;
typedef struct p4_action_param p4_action_param_t;
#endif // __cplusplus

// Entity

#ifdef __cplusplus
typedef p4::v1::Entity p4_entity_t;
#else
struct p4_entity;
typedef struct p4_entity p4_entity_t;
#endif // __cplusplus

// FieldMatch

#ifdef __cplusplus
typedef p4::v1::FieldMatch p4_field_match_t;
#else
struct p4_field_match;
typedef struct p4_field_match p4_field_match_t;
#endif // __cplusplus

// FieldMatch_Exact

#ifdef __cplusplus
typedef p4::v1::FieldMatch_Exact p4_field_match_exact_t;
#else
struct p4_field_match_exact;
typedef struct p4_field_match_exact p4_field_match_exact_t;
#endif // __cplusplus

// ForwardingPipelineConfig

#ifdef __cplusplus
typedef p4::v1::ForwardingPipelineConfig p4_forwarding_pipeline_config_t;
#else
struct p4_forwarding_pipeline_config;
typedef struct p4_forwarding_pipeline_config p4_forwarding_pipeline_config_t;
#endif // __cplusplus

// IdleTimeoutNotification

#ifdef __cplusplus
typedef p4::v1::IdleTimeoutNotification p4_idle_timeout_notification_t;
#else
struct p4_idle_timeout_notification;
typedef struct p4_idle_timeout_notification p4_idle_timeout_notification_t;
#endif // __cplusplus

// MasterArbitrationUpdate

#ifdef __cplusplus
typedef p4::v1::MasterArbitrationUpdate p4_master_arbitration_update_t;
#else
struct p4_master_arbitration_update;
typedef struct p4_master_arbitration_update p4_master_arbitration_update_t;
#endif // __cplusplus

// MulticastGroupEntry

#ifdef __cplusplus
typedef p4::v1::MulticastGroupEntry p4_multicast_group_entry_t;
#else
struct p4_multicast_group_entry;
typedef struct p4_multicast_group_entry p4_multicast_group_entry_t;
#endif // __cplusplus

// PacketMetadata

#ifdef __cplusplus
typedef p4::v1::PacketMetadata p4_packet_metadata_t;
#else
struct p4_packet_metadata;
typedef struct p4_packet_metadata p4_packet_metadata_t;
#endif // __cplusplus

// PacketReplicationEngineEntry

#ifdef __cplusplus
typedef p4::v1::PacketReplicationEngineEntry
    p4_packet_replication_engine_entry_t;
#else
struct p4_packet_replication_engine_entry;
typedef struct p4_packet_replication_engine_entry
    p4_packet_replication_engine_entry_t;
#endif // __cplusplus

// PacketOut

#ifdef __cplusplus
typedef p4::v1::PacketOut p4_packet_out_t;
#else
struct p4_packet_out;
typedef struct p4_packet_out p4_packet_out_t;
#endif // __cplusplus

// ReadRequest

#ifdef __cplusplus
typedef p4::v1::ReadRequest p4_read_request_t;
#else
struct p4_read_request;
typedef struct p4_read_request p4_read_request_t;
#endif // __cplusplus

// Replica

#ifdef __cplusplus
typedef p4::v1::Replica p4_replica_t;
#else
struct p4_replica;
typedef struct p4_replica p4_replica_t;
#endif // __cplusplus

// SetForwardingPipelineConfigRequest

#ifdef __cplusplus
typedef p4::v1::SetForwardingPipelineConfigRequest
    p4_set_forwarding_pipeline_config_request_t;
#else
struct p4_set_forwarding_pipeline_config_request;
typedef struct p4_set_forwarding_pipeline_config_request
    p4_set_forwarding_pipeline_config_request_t;
#endif // __cplusplus

// SetForwardingPipelineConfigRequest_Action

#ifdef __cplusplus
typedef p4::v1::SetForwardingPipelineConfigRequest_Action
    p4_set_forwarding_pipeline_config_request_action_t;
#else
struct p4_set_forwarding_pipeline_config_request_action;
typedef struct p4_set_forwarding_pipeline_config_request_action
    p4_set_forwarding_pipeline_config_request_action_t;
#endif // __cplusplus

// StreamMessageResponse

#ifdef __cplusplus
typedef p4::v1::StreamMessageResponse p4_stream_message_response_t;
#else
struct p4_stream_message_response;
typedef struct p4_stream_message_response p4_stream_message_response_t;
#endif // __cplusplus

// StreamMessageRequest

#ifdef __cplusplus
typedef p4::v1::StreamMessageRequest p4_stream_message_request_t;
#else
struct p4_stream_message_request;
typedef struct p4_stream_message_request p4_stream_message_request_t;
#endif // __cplusplus

// TableAction

#ifdef __cplusplus
typedef p4::v1::TableAction p4_table_action_t;
#else
struct p4_table_action;
typedef struct p4_table_action p4_table_action_t;
#endif // __cplusplus

// TableEntry

#ifdef __cplusplus
typedef p4::v1::TableEntry p4_table_entry_t;
#else
struct p4_table_entry;
typedef struct p4_table_entry p4_table_entry_t;
#endif // __cplusplus

// Update

#ifdef __cplusplus
typedef p4::v1::Update p4_update_t;
#else
struct p4_update;
typedef struct p4_update p4_update_t;
#endif // __cplusplus

// Update_Type

#ifdef __cplusplus
typedef p4::v1::Update_Type p4_update_type_t;
#else
struct p4_update_type;
typedef struct p4_update_type p4_update_type_t;
#endif // __cplusplus

// WriteResponse

#ifdef __cplusplus
typedef p4::v1::WriteResponse p4_write_response_t;
#else
struct p4_write_response;
typedef struct p4_write_response p4_write_response_t;
#endif // __cplusplus

// WriteRequest

#ifdef __cplusplus
typedef p4::v1::WriteRequest p4_write_request_t;
#else
struct p4_write_request;
typedef struct p4_write_request p4_write_request_t;
#endif // __cplusplus

// WriteRequest_Atomicity

#ifdef __cplusplus
typedef p4::v1::WriteRequest_Atomicity p4_write_request_atomicity_t;
#else
struct p4_write_request_atomicity;
typedef struct p4_write_request_atomicity p4_write_request_atomicity_t;
#endif // __cplusplus

/**
 * P4Runtime (gRPC) typedefs go below.
 */

// ClientAsyncReaderWriter

#ifdef __cplusplus
typedef grpc::ClientAsyncReaderWriter<p4_stream_message_request_t,
                                      p4_stream_message_response_t>
    p4runtime_stream_t;
typedef std::shared_ptr<p4runtime_stream_t> p4runtime_stream_ptr_t;
#else
struct p4runtime_stream;
typedef struct p4runtime_stream p4runtime_stream_t;
typedef void *p4runtime_stream_ptr_t;
#endif // __cplusplus

// Stub

#ifdef __cplusplus
typedef p4::v1::P4Runtime::Stub p4runtime_stub_t;
typedef std::unique_ptr<p4runtime_stub_t> p4runtime_stub_ptr_t;
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