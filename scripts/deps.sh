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

echo "Installing Python and Unity test dependencies"
sudo apt-get install -y \
    python3 \
    python3-pip

# Install Unity dependencies 
pip3 install --break-system-packages pyparsing junit-xml

echo "Checking versions:"
cmake --version | head -n1
gcc --version | head -n1
cppcheck --version
clang-format --version
python3 --version
pip3 --version

echo "Done."