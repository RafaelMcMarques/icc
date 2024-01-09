#ifndef AJUSTE_CURVAS_H
#define AJUSTE_CURVAS_H

#include <stdlib.h>
#include "interval-analysis.h"

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

// Monta o sistema linear gerado pelo metodo dos minimos quadrados para ajustar um pol de grau n a k pontos
void montarSistemaAjuste(intervaloDouble *xs, intervaloDouble *ys, long long int n, int k, intervaloDouble **sistemaLinear, intervaloDouble *b);

// calcula a distancia de k yis para f(xi), sendo o f o pol de grau n com coeficiente coef
void calculaResiduo(intervaloDouble *xs, intervaloDouble *ys, long long int n, int k, intervaloDouble *coef, intervaloDouble *residuo);

#endif
