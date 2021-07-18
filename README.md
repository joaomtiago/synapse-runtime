# SyNAPSE Controller Runtime C/C++ Library
Copyright 2021

# Library installation
To build the library from source, install these dependencies on the host machine:
1. **Protobuf (Runtime)** — Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data
2. **gRPC C++** — the C++ implementation of gRPC, a high-performance remote procedure call (RPC) framework

## Protobuf (Runtime)
Start by installing [these tools](https://github.com/protocolbuffers/protobuf/tree/master/src#c-installation---unix) required by Protobuf, and then build the runtime environment from source:

```shellscript
$ git clone git@github.com:protocolbuffers/protobuf.git --recursive
$ cd protobuf/
$ ./autogen.sh
$ ./configure
$ make
$ sudo make install
$ sudo ldconfig
```

## gRPC C++

```shellscript
$ git clone git@github.com:grpc/grpc --recursive
$ cd grpc/
$ mkdir -p cmake/build
$ cmake ../.. -DgRPC_INSTALL=ON \
              -DCMAKE_BUILD_TYPE=Release \
              -DgRPC_PROTOBUF_PROVIDER=package \
              -DgRPC_ABSL_PROVIDER=module \
              -DBUILD_SHARED_LIBS=ON
$ make
$ sudo make install
$ sudo ldconfig
$ sudo cp -r /home/user/grpc/third_party/abseil-cpp/absl /usr/local/include # abominable
```

<!-- 
# Using the library

## Connector
The term connector is used to describe a client of a target switch.
Under the hood, a connector is a gRPC client of the gRPC server launched by the target switch (thus, we only aim at target switches that implement the P4Runtime).

To create a connector, call its constructor with the following arguments:
- `${grpcAddr}` — the address the gRPC server is listening at (in the format `<host>:<port>`, e.g. `10.0.2.5:50051`)
- `${p4InfoFilepath}` — the (relative or absolute) path to the `*.p4info.txt` file generated by the P4 compiler (this file contains metadata about the compiled P4 program, like actions, tables, register, etc.)

Example:
```c++
auto connector = new synapse::runtime::Connector(grpcAddr, p4InfoFilepath);
```
-->

<!-- todo does the connector start automatically? -->
<!-- todo what needs to be done? how to start it? -->
<!-- todo what's happening behind the curtain? -->