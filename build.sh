#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Return to the original directory
cd ..

echo "Build complete. The executable is located at build/bin/newsreader"
