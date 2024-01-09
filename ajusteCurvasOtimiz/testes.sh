#!/bin/bash

# ============================================ # PARAMETROS # ============================================ #

# Versoes do programa
VERSOES=("v01" "v02")
VERSOES=$VERSOES

# Diretorio que contem os resultados
OUTPUT='resultados'

# Numero de pontos a serem testados
PONTOS='64 128 200 256 512 600 800 1024 2000 3000 4096 6000 7000 10000 50000 100000 1000000 10000000 100000000'

# Nomes das tabelas
ARQUIVOS_SAIDA=("Memory" "Cache" "Flops" "FlopsAVX" "Energy")

# Grupos do likwid 
METRICAS=("L3" "L2CACHE" "FLOPS_DP" "FLOPS_DP" "ENERGY")

# METRICAS medidas
COLUNAS=("L3 bandwidth" "L2 miss ratio" "  DP [MFLOP/s]" "AVX DP [MFLOP/s]" "Energy") 

# LIKWID = 0 -> gera tempo e coeficientes; LIKWID = 1 -> faz a execução com likwid para cada métrica acima
LIKWID=1

# Modo performance
echo performance > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

# =========================================== # COMPILAÇÃO # =========================================== # 

# Cria o diretorio dos resultados
mkdir -p $OUTPUT

# Compila as versoes do programa
echo 
echo "Compilando os programas"
echo

# Compila os programas
if [ -e "gera_entrada.c" ]; then
    gcc gera_entrada.c -o gera_entrada
else
    echo "O arquivo gera_entrada.c não existe."
fi

make -C ${VERSOES[0]} 
make -C ${VERSOES[1]} 

# Documentar arquitetura
echo 
echo "Documentando arquitetura!"
echo

likwid-topology -c -g > "${OUTPUT}/topology.out"

echo 
echo "Compilação finalizada!"
echo
echo "Gerando ${OUTPUT}/tempo_*.out e ${OUTPUT}/coeficientes.out"
echo

# ======================================= # TEMPO E COEFICIENTES # ======================================= #

echo "" > "${OUTPUT}/tempo_geraSL.out"
echo "" > "${OUTPUT}/tempo_solSL.out"
echo "" > "${OUTPUT}/tempo_residuo.out"

for p in $PONTOS; do
    n=$(echo "l(${p})/l(10)" | bc -l)

    # Executar programa sem otimização
    echo "./gera_entrada $p | ./"${VERSOES[0]}"/ajustePol > out1"
    ./gera_entrada $p | ./"${VERSOES[0]}"/ajustePol > out1


    # Executar programa com otimização
    echo "./gera_entrada $p | ./"${VERSOES[1]}"/ajustePol > out2"
    ./gera_entrada $p | ./"${VERSOES[1]}"/ajustePol > out2


    # geraSL
    tempoSemOtimiz=$(grep "tgeraSL" out1 | grep -oE ' [0-9]+(\.[0-9]+)?')
    tempoComOtimiz=$(grep "tgeraSL" out2 | grep -oE ' [0-9]+(\.[0-9]+)?')
    tempoSemOtimiz=$(echo "l(${tempoSemOtimiz})/l(10)" | bc -l)
    tempoComOtimiz=$(echo "l(${tempoComOtimiz})/l(10)" | bc -l)
    echo "${n} ${tempoSemOtimiz} ${tempoComOtimiz}" >> ${OUTPUT}/tempo_geraSL.out

    # solSL
    tempoSemOtimiz=$(grep "tsolSL" out1 | grep -oE ' [0-9]+(\.[0-9]+)?')
    tempoComOtimiz=$(grep "tsolSL" out2 | grep -oE ' [0-9]+(\.[0-9]+)?')
    tempoSemOtimiz=$(echo "l(${tempoSemOtimiz})/l(10)" | bc -l)
    tempoComOtimiz=$(echo "l(${tempoComOtimiz})/l(10)" | bc -l)
    echo "${n} ${tempoSemOtimiz} ${tempoComOtimiz}" >> ${OUTPUT}/tempo_solSL.out

    # residuo
    tempoSemOtimiz=$(grep "tResiduo" out1 | grep -oE ' [0-9]+(\.[0-9]+)?')
    tempoComOtimiz=$(grep "tResiduo" out2 | grep -oE ' [0-9]+(\.[0-9]+)?')
    tempoSemOtimiz=$(echo "l(${tempoSemOtimiz})/l(10)" | bc -l)
    tempoComOtimiz=$(echo "l(${tempoComOtimiz})/l(10)" | bc -l)
    echo "${n} ${tempoSemOtimiz} ${tempoComOtimiz}" >> ${OUTPUT}/tempo_residuo.out

done

echo "plotando graficos"
cd ${OUTPUT}
./../plot.gp tempo_residuo.out log_1_0Tempo TempoResiduo
./../plot.gp tempo_geraSL.out log_1_0Tempo TempoGeraSL
./../plot.gp tempo_solSL.out log_1_0Tempo TempoSolSL
cd ..

rm out1
rm out2

# =========================================== # LIKWID # =========================================== #

# Faz os testes de benchmark se o parâmetro LIKWID estiver setado como 1
if [ "$LIKWID" -eq 1 ]; then

echo
echo "Executando os testes para cada grupo!"
echo

# Executar testes para cada grupo
for i in {0..4}; do

    echo
    echo "Medindo ${ARQUIVOS_SAIDA[${i}]}"
    echo

    # Criar tabela de geraSL para a metrica
    outFileGeraSL="${OUTPUT}/geraSL_${ARQUIVOS_SAIDA[${i}]}.out"
    echo "" > ${outFileGeraSL}

    # Criar tabela de solSL para a metrica
    outFileSolSL="${OUTPUT}/solSL_${ARQUIVOS_SAIDA[${i}]}.out"
    echo "" > ${outFileSolSL}

    # Criar tabela de residuo para a metrica
    outFileResiduo="${OUTPUT}/residuo_${ARQUIVOS_SAIDA[${i}]}.out"
    echo "" > ${outFileResiduo}

    # Executar testes para cada tamanho e escrever na tabela
    for p in $PONTOS
    do
        echo
        echo "N = ${p}"
        echo

        n=$(echo "l(${p})/l(10)" | bc -l)

        # Executar likwid para a versão não otimizada
        likwidOut1="${METRICAS[${i}]}${p}1.out"
        echo "./gera_entrada $p | likwid-perfctr -C 3 -g ${METRICAS[${i}]} -m ./"${VERSOES[0]}"/ajustePol > "${likwidOut1}""
        ./gera_entrada $p | likwid-perfctr -C 3 -g ${METRICAS[${i}]} -m ./"${VERSOES[0]}"/ajustePol > "${likwidOut1}"

        # Executar o likwid para a versão otimizada
        likwidOut2="${METRICAS[${i}]}${p}2.out"
        echo "./gera_entrada $p | likwid-perfctr -C 3 -g ${METRICAS[${i}]} -m ./"${VERSOES[1]}"/ajustePol > "${likwidOut2}""
        ./gera_entrada $p | likwid-perfctr -C 3 -g ${METRICAS[${i}]} -m ./"${VERSOES[1]}"/ajustePol > "${likwidOut2}"

        # Achar METRICAS para gerar_SL
        metricaSemOtimiz=$(grep -A 30 "gerar_SL" "${likwidOut1}" | grep "${COLUNAS[${i}]}" | grep -oE ' [0-9]+(\.[0-9]+)?')
        metricaComOtimiz=$(grep -A 30 "gerar_SL" "${likwidOut2}" | grep "${COLUNAS[${i}]}" | grep -oE ' [0-9]+(\.[0-9]+)?')

        # Escrever na tabela da metrica para gerar_SL
        echo "${n} ${metricaSemOtimiz} ${metricaComOtimiz}" >> ${outFileGeraSL}

        # Achar METRICAS para resolver_SL
        metricaSemOtimiz=$(grep -A 30 "resolver_SL" "${likwidOut1}" | grep "${COLUNAS[${i}]}" | grep -oE ' [0-9]+(\.[0-9]+)?')
        metricaComOtimiz=$(grep -A 30 "resolver_SL" "${likwidOut2}" | grep "${COLUNAS[${i}]}" | grep -oE ' [0-9]+(\.[0-9]+)?')

        # Escrever na tabela da metrica para resolver_SL
        echo "${n} ${metricaSemOtimiz} ${metricaComOtimiz}" >> ${outFileSolSL}

        # Achar METRICAS para residuo
        metricaSemOtimiz=$(grep -A 30 "residuo" "${likwidOut1}" | grep "${COLUNAS[${i}]}" | grep -oE ' [0-9]+(\.[0-9]+)?')
        metricaComOtimiz=$(grep -A 30 "residuo" "${likwidOut2}" | grep "${COLUNAS[${i}]}" | grep -oE ' [0-9]+(\.[0-9]+)?')

        # Escrever na tabela da metrica para residuo
        echo "${n} ${metricaSemOtimiz} ${metricaComOtimiz}" >> ${outFileResiduo}

        # Remover log do likwid
        rm "${likwidOut1}"
        rm "${likwidOut2}"
    done
    echo ""
    echo "plotando graficos"
    echo ""
    cd ${OUTPUT}
    ./../plot.gp "../${outFileResiduo}" "${COLUNAS[${i}]}" "${ARQUIVOS_SAIDA[${i}]}Residuo"
    ./../plot.gp "../${outFileGeraSL}" "${COLUNAS[${i}]}" "${ARQUIVOS_SAIDA[${i}]}GeraSL"
    ./../plot.gp "../${outFileSolSL}" "${COLUNAS[${i}]}" "${ARQUIVOS_SAIDA[${i}]}SolSL"
    cd ..
done

echo 
echo "Pronto!"
echo 

fi

echo
echo "Make clean..."
echo

# Make clean
make -C ${VERSOES[0]} clean
make -C ${VERSOES[1]} clean
rm gera_entrada

echo powersave > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

echo
echo "Testes finalizados!"
echo
