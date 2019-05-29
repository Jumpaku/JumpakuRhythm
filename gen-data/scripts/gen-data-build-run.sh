#!/bin/sh

# Build
make -C ./tools/build/

# Run the following command
./tools/bin/extract_notes "./asset/2019-05-23_0753.mp4" "./asset/frames_2019-05-23_0753" > output.txt
