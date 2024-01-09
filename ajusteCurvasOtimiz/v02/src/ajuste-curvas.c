#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "interval-analysis.h"
#include "matriz.h"

/*
 * Lucas Gabriel Batista Lopes GRR20220062
 * Rafel Munhoz da Cunha Marques GRR20224385
*/

/* Monta o sistema linear gerado pelo metodo ds minimos quadrados para ajustar um pol de grau n a k pontos
 *
 * OTIMIZAÇÕES 
 *
 * calcula apenas a primeira linha a ultima coluna e depois atribui aos respectivos elementos da matriz
 * uma vez que os valores se repetem
 * itera pelo vetor de pontos apenas uma vez, somando todas as pontencias de x nos elementos do sl correspondentes
 * 
*/
void montarSistemaAjuste(intervaloDouble * restrict xs, intervaloDouble * restrict ys, long long int n, int k, Matriz * restrict sistemaLinear, intervaloDouble * restrict b) {
    int s = n + 1;

    sistemaLinear->dado[0][0].min = k; // SL[0][0] = somatorio de xs⁰ 
    sistemaLinear->dado[0][0].max = k;

    // itera pelos pontos apenas uma vez, otimizando a cache
    for (long long int i = 0; i < k; i++) {
        intervaloDouble xpot = xs[i];
        b[0] = soma(b[0], ys[i]);
        
        // soma xi^j em sl[0][j] e (xi^j * yi) em b[i]
        for (int j = 1; j < s; j++) {
            sistemaLinear->dado[0][j] = soma(sistemaLinear->dado[0][j], xpot);
            b[j] = soma(b[j], multiplica(xpot, ys[i]));
            xpot = multiplica(xpot, xs[i]);
        }

        // soma xi^j+s-1 em sl[j][s-1]
        for (int j = 1; j < s; j++) {
            sistemaLinear->dado[j][s - 1] = soma(sistemaLinear->dado[j][s - 1], xpot);
            xpot = multiplica(xpot, xs[i]);
        }
    }

    // para os demais elementos de sl, copia o da diagonal superior direita
    for (int i = 1; i < s; i++) {
        for (int j = 0; j < s - 1; j++) {
            sistemaLinear->dado[i][j] = sistemaLinear->dado[i -1][j + 1];
        }
    }

}


/* calcula o residuo em intervalos de min e max
 *
 * OTIMIZACOES:
 *
 * x_pow armazena a potencia da iteração atual
 * ao contrário de calcular a potência do 0 em cada iteração
 *
*/
void calculaResiduo(intervaloDouble * restrict xs, intervaloDouble * restrict ys, long long int n, long long int k, intervaloDouble * restrict coef, intervaloDouble * restrict residuo) {

    for (long long int i = 0; i < k; i++) {
        // Calcular f(xi)
        intervaloDouble x_pow = {1.0, 1.0};
        intervaloDouble fxi = coef[0];
        for (long long int j = 1; j <= n; j += 2) {
            x_pow = multiplica(x_pow, xs[i]);
            fxi = soma(fxi, multiplica(coef[j], x_pow));

            x_pow = multiplica(x_pow, xs[i]);
            fxi = soma(fxi, multiplica(coef[j+1], x_pow));
        }
        // Calcular residuo[i]
        residuo[i] = subtrai(ys[i], fxi);
    }
}
