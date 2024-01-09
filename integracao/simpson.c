#include <stdio.h>
#include <math.h>


/**
 *  Metodo de simpson para calcular integrais
 *  ideia: a cada tres pontos do intervalo, interpolar um polinomio de grau 2
 *  a integral desse polinomio = 1/6 * (x2 - x0) (f(x0) + 4f(x1) + f(x2)))
 */

// funcao integrada 
double f(double x) {
    return x*x*x + x*x/2 + 32*x + 12;
}

int main() {
    double result = 0;

    // limitante da integracao e numero de pontos
    double a,b;
    long int n;
    scanf("%lf %lf", &a, &b);
    scanf("%ld", &n);
    
    // passo entre os pontos
    double h = (b - a)/n;


    for (double i = a; i <= b - 2*h; i+=2 * h)
        result = result + (f(i) + 4 * f(i + h) + f(i + 2*h));

    result = result * h/3;
    printf("%lf\n", result);
}
