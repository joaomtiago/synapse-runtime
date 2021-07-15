# SyNAPSE P4Runtime C/C++ Library
Copyright 2021

# Library Installation
To build the library from source, install these dependencies on the host machine:
1. **Protobuf (Runtime)** — Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data
2. **gRPC C++** — the C++ implementation of gRPC, a high-performance remote procedure call, RPC, framework

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