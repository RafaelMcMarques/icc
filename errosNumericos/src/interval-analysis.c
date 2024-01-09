#include "interval-analysis.h"

float calculaErroAbs(intervaloFloat x) {
    return fabs(x.max.f - x.min.f);
}

float calculaErroRel(intervaloFloat x) {
    return fabs(x.max.f - x.min.f) / x.min.f;
}

long long calculaULPS(intervaloFloat x) {
    return abs(x.max.i - x.min.i - 1);
}

intervaloFloat soma(intervaloFloat x, intervaloFloat y) {
    intervaloFloat result;

    fesetround(FE_DOWNWARD);
    result.min.f = x.min.f + y.min.f;

    fesetround(FE_UPWARD);
    result.max.f = x.max.f + y.max.f;
    
    return result;
}

intervaloFloat subtrai(intervaloFloat x, intervaloFloat y) {
    intervaloFloat result;
    
    fesetround(FE_DOWNWARD);
    result.min.f = x.min.f - y.max.f;

    fesetround(FE_UPWARD);
    result.max.f = x.max.f - y.min.f;

    return result;
}

intervaloFloat multiplica(intervaloFloat x , intervaloFloat y) {
    intervaloFloat result;

    fesetround(FE_DOWNWARD);
    float lower_values[4] = {x.min.f * y.min.f, x.min.f * y.max.f, x.max.f * y.min.f, x.max.f * y.max.f};
    result.min.f = lower_values[0];
    for (int i = 1; i < 4; i++) {
        if (lower_values[i] < result.min.f)
            result.min.f = lower_values[i];
    }

    fesetround(FE_UPWARD);
    float upper_values[4] = {x.min.f * y.min.f, x.min.f * y.max.f, x.max.f * y.min.f, x.max.f * y.max.f};
    result.max.f = upper_values[0];
    for (int i = 1; i < 4; i++) {
        if (upper_values[i] > result.max.f)
            result.max.f = upper_values[i];
    }

    return result;
}

intervaloFloat divide(intervaloFloat x, intervaloFloat y) {
    intervaloFloat result;

    if (y.min.f <= 0.0 && y.max.f >= 0.0) {
        result.min.f = -INFINITY;
        result.max.f = INFINITY;
    } else {
        fesetround(FE_DOWNWARD);
        float lower_values[4] = {x.min.f / y.max.f, x.min.f / y.max.f, x.max.f / y.max.f, x.max.f / y.max.f};
        result.min.f = lower_values[0];
        for (int i = 1; i < 4; i++) {
            if (lower_values[i] < result.min.f)
                result.min.f = lower_values[i];
        }

        fesetround(FE_UPWARD);
        float upper_values[4] = {x.min.f / y.max.f, x.min.f / y.max.f, x.max.f / y.max.f, x.max.f / y.max.f};
        result.max.f = upper_values[0];
        for (int i = 1; i < 4; i++) {
            if (upper_values[i] > result.max.f)
                result.max.f = upper_values[i];
        }
    }

    return result;
}

