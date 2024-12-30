#!/bin/bash

set -e

BUILD_DIR="build"
BUILD_TYPE="Release"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory: $BUILD_DIR"
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

echo "Configuring the project..."
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE

echo "Building the project..."
cmake --build . --config $BUILD_TYPE

cd ..

echo "Build complete!"
