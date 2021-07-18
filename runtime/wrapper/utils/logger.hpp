#ifndef SYNAPSE_RUNTIME_WRAPPER_UTILS_LOGGER_HPP_
#define SYNAPSE_RUNTIME_WRAPPER_UTILS_LOGGER_HPP_

#include "runtime/utils/logger.hpp"

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void synapse_runtime_logger_error(logger_ptr_t logger, const char *message);

void synapse_runtime_logger_warning(logger_ptr_t logger, const char *message);

void synapse_runtime_logger_info(logger_ptr_t logger, const char *message);

void synapse_runtime_logger_debug(logger_ptr_t logger, const char *message);

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_WRAPPER_UTILS_LOGGER_HPP_