#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "interval-analysis.h"

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

void montarSistemaAjuste(intervaloDouble *xs, intervaloDouble *ys, long long int n, long long int k, intervaloDouble **sistemaLinear, intervaloDouble *b) {
    
    // montar sistema linear
    for (long long int i = 0; i < n + 1; i++) {
        for (long long int j = 0; j < n + 1; j++) {
            sistemaLinear[i][j].min = 0.0;
            sistemaLinear[i][j].max = 0.0;
            // SL[i][j] = somatorio de xs^i+j
            for (long long int q = 0; q < k; q++) {
                sistemaLinear[i][j] = soma(sistemaLinear[i][j], potenciaInteira(xs[q], i + j));
            }
        }
    }

    // montar b
    for (long long int i = 0; i < n + 1; i++) {
        b[i].min = 0;
        b[i].max = 0;
        for (long long int j = 0; j < k; j++)
            b[i] = soma(b[i], multiplica(ys[j], potenciaInteira(xs[j], i)));
    }
}

// calcula o residuo em intervalos de min e max
void calculaResiduo(intervaloDouble *xs, intervaloDouble *ys, long long int n, long long int k, intervaloDouble *coef, intervaloDouble *residuo) {
   
    intervaloDouble fxi; 
    for (long long int i = 0; i < k; i++) {
        // calcular f(xi)
        fxi.min = 0.0;
        fxi.max = 0.0;
        for (long long int j = 0; j < n + 1; j++) {
            fxi = soma(fxi, multiplica(coef[j], potenciaInteira(xs[i], j)));
        }
        residuo[i] = subtrai(ys[i], fxi);
    }
}
