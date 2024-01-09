#include <stdio.h>
#include <stdlib.h>

#define F 0
#define N 50

void gaussSeidel(double** m, double *x, int n);
void gaussJacobi(double** m, double *x, int n);

int main() {
    int n;
    scanf("%d", &n);

    double **m1 = malloc(n * sizeof(double*));
    double **m2 = malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        m1[i] = malloc(n * sizeof(double));
        m2[i] = malloc(n * sizeof(double));
    }
    double *x1 = malloc((n + 1) * sizeof(double));
    double *x2 = malloc((n + 1) * sizeof(double));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n + 1; j++) {
            scanf("%lf", &m1[i][j]);
            m2[i][j] = m1[i][j];
        }

    gaussJacobi(m2, x2, n);
    gaussSeidel(m1, x1, n);

    printf("\n");
    for (int i = 0; i < n; i++)
        printf("%lf ", x1[i]);
    printf("\n\n");
    for (int i = 0; i < n; i++)
        printf("%lf ", x2[i]);
    printf("\n");
}

void gaussJacobi(double** m, double *x, int n) {
    for (int i = 0; i < n; i++)
        x[i] = (double)F;
    double nx[n];

    for (int c = 0; c < N; c++) {
        for (int i = 0; i < n; i++) {
            nx[i] = m[i][n];
            for (int j = 0; j < n; j++) {
                if (j != i)
                    nx[i] -= m[i][j] * x[j]; 
            }
            nx[i] /= m[i][i];
        }

        for (int i = 0; i < n; i++)
            x[i] = nx[i];
    }
}

void gaussSeidel(double** m, double *x, int n) {
    for (int i = 0; i < n; i++)
        x[i] = (double)F;
    for (int c = 0; c < N; c++) {
        for (int i = 0; i < n; i++) {
            x[i] = m[i][n];
            for (int j = 0; j < n; j++) {
                if (j != i)
                    x[i] -= m[i][j] * x[j]; 
            }
            x[i] /= m[i][i];
        }
    }

}
