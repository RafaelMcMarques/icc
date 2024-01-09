#include <stdlib.h>
#include <stdio.h>

#include "matriz.h"
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
    long long int n, k;
    scanf("%lld %lld", &n, &k);

    // alocar memoria para os pontos
    intervaloDouble *xs = malloc(k * sizeof(intervaloDouble));
    intervaloDouble *ys = malloc(k * sizeof(intervaloDouble));

    // ler pontos
    for (long long int i = 0; i < k; i++) {
        lerIntervalo(&xs[i]);
        lerIntervalo(&ys[i]);
    }

    // inicializa uma nova matriz (vetor de ponteiros) para SL
    Matriz *SL = nova_matriz(n+1, n+1);
    if (!SL) {
        printf("Não foi possível alocar memória para o SL!\n");
        exit(1);
    }

    // aloca com calloc para não precisa inicializar com 0 em montarSistemaAjuste
    intervaloDouble *b = calloc((n + 1) , sizeof(intervaloDouble));
    if (!b) {
        printf("Não foi possível alocar memória para b!");
        exit(1);
    }

    // montar sistema do ajuste polinomial
    double tgeraSL = timestamp();
    LIKWID_MARKER_START("gerar_SL");

    montarSistemaAjuste(xs, ys, n, k, SL, b);

    LIKWID_MARKER_STOP("gerar_SL");
    tgeraSL = timestamp() - tgeraSL;

    // alocar memoria para coeficientes
    intervaloDouble *coef = malloc((n + 1) * sizeof(intervaloDouble));

    // resolver sistema
    double tsolSL = timestamp();
    LIKWID_MARKER_START("resolver_SL");

    gauss_elim(SL, b, n + 1);
    retrossubs(SL, b, coef, n + 1);

    LIKWID_MARKER_STOP("resolver_SL");
    tsolSL = timestamp() - tsolSL;
    
    // calcular residuo
    intervaloDouble *residuo = malloc(k * sizeof(intervaloDouble));
    double tResiduo = timestamp();
    LIKWID_MARKER_START("residuo");

    calculaResiduo(xs, ys, n, k, coef, residuo);

    LIKWID_MARKER_STOP("residuo");
    tResiduo = timestamp() - tResiduo;

    // coeficientes
    printf("\ncoeficientes:\n");
    for (long long int i = 0; i < n + 1; i++) 
        imprimirIntervalo(coef[i]);
    printf("\n\n");

    // tempos
    printf("tgeraSL = %lf\n", tgeraSL);
    printf("tsolSL = %lf\n", tsolSL);
    printf("tResiduo = %lf\n", tResiduo);

    // liberar memoria
    free(xs);
    free(ys);
    free(coef);
    free(residuo);
    free(b);
    matriz_free(&SL);

    LIKWID_MARKER_CLOSE;
}
