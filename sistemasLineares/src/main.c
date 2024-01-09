#include <stdio.h>
#include <stdlib.h>

#include "eliminacao-gauss.h"
#include "utils.h"
#include "likwid.h"

int main() {

    int n;
    double **A, **Ac;
    double *b, *bc, *r;
    double *x;

    scanf("%d", &n);
    
    /* -- malloc's -- */
    A = malloc(n * sizeof(double*));
    Ac = malloc(n * sizeof(double*));
    if (A == NULL || Ac == NULL) {
        return 1;
    }

    for (int i = 0; i < n; i++) {
        A[i] = malloc(n * sizeof(double));
        Ac[i] = malloc(n * sizeof(double));
        if (A[i] == NULL || Ac[i] == NULL) {
            return 1;
        }
    }

    b = malloc(n * sizeof(double));
    bc = malloc(n * sizeof(double));
    x = malloc(n * sizeof(double));
    r = malloc(n * sizeof(double));
    if (b == NULL || bc == NULL || x == NULL || r == NULL) {
        return 1;
    }

    /* -- read data -- */ 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%lf", &A[i][j]);
        }
        scanf("%lf", &b[i]);
    }
    
    /* -- gauss elimination methods -- */
    void (*methods[3])(double**, double*, int) = {&gauss_elim_classic, &gauss_elim_no_mult, &gauss_elim_alternative};
    LIKWID_MARKER_INIT;
    for (int i = 0; i < 3; i++) {
        /* -- generate copies of A and b  -- */
        for (int i = 0; i < n; i++) {
            bc[i] = b[i];
            for (int j = 0; j < n; j++)
                Ac[i][j] = A[i][j];
        }
        
        LIKWID_MARKER_START (markerName("Metodo", i + 1));

        double before = timestamp();

        methods[i](Ac, bc, n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%lf ", Ac[i][j]);
            }
            printf("%lf\n", bc[i]);
        }
        retrossubs(Ac, bc, x, n);
        get_residual(A, b, x, r, n);

        double after = timestamp();

        LIKWID_MARKER_STOP (markerName("Metodo", i + 1));

        printf("\n\n%d)\n", i + 1);
        printf("X = ");
        print_result(x, n);
        printf("Residuo = ");
        print_result(r, n);
        printf("Tempo: %lfms\n", after - before);
    }
    LIKWID_MARKER_CLOSE;
    
    /* -- free allocated memory -- */
    for (int i = 0; i < n; i++) {
        free(A[i]);
        free(Ac[i]);
    }

    free(A);
    free(Ac);
    free(b);
    free(bc);
    free(x);
    free(r);

    return 0;
}
