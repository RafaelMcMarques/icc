#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdio.h>

#include "interval-analysis.h"

typedef struct Matriz {
    //long long int n
    intervaloDouble* end;
    intervaloDouble** dado;
} Matriz;

/* inicializa Matriz, um vetor de ponteiros para intervaloDouble */
Matriz* nova_matriz(long long int lin, long long int col);

/* free na struct Matriz */
void matriz_free(Matriz** matriz);

#endif
