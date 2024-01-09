#!/bin/bash

METRICA="FLOPS_DP"
CPU=3

make

for k in $METRICA
do
    likwid-perfctr -C ${CPU} -g ${k} -m ./perfEG >${k}_Out.log
done

