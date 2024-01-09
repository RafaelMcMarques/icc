#!/bin/bash

make

# nomes das tabelas
nomesArquivosSaida=("Memory" "Cache" "Energy" "Flops" "FlopsAVX")

# grupos do likwid
metricas=("MEM" "L2CACHE" "ENERGY" "FLOPS_DP" "FLOPS_DP")

# metricas medidas
colunas=("Memory bandwidth" "L2 miss ratio" "Energy \[J\]" "  DP \[MFLOP/s\]" "AVX DP \[MFLOP/s\]")

#documentar arquitetura
likwid-topology -c -g > "resultados/topology"

# executar testes para cada grupo
for i in {0..4};
do
    echo "Medindo ${nomesArquivosSaida[${i}]}"
    
    # criar tabela da funcao multMatMat para a metrica
    outFileMatMat="resultados/${nomesArquivosSaida[${i}]}MatMat"
    echo "${colunas[${i}]}" > ${outFileMatMat}
    echo "N | semOtimiz | comOtimiz" >> ${outFileMatMat}

    # criar tabela da funcao multMatVet para a metrica
    outFileMatVet="resultados/${nomesArquivosSaida[${i}]}MatVet"
    echo "${colunas[${i}]}" > ${outFileMatVet}
    echo "N | semOtimiz | comOtimiz" >> ${outFileMatVet}


    # executar testes para cada tamanho e escrever na tabela
    for j in 64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000
    do
        echo "N = ${j}"

        # executar likwid
        likwidOut="${metricas[${i}]}${j}.out"
        ./perfctr 3 ${metricas[${i}]} ./matmult $j > "${likwidOut}"

        # achar metricas para multMatMat
        metricaSemOtimiz=$(grep -A 30 "multMatMat_SemOtimiz" "${likwidOut}" | grep "${colunas[${i}]}" | grep -oE ' [0-9]+(\.[0-9]+)?')
        metricaComOtimiz=$(grep -A 30 "multMatMat_Otimiz" "${likwidOut}" | grep "${colunas[${i}]}" | grep -oE ' [0-9]+(\.[0-9]+)?')

        # escrever na tabela da metrica para multMatMat
        echo "${j} | ${metricaSemOtimiz} | ${metricaComOtimiz}" >> ${outFileMatMat}

        # achar metricas para multMatVet
        metricaSemOtimiz=$(grep -A 30 "multMatVet_SemOtimiz" "${likwidOut}" | grep "${colunas[${i}]}" | grep -oE ' [0-9]+(\.[0-9]+)?')
        metricaComOtimiz=$(grep -A 30 "multMatVet_Otimiz" "${likwidOut}" | grep "${colunas[${i}]}" | grep -oE ' [0-9]+(\.[0-9]+)?')

        # escrever na tabela da metrica para multMatVet
        echo "${j} | ${metricaSemOtimiz} | ${metricaComOtimiz}" >> ${outFileMatVet}


        # remover log do likwid
        rm "${likwidOut}"
    done
done

#medir tempo
echo "N | semOtimiz | comOtimiz" > "resultados/TempoMatMat"
echo "N | semOtimiz | comOtimiz" > "resultados/TempoMatVet"
out=a.out

for j in  64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000
do
    
    # executar programa
    ./matmult $j > $out

    # multMatVet
    tempoComOtimiz=$(grep -A 2 "tempo com otimizacao" "${out}"| grep "multMatVet" | grep -oE ' [0-9]+(\.[0-9]+)?')
    tempoSemOtimiz=$(grep -A 2 "tempo sem otimizacao" "${out}"| grep "multMatVet" | grep -oE ' [0-9]+(\.[0-9]+)?')
    echo "${j} |${tempoSemOtimiz} | ${tempoComOtimiz}" >> resultados/TempoMatVet

    # multMatMat
    tempoComOtimiz=$(grep -A 2 "tempo com otimizacao" "${out}"| grep "multMatMat" | grep -oE ' [0-9]+(\.[0-9]+)?')
    tempoSemOtimiz=$(grep -A 2 "tempo sem otimizacao" "${out}"| grep "multMatMat" | grep -oE ' [0-9]+(\.[0-9]+)?')
    echo "${j} |${tempoSemOtimiz} | ${tempoComOtimiz}" >> resultados/TempoMatMat

    rm $out
done

make clean
