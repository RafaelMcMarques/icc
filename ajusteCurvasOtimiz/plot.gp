#!/usr/bin/gnuplot -c

#
# 
#   USO: ./plot.gp <arquivo pontos> <eixo y> <nome saida>
#
#

## set encoding iso_8859_15
set encoding utf

# set terminal qt persist

set grid

set style data point
set style function line
set style line 1 lc 3 pt 7 ps 0.3
set boxwidth 1
set xtics
# set xrange [0:]
set xlabel  "log_1_0 N"

#
# ALTERNATIVA 3: Função
#
set style function dots
set ylabel ARG2
set title ARG3


# Gerando figura PNG
set terminal png
set output  ARG3.".png" 
plot ARG1 using 1:2 title "<sem otimização>" with linespoints, \
     '' using 1:3 title "<com otimização>" with linespoints
replot
unset output

