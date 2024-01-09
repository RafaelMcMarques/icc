#ifndef ELIMINACAO_GAUSS_H
#define ELIMINACAO_GAUSS_H

#include <stdlib.h>

#include "interval-analysis.h"
#include "matriz.h"

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

// triangulariza a matriz A[n][n] por eliminacao de gauss
void gauss_elim(Matriz * restrict A, intervaloDouble * restrict b, long long int n);

// realiza a retrosubstituicao e guarda os valores de x em x
void retrossubs(Matriz * restrict A, intervaloDouble * restrict b, intervaloDouble * restrict x, long long int n);

#endif
