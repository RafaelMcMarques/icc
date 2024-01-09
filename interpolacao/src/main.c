#include <stdio.h>
#include <stdlib.h>

#include "newtonInterpolation.h"
#include "lagrangeInterpolation.h"
#include "utils.h"
#include "likwid.h"

int main(int argc, char *argv[]) {
    
    // get argc and n
    if (argc != 2)
        exit(1);
    double xe = atof(argv[1]);
    int n;
    scanf("%d", &n);
    
    // allocate memory
    double *knownXs = malloc(n * sizeof(double));
    double *knownYs = malloc(n * sizeof(double));
    if (!knownXs || !knownYs) 
        exit(1);

    // read input 
    for (int i = 0; i < n; i++) {
        scanf("%lf", &knownXs[i]);
        scanf("%lf", &knownYs[i]);
    }

    // check if xe belongs to x interval
    double min = knownXs[0];
    double max = knownXs[0];
    for (int i = 0; i < n; i++) {
        if (knownXs[i] > max)
            max = knownXs[i];
        if (knownXs[i] < min)
            min = knownXs[i];
    }
    if (xe > max || xe < min) {
        fprintf(stderr, "Ponto fora do intervalo\n");
        free(knownXs);
        free(knownYs);
        exit(1);
    }


    // perform interpolation
    LIKWID_MARKER_INIT;

    LIKWID_MARKER_START("Lagrange");
    double tempoLagrange = timestamp();
    double yeLagrange = lagrangeInterpolation(xe, knownXs, knownYs, n);
    tempoLagrange = timestamp() - tempoLagrange;
    LIKWID_MARKER_STOP("Lagrange");

    LIKWID_MARKER_START("Newton");
    double tempoNewton = timestamp();
    double yeNewton = newtonInterpolation(xe, knownXs, knownYs, n);
    tempoNewton = timestamp() - tempoNewton;
    LIKWID_MARKER_STOP("Newton");

    LIKWID_MARKER_CLOSE;

    // print output and free memory
    printf("%lf\n%lf\n%lf\n%lf\n", yeLagrange, yeNewton, tempoLagrange, tempoNewton);
    free(knownXs);
    free(knownYs);
    return 0;
}
