# ##############################################################################
# Build the example `controller`
# ##############################################################################

project(controller)

# Include files in `pb` and `grpc`
include_directories(${p4runtime_cpp_pb_path} ${p4runtime_cpp_grpc_path})

# Generate two empty controllers
set(cpp_controller ${PROJECT_NAME}_cpp)
set(c_controller ${PROJECT_NAME}_c)

# Build the example controller
add_executable(cpp_controller ${controller_path}/controller.cpp)
add_executable(c_controller ${controller_path}/controller.c)

# Link the SyNAPSE P4Runtime library
target_link_libraries(cpp_controller PUBLIC synapse_runtime)
target_link_libraries(c_controller PUBLIC synapse_runtime)
