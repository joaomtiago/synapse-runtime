#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_STANDARD_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_STANDARD_HPP_

#include "synapse/runtime/p4runtime/stream/handler/environment/standard.hpp"

namespace synapse::runtime {

/**
 * Below we declare the multiple listener-space handlers (which all take a
 * reference to a listener-space environment as argument).
 */
bool handleConnected(standard_env_ptr_t env);

bool handleMakePrimarySent(standard_env_ptr_t env);

bool handleMakePrimaryReceived(standard_env_ptr_t env);

bool handleMessageReceived(standard_env_ptr_t env);

} // namespace synapse::runtime

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_STANDARD_HPP_