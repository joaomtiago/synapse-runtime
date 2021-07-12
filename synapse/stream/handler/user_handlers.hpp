#ifndef SYNAPSE_P4RUNTIME_STREAM_HANDLER_USER_SPACE_HPP_
#define SYNAPSE_P4RUNTIME_STREAM_HANDLER_USER_SPACE_HPP_

#include "environment/user_environment.hpp"

namespace synapse::p4runtime {

/**
 * Below we declare the multiple user-space handlers (which all take a reference
 * to a user-space environment as argument).
 */

bool preConfigure(user_env_t *env);

bool handlePacketReceived(user_env_t *env);

bool handleIdleTimeoutNotificationReceived(user_env_t *env);

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_P4RUNTIME_STREAM_HANDLER_USER_SPACE_HPP_