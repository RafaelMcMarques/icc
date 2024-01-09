#ifndef GAUSS_H
#define GAUSS_H

void gauss_elim_classic(double **A, double *b, int n);

void gauss_elim_no_mult(double **A, double *b, int n);

void gauss_elim_alternative(double **A, double *b, int n);

void retrossubs(double **A, double *b, double *x, int n);

void get_residual(double **A, double *b, double *x, double *r, int n);

void print_result(double *x, int n);

#endif
