#!/bin/sh

mkdir -p ./tools/
mkdir -p ./tools/build
rm ./tools/build/* -rf
cd ./tools/build/
cmake ..
cd ./../../

make -C ./tools/build/ clean

# for external editor's intelligent code completion
mkdir -p ./tools/include
cp -r /usr/local/include/opencv4/ ./tools/include/