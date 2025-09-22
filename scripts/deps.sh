#!/usr/bin/env bash
set -euo pipefail

echo "Setting up build dependencies..."

sudo apt-get update

echo "Installing build tools"
sudo apt-get install -y \
    build-essential \
    cmake \
    git

echo "Installing linting tools"
sudo apt-get install -y \
    cppcheck \
    clang-format

echo "Checking versions:"
cmake --version | head -n1
gcc --version | head -n1
cppcheck --version
clang-format --version

echo "Done."