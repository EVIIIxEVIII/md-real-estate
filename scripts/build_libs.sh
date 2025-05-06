#!/bin/bash
set -e

BUILD_TYPE=Release

echo "🔄 Cleaning old build..."
rm -rf build

echo "📦 Installing dependencies with Conan..."
conan install . --build=missing -s build_type=$BUILD_TYPE
