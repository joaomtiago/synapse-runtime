#include "synapse/runtime/wrapper/logger/logger.hpp"

namespace synapse::runtime {

void synapse_runtime_logger_error(logger_ptr_t logger, const char *message) {
  logger->error(message);
}

void synapse_runtime_logger_warning(logger_ptr_t logger, const char *message) {
  logger->warning(message);
}

void synapse_runtime_logger_info(logger_ptr_t logger, const char *message) {
  logger->info(message);
}

void synapse_runtime_logger_debug(logger_ptr_t logger, const char *message) {
  logger->debug(message);
}

} // namespace synapse::runtime
