# ##############################################################################
# Build the `synapse_runtime` library
# ##############################################################################

project(synapse_runtime)

# Add compiled P4Runtime files
include_directories(${p4runtime_cpp_pb_path})
include_directories(${p4runtime_cpp_grpc_path})

# Find all .cpp files and compile them
file(GLOB_RECURSE src_files ${synapse_runtime_path}**/*.cpp)
add_library(${PROJECT_NAME} SHARED ${src_files})

# Include the interface header
# set_target_properties(
#   ${PROJECT_NAME}
#   PROPERTIES PUBLIC_HEADER
#              ${synapse_runtime_path}/synapse_p4runtime.h)

# Link the P4Runtime library
target_link_libraries(${PROJECT_NAME} PUBLIC p4runtime_cpp)
