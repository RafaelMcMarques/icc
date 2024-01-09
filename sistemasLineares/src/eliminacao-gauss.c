#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "eliminacao-gauss.h"

int find_max(double **A, int col, int n) {
    int max_row = col;
    for (int i = col + 1; i < n; i++) {
        if (fabs(A[i][col]) > fabs(A[max_row][col])) {
            max_row = i;
        }
    }
    return max_row;
}

void swap_line(double **A, double *b, int row1, int row2, int n) {
    for (int j = 0; j < n; j++) {
        double temp = A[row1][j];
        A[row1][j] = A[row2][j];
        A[row2][j] = temp;
    }
    double temp_b = b[row1];
    b[row1] = b[row2];
    b[row2] = temp_b;
}

void gauss_elim_classic(double **A, double *b, int n) {
    for (int i = 0; i < n; i++) {
        int ipivo = find_max(A, i, n);
        if (i != ipivo) {
            swap_line(A, b, i, ipivo, n);
        }

        for (int k = i+1; k < n; k++) {
            double m = A[k][i] / A[i][i];
            A[k][i] = 0.0;
            for (int j = i+1; j < n; j++) {
                A[k][j] -= A[i][j] * m;
            }
            b[k] -= b[i] * m;
        }
    }
}

void gauss_elim_no_mult(double **A, double *b, int n) {

    for (int i = 0; i < n; i++) {
        int ipivo = find_max(A, i, n);
        if (i != ipivo) {
            swap_line(A, b, i, ipivo, n);
        }

        for (int k = i+1; k < n; k++) {
            for (int j = i+1; j < n; j++) {
                A[k][j] = A[k][j] * A[i][i] - A[i][j] * A[k][i];
            }
            b[k] = b[k] * A[i][i] - b[i] * A[k][i];
            A[k][i] = 0.0;
        }
    }
}


void gauss_elim_alternative(double **A, double *b, int n) {
    for(int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++)
            A[i][j] /= A[i][i];
        b[i] /= A[i][i];
        A[i][i] = 1.0;
        for (int k = i + 1; k < n; k++){
            double m = A[k][i];
            A[k][i] = 0.0;
            for (int j = i + 1; j < n; j++)
                A[k][j] -= A[i][j] * m;
            b[k] -= b[i] * m;
        }
    }
} 

void retrossubs(double **A, double *b, double *x, int n) {
    for(int i = n-1; i >= 0; i--) {
        x[i] = b[i];
        for(int j = i+1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
}

void get_residual(double **A, double *b, double *x, double *r, int n) {
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            sum += x[j] * A[i][j];
        }
        r[i] = fabs(sum - b[i]);
    }
}

void print_result(double *x, int n) {

    for(int i = 0; i < n; i++) {
        printf("%lf ", x[i]);
    }
    printf("\n");
}

