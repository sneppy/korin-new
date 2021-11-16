#!/bin/bash

# Print information on envirornment
gcc --version
g++ --version
cmake --version

# Create build directory
mkdir build

# Configure project
pushd build
cmake .. "$@"
popd
