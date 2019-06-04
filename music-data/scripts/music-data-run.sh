#!/bin/sh

# Build
make -C ./tools/build/

# Run the following command
./tools/bin/main "../asset/2019-05-23_0753.wav" "./" > output.txt
