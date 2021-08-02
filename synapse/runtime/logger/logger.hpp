#ifndef SYNAPSE_RUNTIME_LOGGER_LOGGER_HPP_
#define SYNAPSE_RUNTIME_LOGGER_LOGGER_HPP_

#ifdef __cplusplus
#include <string>
#endif // __cplusplus

#ifdef __cplusplus
namespace synapse::runtime {
#endif // __cplusplus

typedef enum {
  LoggerError = 1,
  LoggerWarning = 2,
  LoggerInfo = 3,
  LoggerDebug = 4

} logging_level_t;

#ifdef __cplusplus
class Logger {
public:
  Logger(logging_level_t level);

  void error(const std::string &message);

  void warning(const std::string &message);

  void info(const std::string &message);

  void debug(const std::string &message);

private:
  logging_level_t level_;
};
#endif // __cplusplus

#ifdef __cplusplus
typedef Logger logger_t;
typedef logger_t *logger_ptr_t;
#else
struct logger;
typedef struct logger logger_t;
typedef void *logger_ptr_t;
#endif // __cplusplus

#ifdef __cplusplus
} // namespace synapse::runtime
#endif // __cplusplus

#endif // SYNAPSE_RUNTIME_LOGGER_LOGGER_HPP_