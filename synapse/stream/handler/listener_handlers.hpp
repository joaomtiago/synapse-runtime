#ifndef SYNAPSE_P4RUNTIME_STREAM_HANDLER_LISTENER_SPACE_HPP_
#define SYNAPSE_P4RUNTIME_STREAM_HANDLER_LISTENER_SPACE_HPP_

#include "environment/listener_environment.hpp"

namespace synapse::p4runtime {

/**
 * Below we declare the multiple listener-space handlers (which all take a
 * reference to a listener-space environment as argument).
 */
bool handleConnected(listener_env_t* env);

bool handleMakePrimarySent(listener_env_t* env);

bool handleMakePrimaryReceived(listener_env_t* env);

bool handleMessageReceived(listener_env_t* env);

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_STREAM_HANDLER_LISTENER_SPACE_HPP_