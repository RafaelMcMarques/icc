#include <stdio.h>
#include <math.h>
#include <fenv.h>

#include "interval-analysis.h"

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

intervaloDouble soma(intervaloDouble x, intervaloDouble y) {
    intervaloDouble result;

    fesetround(FE_DOWNWARD);
    result.min = x.min + y.min;

    fesetround(FE_UPWARD);
    result.max = x.max + y.max;
    
    return result;
}

intervaloDouble subtrai(intervaloDouble x, intervaloDouble y) {
    intervaloDouble result;
    
    fesetround(FE_DOWNWARD);
    result.min = x.min - y.max;

    fesetround(FE_UPWARD);
    result.max = x.max - y.min;

    return result;
}

intervaloDouble multiplica(intervaloDouble x , intervaloDouble y) {
    intervaloDouble result;

    // achar minimo
    fesetround(FE_DOWNWARD);
    double lower_values[4] = {x.min * y.min, x.min * y.max, x.max * y.min, x.max * y.max};
    result.min = lower_values[0];
    for (int i = 1; i < 4; i++) {
        if (lower_values[i] < result.min)
            result.min = lower_values[i];
    }

    // achar maximo
    fesetround(FE_UPWARD);
    double upper_values[4] = {x.min * y.min, x.min * y.max, x.max * y.min, x.max * y.max};
    result.max = upper_values[0];
    for (int i = 1; i < 4; i++) {
        if (upper_values[i] > result.max)
            result.max = upper_values[i];
    }

    return result;
}

intervaloDouble divide(intervaloDouble x, intervaloDouble y) {
    intervaloDouble result;

   if (y.min <= 0.0 && y.max >= 0.0) {
        result.min = -INFINITY;
        result.max = INFINITY;
    } else {
            double min = y.min;
            y.min = 1/y.max;
            y.max = 1/min;
            result = multiplica(x, y); // a,b / c,d = a,b * 1/c,1/b
        }
    
    return result;
}

intervaloDouble potenciaInteira(intervaloDouble x, int p) {
    intervaloDouble result;
    
    // x⁰ = 1
    if (p == 0) {
        result.min = 1.0;
        result.max = 1.0;
    }

    // p eh impar
    else if (p % 2 == 1) {
        result.min = pow(x.min, p);
        result.max = pow(x.max, p);
    }

    // p eh par
    else {
        if (x.min >= 0) {
            result.min = pow(x.min, p);
            result.max = pow(x.max, p);
        }

        else if (x.max < 0) {
            result.min = pow(x.max, p);
            result.max = pow(x.min, p);
        }

        else {
            result.min = 0;
            double a = pow(x.min, p);
            double b = pow(x.max, p);
            result.max = (a > b) ? a : b; 
        }
    }
    return result;
}

void lerIntervalo(intervaloDouble *x) {
    fesetround(FE_DOWNWARD);
    scanf("%lf", &x->min);
    x->max = nextafter(x->min, INFINITY);
}

void imprimirIntervalo(intervaloDouble x) {
    printf("[%lf,%lf] ", x.min, x.max);
}
