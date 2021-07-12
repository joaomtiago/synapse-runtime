#include "file_utils.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace synapse::p4runtime {

std::string readFile(std::ifstream stream) {
  if (!stream.good()) {
    std::cerr << "[-] could not open file" << std::endl;
    exit(1);
  }

  std::stringstream _buffer;
  _buffer << stream.rdbuf();
  return _buffer.str();
}

std::string readFile(const std::string &filename) {
  return readFile(std::ifstream(filename));
}

std::string readFile(std::string *filename) {
  return readFile(std::ifstream(*filename));
}

}  // namespace synapse::p4runtime
