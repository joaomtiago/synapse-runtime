#ifndef SYNAPSE_RUNTIME_CONNECTOR_CONNECTOR_HPP_
#define SYNAPSE_RUNTIME_CONNECTOR_CONNECTOR_HPP_

#include "synapse/runtime/p4runtime/typedefs.hpp"
#include "synapse/runtime/logger/logger.hpp"
#include "synapse/runtime/wrapper/utils/wrappers.hpp"

#ifdef __cplusplus
#include <stack>
#include <thread>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
typedef class Connector {
public:
  /**
   * Create the gRPC channel, using insecure channel credentials, wait for a
   * successful connection to be established, create the key-value dictionary,
   * build the Protobuf helpers, and create the stub.
   */
  Connector(const std::string &grpcAddr, logger_logging_level_t loggingLevel);

  /**
   * Configure the underlying structure of the connector by providing a path to
   * the file containing the JSON specification of the program, and a path to
   * the file containing information about the P4 structures of the said
   * program.
   */
  bool configure(const std::string &bmv2JsonFilepath,
                 const std::string &p4InfoFilepath);

  /**
   * Create the asycnhronous bi-directional stream handler, launch the gRPC
   * thread that dispatches stream messages to the respective handlers, and wait
   * for the gRPC thread to join.
   */
  bool startAndWait();

public:
  stack_ptr_t stack;

  p4runtime_stub_ptr_t stub;

private:
  // Thread that consumes messages from the stream
  std::thread listener_thread_;

  logger_ptr_t logger_;

} conn_t;
#else
struct conn;
typedef struct conn conn_t;
#endif // __cplusplus

#ifdef __cplusplus
typedef conn_t *conn_ptr_t;
#else
typedef void *conn_ptr_t;
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_CONNECTOR_CONNECTOR_HPP_