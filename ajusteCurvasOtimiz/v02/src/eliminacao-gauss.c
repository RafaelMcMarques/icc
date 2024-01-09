#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "interval-analysis.h"
#include "matriz.h"

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

// acha maior elemento da coluna
long long int find_max(Matriz *A, long long int col, long long int n) {
    long long int max_row = col;
    for (long long int i = col + 1; i < n; i++) {
        if (fabs(A->dado[i][col].max) > fabs(A->dado[max_row][col].max)) {
            max_row = i;
        }
    }
    return max_row;
}

// troca duas linhas
void swap_line(Matriz * restrict A, intervaloDouble * restrict b, long long int row1, long long int row2, long long int n) {
    intervaloDouble temp1 = b[row1];
    b[row1] = b[row2];
    b[row2] = temp1;

    intervaloDouble *temp2 = A->dado[row1];
    A->dado[row1] = A->dado[row2];
    A->dado[row2] = temp2;
}

// triangulariza a matriz A[n][n] por eliminacao de gauss
void gauss_elim(Matriz * restrict A, intervaloDouble * restrict b, long long int n) {
    for (long long int i = 0; i < n; i++) {
        // pivoteamento parcial
        long long int ipivo = find_max(A, i, n);
        if (i != ipivo) {
            swap_line(A, b, i, ipivo, n);
        }

        // salva o inverso do pivo antes do loop para não ter que dividir toda iteração
        intervaloDouble uno = {1.0, 1.0};
        intervaloDouble inv_pivo = divide(uno, A->dado[i][i]);

        for (long long int k = i + 1; k < n; k++) {
            intervaloDouble m = multiplica(A->dado[k][i], inv_pivo);
            A->dado[k][i].min = 0.0;
            A->dado[k][i].max = 0.0;
            for (long long int j = i + 1; j < n; j += 1) {
                A->dado[k][j] = subtrai(A->dado[k][j], multiplica(A->dado[i][j], m));
            }
            b[k] = subtrai(b[k], multiplica(b[i], m));
        }
    }
}

// realiza a retrosubstituicao e guarda os valores de x em x
void retrossubs(Matriz * restrict A, intervaloDouble * restrict b, intervaloDouble * restrict x, long long int n) {
    for (long long int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        // Unrolling de 2 instruções
        for (long long int j = i + 1; j < n; j += 2) {
            x[i] = subtrai(x[i], multiplica(A->dado[i][j], x[j]));
            x[i] = subtrai(x[i], multiplica(A->dado[i][j + 1], x[j + 1]));
        }

        x[i] = divide(x[i], A->dado[i][i]);
    }
}
