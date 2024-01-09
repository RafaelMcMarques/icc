#include <stdio.h>
#include <math.h>

#define EPS 1e-8

/*
 *  IMPLEMENTACAO DE METODOS PARA A RESOLUCAO DE EQUACOES NAO LINEARES
 *
 *  METODO DA BISECAO:
 *      - achar intervalo em que o 0 da funcao se encontra
 *      - ver meio do intervalo
 *      - ver se x esta para direita ou esquerda
 *      - repetir
 *
 *  METODO DE NEWTON_RAPHSON;
 *      - xi+1 = xi - f(xi)/f'(xi)
 *      - repetir ate chegar num x cujo f(x) é proximo o suficiente de 0, ou que mudou pouco desde xi-1
 *
 *
 *
 * */

double f(double x) {
    return x*x + x*x*x + cos(-x) * log(x/2);
}

double df(double x) {
    return -log(x/2)*sin(x) + cos(x)/x + 3*x*x + 2*x;
}

double newton(double x);
double bissecao(double a, double b);

long long int iterBiss = 0;
long long int iterNew = 0;

int main() {
    printf("bissecao: %lf\n", bissecao(0, 3));
    printf("newton: %lf\n", newton(1.5));
    printf("iteracoes bissecao: %lld\n", iterBiss);
    printf("iteracoes newton: %lld\n", iterNew);

}

double bissecao(double a, double b) {
    double m = (a + b) / 2;
    double fm = f(m);

    if (fabs(fm) <= EPS)
        return m;

    iterBiss++;
    if (f(a) > 0 && f(b) < 0) {
        if (fm > 0)
            return bissecao(m, b);
        return bissecao(a, m);
    }
    if (fm > 0)
        return bissecao(a, m);
    return bissecao(m, b);

    return 0;
        
}

double newton(double x) {
    while (fabs(f(x)) > EPS) {
        iterNew++;
        x = x - f(x)/df(x);
    }
    return x;
}
