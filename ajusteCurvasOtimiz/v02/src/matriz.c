#include <stdlib.h>

#include "matriz.h"

/* inicializa Matriz, um vetor de ponteiros para intervaloDouble */
Matriz* nova_matriz(long long int lin, long long int col) {
    Matriz* matriz = malloc(sizeof(*matriz));
    if (!matriz)
        return NULL;

    matriz->dado = malloc(lin * sizeof(*matriz->dado));
    if (!matriz->dado) {
        free(matriz);
        return NULL;
    }

    /* inicia com 0.0 para economizar atribuições futuras */
    matriz->dado[0] = calloc(lin * col, sizeof(*matriz->dado[0]));
    if (!matriz->dado[0]) {
        free(matriz->dado);
        free(matriz);
        return NULL;
    }

    /* salva o endereço inicial para realizar o free */
    matriz->end = matriz->dado[0];

    /* atribui o endereço de cada linha */
    for (long long int i = 1; i < lin; i++) {
        matriz->dado[i] = matriz->dado[i - 1] + col;
    }

    printf("%lld\n", lin);

    return matriz;
}

/* free na struct Matriz */
void matriz_free(Matriz** matriz) {
    
    if (*matriz) {
        free((*matriz)->end);
        free((*matriz)->dado);
        free(*matriz);
        *matriz = NULL;
    }
}
