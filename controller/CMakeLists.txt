# ##############################################################################
# Build the example `controller`.
# ##############################################################################

# Include files in `pb` and `grpc`.
include_directories(${p4runtime_pb_path} ${p4runtime_grpc_path})

# Build the example controller.
add_executable(controller ${controller_path}/controller.cpp)

# Link the SyNAPSE P4Runtime library.
target_link_libraries(controller PUBLIC synapse_p4runtime)
