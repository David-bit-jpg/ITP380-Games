#!/bin/bash
cmake -G Ninja -B Releases -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DCMAKE_OSX_DEPLOYMENT_TARGET=11 || { echo "CMake generation failed"; exit 1; }
cd Releases

cmake --build . --target "$1" || { echo "Failed to build target $1"; exit 1; }

if [ -d "Packages" ]; then
    if [ -d "Packages/$1.app" ]; then
        rm -rf "Packages/$1.app"
    fi
else
    mkdir Packages
fi

mkdir -p "Packages/$1.app/Contents/MacOS" || { echo "Failed to make app bundle $1.app"; exit 1; }
cp "$1/$1" "Packages/$1.app/Contents/MacOS/$1" || { echo "Failed to copy executable to app bundle"; exit 1; }
mkdir -p "Packages/$1.app/Contents/Resources"
if [ -d "$1/Assets" ]; then
    cp -r "$1/Assets" "Packages/$1.app/Contents/Resources/Assets" || { echo "Failed to copy Assets"; exit 1; }
fi
if [ -d "$1/Shaders" ]; then
    cp -r "$1/Shaders" "Packages/$1.app/Contents/Resources/Shaders" || { echo "Failed to copy Shaders"; exit 1; }
fi

echo -e "\xE2\x9C\x85 Succesfully created Releases/Packages/$1.app"
