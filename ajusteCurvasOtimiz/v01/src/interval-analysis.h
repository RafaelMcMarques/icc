#ifndef INTERVAL_ANALYSIS_H
#define INTERVAL_ANALYSIS_H

#include <stdlib.h>

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

typedef struct {
    double min;
    double max;
} intervaloDouble;

intervaloDouble soma(intervaloDouble x, intervaloDouble y);

intervaloDouble subtrai(intervaloDouble x, intervaloDouble y);

intervaloDouble multiplica(intervaloDouble x , intervaloDouble y);

intervaloDouble divide(intervaloDouble x, intervaloDouble y);

intervaloDouble potenciaInteira(intervaloDouble x, int y);

void lerIntervalo(intervaloDouble *x);

void imprimirIntervalo(intervaloDouble x);


#endif



