#ifndef SYNAPSE_RUNTIME_UTILS_FILE_HPP_
#define SYNAPSE_RUNTIME_UTILS_FILE_HPP_

#include <string>

namespace synapse::runtime {

std::string readFile(const std::string &filename);

std::string readFile(std::string *filename);

} // namespace synapse::runtime

#endif // SYNAPSE_RUNTIME_UTILS_FILE_HPP_