#ifndef SYNAPSE_UTILS_FILE_UTILS_H_
#define SYNAPSE_UTILS_FILE_UTILS_H_

#include <string>

namespace synapse::p4runtime {

std::string readFile(const std::string &filename);

std::string readFile(std::string *filename);

}  // namespace synapse::p4runtime

#endif  // SYNAPSE_UTILS_FILE_UTILS_H_