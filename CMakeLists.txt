cmake_minimum_required(VERSION 3.16.3)

project(synapse)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 17)

# lots of warnings (-Werror to treat warnings as errors)
add_compile_options(-Wall -Wextra -pedantic -Wconversion -g)

# https://stackoverflow.com/questions/34571583/understanding-gcc-5s-glibcxx-use-cxx11-abi-or-the-new-abi
# fixme it will be changed in a later release
# add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/exec)

# working directory
set(root_path ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${root_path})

# ##############################################################################
# build the `p4runtime` library
# ##############################################################################

# subdirectory working directory
set(p4runtime_path ${root_path}/p4runtime)

# `pb` stores the .proto and the compiled *.pb.{h, cpp} files
set(p4runtime_pb_path ${p4runtime_path}/pb)
# `grpc` stores the compiled *.grpc.pb.{h, cpp} files
set(p4runtime_grpc_path ${p4runtime_path}/grpc)

# build it
add_subdirectory(${p4runtime_path})

# ##############################################################################
# build the `synapse p4runtime` library
# ##############################################################################

# subdirectory working directory
set(synapse_p4runtime_path ${root_path}/synapse)

# build it
add_subdirectory(${synapse_p4runtime_path})

# ##############################################################################
# build the example controller
# ##############################################################################

# include files in `pb` and `grpc`
include_directories(${p4runtime_pb_path} ${p4runtime_grpc_path})
# build the controller
add_executable(controller ${root_path}/controller.cpp)
# link the synapse p4runtime library
target_link_libraries(controller PUBLIC synapse_p4runtime)
