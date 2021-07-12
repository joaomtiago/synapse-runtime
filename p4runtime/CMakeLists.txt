# ##############################################################################
# build the `p4runtime` library
# ##############################################################################

project(p4runtime)

# include protobuf
find_package(Protobuf REQUIRED)

# include absl
find_package(absl REQUIRED)

# include grpc
find_package(gRPC REQUIRED)
# where can we find the grpc_cpp_plugin?
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

# p4runtime uses proto files from the googleapis, which also need to be compiled
set(googleapis_path "/home/user/googleapis")

# build `p4runtime_pb_path`  and `p4runtime_grpc_path` at build time
add_custom_target(directories ALL COMMAND ${CMAKE_COMMAND} -E make_directory
                                          ${p4runtime_pb_path} ${p4runtime_grpc_path})

# protobuf files serve two purposes: (1) declare the argument and return data
# structures (messages) used by rpc calls (i.4. p4runtime data structures), and
# (2) specify what functionality the grpc server exposes to its clients
set(protoc_pb_args --cpp_out ${p4runtime_pb_path} "-I" ${googleapis_path} "-I" ${p4runtime_pb_path})
set(protoc_grpc_args
    --grpc_out
    ${p4runtime_grpc_path}
    "-I"
    ${googleapis_path}
    "-I"
    ${p4runtime_pb_path}
    --plugin=protoc-gen-grpc=${grpc_cpp_plugin_location})

# function that compiles a proto file
function(compile_proto output_files proto_relative_path
         proto_parent_absolute_path)
  # pb output
  set(pb_out ${p4runtime_pb_path}/${proto_relative_path}.pb.h
             ${p4runtime_pb_path}/${proto_relative_path}.pb.cc)

  # grpc output
  set(grpc_out ${p4runtime_grpc_path}/${proto_relative_path}.grpc.pb.h
               ${p4runtime_grpc_path}/${proto_relative_path}.grpc.pb.cc)

  # add compiled files to `output_files` set
  set(${output_files}
      ${${output_files}} ${pb_out} ${grpc_out}
      PARENT_SCOPE)

  # alias for the proto file
  set(proto_file ${proto_parent_absolute_path}/${proto_relative_path}.proto)

  add_custom_command(OUTPUT ${pb_out} COMMAND ${Protobuf_PROTOC_EXECUTABLE}
                                              ${protoc_pb_args} ${proto_file})

  add_custom_command(
    OUTPUT ${grpc_out} COMMAND ${Protobuf_PROTOC_EXECUTABLE}
                               ${protoc_grpc_args} ${proto_file})
endfunction()

# `protoc_output` stores the result of calling protoc on files below
set(protoc_output)
compile_proto(protoc_output "p4/config/v1/p4info" ${p4runtime_pb_path})
compile_proto(protoc_output "p4/config/v1/p4types" ${p4runtime_pb_path})
compile_proto(protoc_output "p4/v1/p4data" ${p4runtime_pb_path})
compile_proto(protoc_output "p4/v1/p4runtime" ${p4runtime_pb_path})
compile_proto(protoc_output "google/rpc/status" ${googleapis_path})

# additional protobuf .h files can be found here (set by the
# `find_package(Protobuf REQUIRED)`)
include_directories(${Protobuf_INCLUDE_DIRS})
# also include the recently compiled p4runtime files
include_directories(${p4runtime_pb_path})
include_directories(${p4runtime_grpc_path})

# create the p4runtime library (generates lib/libp4runtime.so)
add_library(${PROJECT_NAME} SHARED ${protoc_output})
# link the protobuf and the grpc libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${Protobuf_LIBRARIES} gRPC::grpc++)
# make sure output directories are built
add_dependencies(${PROJECT_NAME} directories)
