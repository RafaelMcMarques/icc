#include <stdlib.h>
#include <stdio.h>

#include "ajuste-curvas.h"
#include "interval-analysis.h"
#include "eliminacao-gauss.h"
#include "likwid.h"
#include "utils.h"

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

int main() {
    LIKWID_MARKER_INIT;
    
    // ler grau do polinomio e numero de pontos
    int n, k;
    scanf("%d %d", &n, &k);

    // alocar memoria para os pontos
    intervaloDouble *xs = malloc(k * sizeof(intervaloDouble));
    intervaloDouble *ys = malloc(k * sizeof(intervaloDouble));

    // ler pontos
    for (int i = 0; i < k; i++) {
        lerIntervalo(&xs[i]);
        lerIntervalo(&ys[i]);
    }

    // alocar memoria para o sistema
    intervaloDouble **SL = malloc((n + 1) * sizeof(intervaloDouble *));
    for (int i = 0; i < n + 1; i++)
        SL[i] = malloc((n + 1) * sizeof(intervaloDouble));
    intervaloDouble *b = malloc((n + 1) * sizeof(intervaloDouble));

    // montar sistema do ajuste polinomial
    double tgeraSL = timestamp();
    LIKWID_MARKER_START("gerar SL");

    montarSistemaAjuste(xs, ys, n, k, SL, b);

    LIKWID_MARKER_STOP("gerar SL");
    tgeraSL = timestamp() - tgeraSL;

    // alocar memoria para coeficientes
    intervaloDouble *coef = malloc((n + 1) * sizeof(intervaloDouble));

    // resolver sistema
    double tsolSL = timestamp();
    LIKWID_MARKER_START("resolver SL");

    gauss_elim(SL, b, n + 1);
    retrossubs(SL, b, coef, n + 1);

    LIKWID_MARKER_STOP("resolver SL");
    tsolSL = timestamp() - tsolSL;
    
    // calcular residuo
    intervaloDouble *residuo = malloc(k * sizeof(intervaloDouble));
    calculaResiduo(xs, ys, n, k, coef, residuo);

    // coeficientes
    for (int i = 0; i < n + 1; i++) 
        imprimirIntervalo(coef[i]);
    printf("\n");

    // residuo
    for (int i = 0; i < k; i++) 
        imprimirIntervalo(residuo[i]);
    printf("\n");

    // tempos
    printf("%lf\n", tgeraSL);
    printf("%lf\n", tsolSL);

    // liberar memoria
    free(xs);
    free(ys);
    free(coef);
    free(residuo);
    free(b);
    for (int i = 0; i < n + 1; i++) {
        free(SL[i]);
    }
    free(SL);

    LIKWID_MARKER_CLOSE;
}
