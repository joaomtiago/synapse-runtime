#ifndef SYNAPSE_RUNTIME_CONNECTOR_CONNECTOR_HPP_
#define SYNAPSE_RUNTIME_CONNECTOR_CONNECTOR_HPP_

#include "runtime/utils/logger.hpp"
#include "runtime/wrapper/p4runtime/helper.hpp"

#ifdef __cplusplus
#include <thread>
#include <unordered_map>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
typedef class Connector {
public:
  typedef class ConnectorParams {
    /**
     * This is very basic key-value dictionary.
     * It quashes the need for passing string parameters in the tag.
     * Just add them to the dictionary, and retrieve them when needed.
     */

  public:
    void put(const std::string &key, const std::string &value);

    std::string find(const std::string &key);

    void erase(const std::string &key);

  private:
    std::unordered_map<std::string, std::string> dict_;

  } conn_params_t;

public:
  /**
   * Create the gRPC channel, using insecure channel credentials, wait for a
   * successful connection to be established, create the key-value dictionary,
   * build the Protobuf helpers, and create the stub.
   */
  Connector(const std::string &grpcAddr, const std::string &p4InfoFilepath,
            logging_level_t loggingLevel);

  /**
   * Create the asycnhronous bi-directional stream handler, launch the gRPC
   * thread that dispatches stream messages to the respective handlers, and wait
   * for the gRPC thread to join.
   */
  bool startAndWait(const std::string &bmv2JsonFilepath);

public:
  conn_params_t params;

  helper_ptr_t helper;

  p4runtime_stub_ptr_t stub;

  p4_write_request_t *request;
  p4_write_response_t *response;

private:
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