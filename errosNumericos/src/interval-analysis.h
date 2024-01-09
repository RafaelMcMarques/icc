#include <stdlib.h>
#include <math.h>
#include <fenv.h>

typedef union {
    int i;
    float f;
} int_float;

typedef struct {
    int_float min;
    int_float max;
} intervaloFloat;

float calculaErroAbs(intervaloFloat x);

long long calculaULPS(intervaloFloat x);

float calculaErroRel(intervaloFloat x);

intervaloFloat soma(intervaloFloat x, intervaloFloat y);

intervaloFloat subtrai(intervaloFloat x, intervaloFloat y);

intervaloFloat multiplica(intervaloFloat x , intervaloFloat y);

intervaloFloat divide(intervaloFloat x, intervaloFloat y);






