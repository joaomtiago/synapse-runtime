cmake_minimum_required(VERSION 3.16.3)

project(synapse)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 17)

# Lots of warnings (-Werror to treat warnings as errors)
add_compile_options(-Wall -Wextra -pedantic -Wconversion -g3)

# Working directory
set(root_path ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${root_path})

# Set default location for targets in the build directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ##############################################################################
# Build the `p4runtime_cpp` library
# ##############################################################################

# Subdirectory working directory
set(p4runtime_cpp_path ${root_path}/p4runtime_cpp)

# `pb` stores the .proto and the compiled *.pb.{h, cpp} files
set(p4runtime_cpp_pb_path ${p4runtime_cpp_path}/pb)
# `grpc` stores the compiled *.grpc.pb.{h, cpp} files
set(p4runtime_cpp_grpc_path ${p4runtime_cpp_path}/grpc)

# Build the library
add_subdirectory(${p4runtime_cpp_path})

# ##############################################################################
# Build the `synapse_runtime` library
# ##############################################################################

# Subdirectory working directory
set(synapse_runtime_path ${root_path}/runtime)

# Build the library
add_subdirectory(${synapse_runtime_path})

# ##############################################################################
# Build the example `controller`
# ##############################################################################

# Subdirectory working directory
set(controller_path ${root_path}/controller)

# Build the executable
add_subdirectory(${controller_path})
