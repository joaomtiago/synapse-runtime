#!/bin/bash

SCRIPT_DIR=$(dirname "$0")

$SCRIPT_DIR/../build/bin/cpp_controller 10.0.2.5:50051 $SCRIPT_DIR/program.p4info.txt $SCRIPT_DIR/program.json
