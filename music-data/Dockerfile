FROM python:3.7

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y build-essential ffmpeg

WORKDIR /home/music-data/

COPY ./requirements.txt /home/music-data/
RUN pip install -r requirements.txt 

CMD [ "sleep", "infinity" ]
