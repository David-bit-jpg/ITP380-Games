#!/bin/bash
ls -1 "$1"/*.cpp > clang-format-files.txt
ls -1 "$1"/*.h >> clang-format-files.txt
clang-format -i --files=clang-format-files.txt
