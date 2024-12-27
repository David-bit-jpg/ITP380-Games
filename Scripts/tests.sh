#!/bin/bash
# Cmake into build directory
# clone tests repo
echo ${LAB_TO_TEST}
git clone "https://github.com/itp380-20241/tests-${LAB_TO_TEST}.git" tests
cd tests
./copyFiles.sh
echo "Compiling..."
CC=clang CXX=clang++ cmake -B build || exit 1
cd build
make || { echo "::error::Code did not compile!"; exit 1; }
cd ..
# Run tests
echo "Running tests..."
timeout 10 build/main -r=github || { echo "::error::Not all tests passed!"; exit 1; }
