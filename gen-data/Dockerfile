FROM ubuntu:18.04

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y build-essential
RUN apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
RUN apt-get install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev

WORKDIR /home/opencv/src/
RUN git clone https://github.com/opencv/opencv.git

WORKDIR /home/opencv/build/
RUN cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local /home/opencv/src/opencv/
RUN make -j7 && make install

WORKDIR /home/opencv/
RUN rm src -rf
RUN rm src build -rf
