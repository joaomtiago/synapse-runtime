# ##############################################################################
# build the `synapse p4runtime` library
# ##############################################################################

project(synapse_p4runtime)

# add compiled p4runtime files
include_directories(${p4runtime_pb_path})
include_directories(${p4runtime_grpc_path})

# find all .cpp files and compile them
file(GLOB_RECURSE src_files ${CMAKE_CURRENT_SOURCE_DIR}**/*.cpp)
add_library(${PROJECT_NAME} SHARED ${src_files})

# libp4runtime.hpp contains is the public header of the library
# set_target_properties( ${PROJECT_NAME} PROPERTIES PUBLIC_HEADER
# ${synapse_p4runtime_path}/libp4runtime.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC p4runtime)
