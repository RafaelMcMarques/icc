#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "interval-analysis.h"

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

void montarSistemaAjuste(intervaloDouble restrict *xs, intervaloDouble restrict *ys, int n, int k, intervaloDouble restrict **sistemaLinear, intervaloDouble restrict *b) {
    
    // s eh o numero de variaveis do sistema
    int s = n + 1;

    // calcular apenas primeira linha e ultima coluna do sistema
    sistemaLinear[0][0].min = k; // SL[0][0] = somatorio de xs⁰ 
    sistemaLinear[0][0].max = k;
    for (int i = 1; i < s; i++) {
        sistemaLinear[0][i].min = 0.0;
        sistemaLinear[0][i].max = 0.0;
        sistemaLinear[i][s - 1].min = 0.0;
        sistemaLinear[i][s - 1].max = 0.0;
        // SL[i][j] = somatorio de xs^i+j
        for (int j = 0; j < k; j++) {
            sistemaLinear[0][i] = soma(sistemaLinear[0][i], potenciaInteira(xs[j], i));
            sistemaLinear[i][s - 1] = soma(sistemaLinear[i][s - 1], potenciaInteira(xs[j], s + i - 1));
        }
    }

    // montar resto da matriz sabendo que ela eh simetrica
    // i.e. copiar elemento da diagonal superior diretia
    for (int i = 1; i < s; i++) {
        for (int j = 0; j < s - 1; j++) {
            sistemaLinear[i][j].min = sistemaLinear[i - 1][j + 1].min;
            sistemaLinear[i][j].max = sistemaLinear[i - 1][j + 1].max;
        }
    }

    // montar b
    for (int i = 0; i < s; i++) {
        b[i].min = 0;
        b[i].max = 0;
        for (int j = 0; j < k; j++)
            b[i] = soma(b[i], multiplica(ys[j], potenciaInteira(xs[j], i)));
    }
}

// calcula o residuo em intervalos de min e max
void calculaResiduo(intervaloDouble *xs, intervaloDouble *ys, int n, int k, intervaloDouble *coef, intervaloDouble *residuo) {
   
    intervaloDouble fxi; 
    for (int i = 0; i < k; i++) {
        // calcular f(xi)
        fxi.min = 0.0;
        fxi.max = 0.0;
        for (int j = 0; j < n + 1; j++) {
            fxi = soma(fxi, multiplica(coef[j], potenciaInteira(xs[i], j)));
        }
        residuo[i] = subtrai(ys[i], fxi);
    }
}
