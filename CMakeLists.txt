cmake_minimum_required(VERSION 3.16.3)

project(synapse)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 17)

# Lots of warnings (-Werror to treat warnings as errors).
add_compile_options(-Wall -Wextra -pedantic -Wconversion -g3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/exec)

# Working directory.
set(root_path ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${root_path})

# ##############################################################################
# Build the `p4runtime` library.
# ##############################################################################

# Subdirectory working directory.
set(p4runtime_path ${root_path}/p4runtime)

# `pb` stores the .proto and the compiled *.pb.{h, cpp} files
set(p4runtime_pb_path ${p4runtime_path}/pb)
# `grpc` stores the compiled *.grpc.pb.{h, cpp} files
set(p4runtime_grpc_path ${p4runtime_path}/grpc)

# Build the library.
add_subdirectory(${p4runtime_path})

# ##############################################################################
# Build the `synapse_p4runtime` library.
# ##############################################################################

# Subdirectory working directory.
set(synapse_p4runtime_path ${root_path}/synapse)

# Build the library.
add_subdirectory(${synapse_p4runtime_path})

# Install the public headers of the library
install(
  DIRECTORY ${synapse_p4runtime_path}
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "*.hpp"
  PATTERN "*.hpp"
  PATTERN "synapse_p4runtime.h" EXCLUDE)

# ##############################################################################
# Build the example controller.
# ##############################################################################

# Include files in `pb` and `grpc`.
include_directories(${p4runtime_pb_path} ${p4runtime_grpc_path})
# Build the example controller.
add_executable(controller ${root_path}/controller.cpp)
# Link the SyNAPSE P4Runtime library.
target_link_libraries(controller PUBLIC synapse_p4runtime)
