FROM ubuntu:18.04

RUN apt-get update -y && apt-get upgrade -y && \
    apt-get install -y build-essential && \
    apt-get install -y cmake git libopenblas-dev liblapack-dev libarpack2-dev

WORKDIR /home/armadillo/
RUN git clone https://gitlab.com/conradsnicta/armadillo-code.git
WORKDIR /home/armadillo/armadillo-code
RUN cmake . && make -j7 && make install

WORKDIR /home/
RUN rm armadillo/ -rf

WORKDIR /home/
RUN git clone https://github.com/audionamix/wave.git
WORKDIR /home/wave/
RUN cmake . && make

WORKDIR /home/music-data/

CMD [ "sleep", "infinity" ]
