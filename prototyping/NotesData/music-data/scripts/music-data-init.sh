#!/bin/sh

mkdir -p ./tools/
mkdir -p ./tools/build
rm ./tools/build/* -rf
cd ./tools/build/
cmake ..
cd ./../../

make -C ./tools/build/ clean
