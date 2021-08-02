#include "synapse/runtime/logger/logger.hpp"
#include <iostream>

namespace synapse::runtime {

Logger::Logger(logging_level_t level) : level_(level) {}

void Logger::error(const std::string &message) {
  if (level_ >= logging_level_t::LoggerError) {
    std::cerr << "[-] " << message << std::endl;
  }
}

void Logger::warning(const std::string &message) {
  if (level_ >= logging_level_t::LoggerWarning) {
    std::cerr << "[!] " << message << std::endl;
  }
}

void Logger::info(const std::string &message) {
  if (level_ >= logging_level_t::LoggerInfo) {
    std::cout << "[+] " << message << std::endl;
  }
}

void Logger::debug(const std::string &message) {
  if (level_ >= logging_level_t::LoggerDebug) {
    std::cout << "[*] " << message << std::endl;
  }
}

} // namespace synapse::runtime
