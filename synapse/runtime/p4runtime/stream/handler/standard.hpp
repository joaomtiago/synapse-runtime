#ifndef SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_STANDARD_HPP_
#define SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_STANDARD_HPP_

#include "synapse/runtime/p4runtime/stream/handler/environment.hpp"

namespace synapse::runtime {

bool handleConnected(env_ptr_t, stack_ptr_t, void **);

bool handleMakePrimarySent(env_ptr_t, stack_ptr_t, void **);

bool handleMakePrimaryReceived(env_ptr_t, stack_ptr_t, void **);

bool handleMessageReceived(env_ptr_t, stack_ptr_t, void **);

bool handleMessageSent(env_ptr_t, stack_ptr_t, void **);

} // namespace synapse::runtime

#endif // SYNAPSE_RUNTIME_P4RUNTIME_STREAM_HANDLER_STANDARD_HPP_