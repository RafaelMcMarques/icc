#include <stdio.h>
#include <stdlib.h>

#include "eliminacao-gauss.h"

// calcula L e U, gaurda U em m e L em l
void getLUFat(double **m, double **l, int n);

int main() {
    int n;
    scanf("%d", &n);

    double **m = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
        m[i] = malloc(n * sizeof(double));

    double **mi = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
        mi[i] = malloc(n * sizeof(double));

    double **l = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
        l[i] = malloc(n * sizeof(double));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%lf", &m[i][j]);
    
    // guardamos a matriz u na propria m
    getLUFat(m, l, n);

}     

void getLUFat(double **m, double **l, int n) {
    // inicializar valores fixos de l
    for (int i = 0; i < n; i++) {
        l[i][i] = 1;
        for (int j = i + 1; j < n; j++) 
            l[i][j] = 0;
    }
    
    for (int i = 0; i < n; i++) {
        for (int k = i + 1; k < n; k++) {
            double mult = m[k][i] / m[i][i];
            m[k][i] = 0;
            for (int j = i + 1; j < n; j++)
                m[k][j] -= mult * m[i][j];
            l[k][i] = mult;
        }
    }
}
