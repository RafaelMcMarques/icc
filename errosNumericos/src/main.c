#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fenv.h>

#include "interval-analysis.h"

#define INPUT_NUM 5

int main() {
    intervaloFloat (*operacao)(intervaloFloat, intervaloFloat);
    intervaloFloat result;
    intervaloFloat x[INPUT_NUM];
    char op[INPUT_NUM - 1];
     
    fesetround(FE_DOWNWARD);

    /* read input */
    int i = 0;
    while (i < INPUT_NUM - 1) {
        scanf("%f %c", &x[i].min.f, &op[i]);
        i++;
    }
    scanf("%f", &x[i].min.f);

    /* get next represantable number towards positive infinity */
    for (i = 0; i < INPUT_NUM; i++) {
        x[i].max.f = nextafterf(x[i].min.f, INFINITY);
    }
    
    result = x[0];
    /* (((X0 O0 X1) O1 X2) O2 X3) O3 X4 */ 
    for (int i = 0; i < INPUT_NUM - 1; i++) {
        printf("%d:\n", i+1);
    
        /* select the respective function */
        switch (op[i]) {
        case '+':
            operacao = soma;
            break;
        case '-':
            operacao = subtrai;
            break;
        case '*':
            operacao = multiplica;
            break;
        case '/':
            operacao = divide;
            break;
        }

        printf("[%1.8e , %1.8e] %c [%1.8e , %1.8e] = ", 
                result.min.f, result.max.f, op[i], x[i+1].min.f, x[i+1].max.f);

        /* update result */
        result = operacao(result, x[i+1]);
        printf("[%1.8e , %1.8e]\n", result.min.f, result.max.f);
        
        /* print absolute error, relative error and ULPs */
        printf("EA: %1.8e; ER: %1.8e; ULPs: %lli\n\n\n",
               calculaErroAbs(result), calculaErroRel(result), calculaULPS(result));
    }

    return 0;    
}


