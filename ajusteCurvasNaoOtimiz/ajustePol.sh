#!/bin/bash

make

likwid-perfctr -C 0-4 -g FLOPS_DP -m ./ajustePol < $1 > flops.out
likwid-perfctr -C 0-4 -g ENERGY -m ./ajustePol < $1 > energy.out

# saida do programa
echo
    sed -n '6,9p' energy.out

# metricas para gerar o SL
echo -e "\n\nGERAR SL: "
grep -A 31 "gerar SL" "flops.out" | grep "DP \[MFLOP/s\]"
grep -A 31 "gerar SL" "energy.out" | grep -E "Energy \[J\]"

# metricas para resolver o SL
echo -e "\n\nRESOLVER SL: "
grep -A 31 "resolver SL" "flops.out" | grep "DP \[MFLOP/s\]"
grep -A 31 "resolver SL" "energy.out" | grep -E "Energy \[J\]"

make clean

rm flops.out
rm energy.out
