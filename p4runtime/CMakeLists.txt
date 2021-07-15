# ##############################################################################
# Build the `p4runtime` library.
# ##############################################################################

project(p4runtime)

# Include `threads`.
find_package(Threads REQUIRED)

# Include `Protobuf`.
find_package(Protobuf REQUIRED)

# Include `gRPC`.
find_package(gRPC CONFIG REQUIRED)
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

# Include absl. find_package(absl REQUIRED)

# p4runtime uses proto files from the googleapis, which also need to be compiled
set(googleapis_path "/home/user/googleapis")

# Make directories `p4runtime_pb_path` and `p4runtime_grpc_path` at build time.
add_custom_target(
  directories ALL COMMAND ${CMAKE_COMMAND} -E make_directory
                          ${p4runtime_pb_path} ${p4runtime_grpc_path})

# Protobuf files serve two purposes: (1) declare the argument and return data
# structures (messages) used by RPC calls (i.4. P4Runtime data structures), and
# (2) specify what functionality the gRPC server exposes to its clients.
set(protoc_pb_args --cpp_out ${p4runtime_pb_path} "-I" ${googleapis_path} "-I"
                   ${p4runtime_pb_path})
set(protoc_grpc_args
    --grpc_out
    ${p4runtime_grpc_path}
    "-I"
    ${googleapis_path}
    "-I"
    ${p4runtime_pb_path}
    --plugin=protoc-gen-grpc=${grpc_cpp_plugin_location})

# Function that compiles a proto file.
function(compile_proto output_files proto_relative_path
         proto_parent_absolute_path)
  # Protobuf output.
  set(pb_out ${p4runtime_pb_path}/${proto_relative_path}.pb.h
             ${p4runtime_pb_path}/${proto_relative_path}.pb.cc)

  # gRPC output.
  set(grpc_out ${p4runtime_grpc_path}/${proto_relative_path}.grpc.pb.h
               ${p4runtime_grpc_path}/${proto_relative_path}.grpc.pb.cc)

  # Append compiled files to `output_files` set.
  set(${output_files}
      ${${output_files}} ${pb_out} ${grpc_out}
      PARENT_SCOPE)

  # Alias for the proto file.
  set(proto_file ${proto_parent_absolute_path}/${proto_relative_path}.proto)

  add_custom_command(OUTPUT ${pb_out} COMMAND ${Protobuf_PROTOC_EXECUTABLE}
                                              ${protoc_pb_args} ${proto_file})

  add_custom_command(
    OUTPUT ${grpc_out} COMMAND ${Protobuf_PROTOC_EXECUTABLE}
                               ${protoc_grpc_args} ${proto_file})
endfunction()

# `protoc_output` stores the result of calling protoc on files below.
set(protoc_output)
compile_proto(protoc_output "p4/config/v1/p4info" ${p4runtime_pb_path})
compile_proto(protoc_output "p4/config/v1/p4types" ${p4runtime_pb_path})
compile_proto(protoc_output "p4/v1/p4data" ${p4runtime_pb_path})
compile_proto(protoc_output "p4/v1/p4runtime" ${p4runtime_pb_path})
compile_proto(protoc_output "google/rpc/status" ${googleapis_path})

# Include additional header files, and the P4Runtime files.
include_directories(${Protobuf_INCLUDE_DIRS} ${p4runtime_pb_path}
                    ${p4runtime_grpc_path})

# Create the P4Runtime library (generates lib/libp4runtime.so).
add_library(${PROJECT_NAME} SHARED ${protoc_output})

# Link the Protobuf and the gRPC libraries.
target_link_libraries(
  ${PROJECT_NAME} gRPC::grpc++_reflection gRPC::grpc++ ${Protobuf_LIBRARIES}
  ${Protobuf_LIBRARY_DEBUG} ${Protobuf_PROTOC_LIBRARY_DEBUG})

# Make sure output directories are built.
add_dependencies(${PROJECT_NAME} directories)

# Install the library to `/usr/local/lib`.
install(TARGETS ${PROJECT_NAME})
