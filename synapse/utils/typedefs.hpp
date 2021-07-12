#ifndef SYNAPSE_P4RUNTIME_UTILS_TYPEDEFS_HPP_
#define SYNAPSE_P4RUNTIME_UTILS_TYPEDEFS_HPP_

#include <memory>

#include "p4/v1/p4runtime.grpc.pb.h"
#include "synapse/protobuf/p4runtime.hpp"
#include "synapse/protobuf/p4runtime_info.hpp"

namespace synapse::p4runtime {

/**
 * Custom helpers that wrap the the building process of P4Runtime Protobuf
 * objects (such as write requests, table entries, etc.).
 *
 * The difference between P4RuntimeHelper and P4RuntimeInfoHelper is that
 * the latter is strictly bound to the P4Runtime configuration environment
 * (known as P4Runtime information), which provides valuable information on
 * documentation, forwarding pipeline configuration, P4 tables and actions,
 * etc.
 */

typedef P4RuntimeInfoHelper info_helper_t;
typedef std::shared_ptr<P4RuntimeInfoHelper> info_helper_ptr_t;

typedef P4RuntimeHelper helper_t;
typedef std::shared_ptr<P4RuntimeHelper> helper_ptr_t;

/**
 * gRPC-related typedefs in the scope of P4Runtime (wrappers to a P4Runtime gRPC
 * stub, to gRPC client contexts and completion queues - used by the
 * asynchronous bi-directional stream).
 */

typedef std::unique_ptr<p4::v1::P4Runtime::Stub> stub_ptr_t;

typedef grpc::ClientContext ctx_t;
typedef std::shared_ptr<ctx_t> ctx_ptr_t;

typedef grpc::CompletionQueue queue_t;
typedef std::shared_ptr<queue_t> queue_ptr_t;

/**
 * Stream-related typedefs (request and response messages bound to the stream,
 * as as well as the reader/writer interface provided by gRPC that let's us send
 * and receive messages using the same stream - and, thus, the same client
 * context).
 */

typedef p4::v1::StreamMessageRequest stream_request_t;
typedef p4::v1::StreamMessageResponse stream_response_t;

typedef grpc::ClientAsyncReaderWriter<stream_request_t, stream_response_t>
    stream_t;
typedef std::shared_ptr<stream_t> stream_ptr_t;

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_UTILS_TYPEDEFS_HPP_