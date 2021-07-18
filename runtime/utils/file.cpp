#include "file.hpp"
#include "runtime/utils/runtime_exception.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace synapse::runtime {

std::string readFile(const std::string &filename) {
  std::ifstream stream(filename);
  if (!stream.good()) {
    throw new RuntimeException("Could not open file `" + filename + "`");
  }

  return std::string((std::istreambuf_iterator<char>(stream)),
                     (std::istreambuf_iterator<char>()));
}

std::string readFile(std::string *filename) {
  return readFile(static_cast<const std::string>(*filename));
}

} // namespace synapse::runtime
