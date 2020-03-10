FROM python:3.7

RUN mkdir -p /model
ENV MODEL_PATH /spleeter_model

RUN apt update && apt install -y git libsndfile1 ffmpeg aubio-tools
# python3-pytest-pylint git
RUN pip install autopep8
RUN pip install rope
RUN pip install musdb museval
RUN pip install matplotlib
RUN pip install spleeter==1.4.5
RUN pip install librosa==0.7.1
RUN pip install aubio==0.4.9
RUN mkdir -p /spleeter_model

WORKDIR /home/