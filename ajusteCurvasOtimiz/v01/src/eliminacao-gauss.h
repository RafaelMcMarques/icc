#ifndef ELIMINACAO_GAUSS_H
#define ELIMINACAO_GAUSS_H

#include <stdlib.h>

#include "interval-analysis.h"

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

// triangulariza a matriz A[n][n] por eliminacao de gauss
void gauss_elim(intervaloDouble **A, intervaloDouble *b, long long int n);

// realiza a retrosubstituicao e guarda os valores de x em x
void retrossubs(intervaloDouble **A, intervaloDouble *b, intervaloDouble *x, long long int n);

#endif
