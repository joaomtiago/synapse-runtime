#include "file_utils.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace synapse::p4runtime {

std::string readFile(const std::string &filename) {
  std::ifstream stream(filename);
  if (!stream.good()) {
    std::cerr << "[-] could not open file `" << filename << "`" << std::endl;
    exit(1);
  }

  return std::string((std::istreambuf_iterator<char>(stream)),
                     (std::istreambuf_iterator<char>()));
}

std::string readFile(std::string *filename) {
  return readFile(static_cast<const std::string>(*filename));
}

}  // namespace synapse::p4runtime
