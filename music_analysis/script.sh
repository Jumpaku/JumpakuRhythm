#!/bin/bash

FILE="asset/2019-05-23_0753.wav"
#FILE="asset/crow_song.mp3"
#FILE="asset/dakedo_baby.m4a"
#FILE="asset/ジャージ部魂!.mp3"
#FILE="asset/Kiri.mp3"
#FILE="asset/totsugeki_love_heart.m4a"
#FILE="asset/暁の車.mp3"

(
    echo "duration" && \
    (ffprobe -show_streams:m:duration -v error "$FILE" | grep "duration=" | sed s/duration=//) && \
    echo "sample_rate" && \
    (ffprobe -show_streams:m:sample_rate -v error "$FILE" |grep "sample_rate=" | sed s/sample_rate=//) && \
    echo "onset" && \
    (cat "$FILE" | aubioonset -i /dev/stdin) && \
    echo "tempo" && \
    (cat "$FILE" | aubiotrack -i /dev/stdin) \
) | python main.py