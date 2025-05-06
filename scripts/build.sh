#!/bin/bash
BUILD_TYPE=Release
BUILD_DIR="build/$BUILD_TYPE"

cmake -B "$BUILD_DIR" \
	-DCMAKE_TOOLCHAIN_FILE="$BUILD_DIR/generators/conan_toolchain.cmake" \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
	-DCMAKE_BUILD_TYPE="$BUILD_TYPE"

echo "🔨 Building..."
cmake --build "$BUILD_DIR"

echo "🔗 Linking compile_commands.json for clangd..."
ln -sf "$BUILD_DIR/compile_commands.json" ./compile_commands.json

echo "✅ Done."
