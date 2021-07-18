# ##############################################################################
# Build the example `controller`
# ##############################################################################

project(controller)

# Include files in `pb` and `grpc`
include_directories(${p4runtime_cpp_pb_path} ${p4runtime_cpp_grpc_path})

# Build the example controller
add_executable(${PROJECT_NAME} ${controller_path}/controller.cpp)

# Link the SyNAPSE P4Runtime library
target_link_libraries(${PROJECT_NAME} PUBLIC synapse_runtime)
