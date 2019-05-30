FROM ubuntu

WORKDIR /home/plot-data
RUN apt update -y && apt upgrade -y && apt install -y gnuplot

CMD ["gnuplot", "plot.plt"]