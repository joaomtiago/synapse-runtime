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

# Link the P4Runtime library
target_link_libraries(${PROJECT_NAME} PUBLIC p4runtime_cpp)

# Install the public headers of the library
install(
  DIRECTORY ${synapse_runtime_path}
  DESTINATION ${root_path}/synapse/include
  FILES_MATCHING
  PATTERN "*.hpp")
