#!/bin/bash

BUILD_DIR="build"
COMPILE_COMMANDS="compile_commands.json"

# Clean up existing build directory to prevent cross-platform issues
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning up existing build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create fresh build directory
echo "Creating new build directory..."
mkdir "$BUILD_DIR"

# Change to build directory
cd "$BUILD_DIR" || exit 1

# Run CMake commands
echo "Configuring CMake..."
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. || exit 1
echo "Building networking target..."
cmake --build . --target networking || exit 1
echo "Building server target..."
cmake --build . --target server || exit 1
echo "Building client target..."
cmake --build . --target client || exit 1

# Handle compile_commands.json
if [ -f "$COMPILE_COMMANDS" ]; then
    cp "$COMPILE_COMMANDS" ..
    echo "Copied $COMPILE_COMMANDS to the root directory."
else
    echo "Warning: $COMPILE_COMMANDS not found in build directory."
fi

# Return to original directory
cd ..
