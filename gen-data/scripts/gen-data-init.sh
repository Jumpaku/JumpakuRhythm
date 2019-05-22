#!/bin/sh

mkdir -p ./build
rm ./build/* -rf
cd ./build
cmake ..

cp -r /usr/local/include/opencv4/ ./include/