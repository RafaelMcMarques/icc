#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "interval-analysis.h"

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

// acha maior elemento da coluna
long long int find_max(intervaloDouble **A, long long int col, long long int n) {
    long long int max_row = col;
    for (long long int i = col + 1; i < n; i++) {
        if (fabs(A[i][col].max) > fabs(A[max_row][col].max)) {
            max_row = i;
        }
    }
    return max_row;
}

// troca duas linhas
void swap_line(intervaloDouble **A, intervaloDouble *b, long long int row1, long long int row2, long long int n) {
    intervaloDouble temp;
    for (long long int j = 0; j < n; j++) {
        temp.min = A[row1][j].min;
        temp.max = A[row1][j].max;
        A[row1][j].min = A[row2][j].min;
        A[row1][j].max = A[row2][j].max;
        A[row2][j].min = temp.min;
        A[row2][j].max = temp.max;
    }
    temp.min = b[row1].min;
    temp.max = b[row1].max;
    b[row1].min = b[row2].min;
    b[row1].max = b[row2].max;
    b[row2].min = temp.min;
    b[row2].max = temp.max;
}

void gauss_elim(intervaloDouble **A, intervaloDouble *b, long long int n) {
    for (long long int i = 0; i < n; i++) {
        // pivoteamento parcial
        long long int ipivo = find_max(A, i, n);
        if (i != ipivo) {
            swap_line(A, b, i, ipivo, n);
        }
        
        // eliminacao de gauss
        for (long long int k = i+1; k < n; k++) {
            intervaloDouble m = divide(A[k][i], A[i][i]);
            A[k][i].min = 0.0;
            A[k][i].max = 0.0;
            for (long long int j = i+1; j < n; j++)
                A[k][j] = subtrai(A[k][j] , multiplica(A[i][j], m));
            b[k] = subtrai(b[k], multiplica(b[i], m));
        }
    }
}

void retrossubs(intervaloDouble **A, intervaloDouble *b, intervaloDouble *x, long long int n) {
    for(long long int i = n-1; i >= 0; i--) {
        x[i].max = b[i].max;
        x[i].min = b[i].min;
        for(long long int j = i+1; j < n; j++)
            x[i] = subtrai(x[i], multiplica(A[i][j], x[j]));
        x[i] = divide(x[i], A[i][i]);
    }
}
