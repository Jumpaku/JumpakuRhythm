set terminal pdfcairo size 100.0,2.0
set output 'plot.pdf'
set xlabel 'time [msec]'
set ylabel 'exists'
set xrange [0:125000]
set xtics 1000
#set yrange [0:1]
#set samples 500
plot "output.txt" with lines
