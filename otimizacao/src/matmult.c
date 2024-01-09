#include <stdio.h>
#include <stdlib.h>    /* exit, malloc, calloc, etc. */
#include <string.h>
#include <getopt.h>    /* getopt */
#include <time.h>

#include "matriz.h"
#include "utils.h"
#include <likwid.h>

//#define DEBUG

/**
 * Exibe mensagem de erro indicando forma de uso do programa e termina
 * o programa.
 */

static void usage(char *progname)
{
  fprintf(stderr, "Forma de uso: %s [ <ordem> ] \n", progname);
  exit(1);
}



/**
 * Programa principal
 * Forma de uso: matmult [ -n <ordem> ]
 * -n <ordem>: ordem da matriz quadrada e dos vetores
 *
 */

int main (int argc, char *argv[]) 
{
  int c, n=DEF_SIZE;
  
  MatRow mRow_1, mRow_2, resMat;
  Vetor vet, res;
  
  /* =============== TRATAMENTO DE LINHA DE COMANDO =============== */

  if (argc < 2)
    usage(argv[0]);

  n = atoi(argv[1]);
  
  /* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */

  srandom(20232);
      
  res = (real_t *) malloc (n*sizeof(real_t));
  resMat = geraMatRow(n, n, 1);
    
  mRow_1 = geraMatRow (n, n, 0);
  mRow_2 = geraMatRow (n, n, 0);

  vet = geraVetor (n, 0);

#ifdef _DEBUG_
    prnMat (mRow_1, n, n);
    prnMat (mRow_2, n, n);
    prnVetor (vet, n);
    printf ("=================================\n\n");
#endif /* _DEBUG_ */

  LIKWID_MARKER_INIT;

  printf("===== tempo sem otimizacao =====\n");
  double time = timestamp();

  LIKWID_MARKER_START("multMatVet_SemOtimiz");
  multMatVet (mRow_1, vet, n, n, res);
  LIKWID_MARKER_STOP("multMatVet_SemOtimiz");

  time = timestamp() - time;
  printf("multMatVet = %lf\n", time);

  time = timestamp();

  LIKWID_MARKER_START("multMatMat_SemOtimiz");
  multMatMat (mRow_1, mRow_2, n, resMat);
  LIKWID_MARKER_STOP("multMatMat_SemOtimiz");

  time = timestamp() - time;
  printf("multMatMat %lf\n", time);


#ifdef DEBUG
  prnVetor (res, n);
  prnMat (resMat, n, n);
#endif /* DEBUG */

  /* zerar vetor de resultado */
  liberaVetor((void *) res);
  res = geraVetor(n, 1);

  printf("===== tempo com otimizacao =====\n");
  time = timestamp();

  
  LIKWID_MARKER_START("multMatVet_Otimiz");
  multMatVet_opt (mRow_1, vet, n, n, res);
  LIKWID_MARKER_STOP("multMatVet_Otimiz");

  time = timestamp() - time;
  printf("multMatVet = %lf\n", time);

  time = timestamp();

  LIKWID_MARKER_START("multMatMat_Otimiz");
  multMatMat_opt (mRow_1, mRow_2, n, resMat);
  LIKWID_MARKER_STOP("multMatMat_Otimiz");

  time = timestamp() - time;
  printf("multMatMat %lf\n", time);

  LIKWID_MARKER_CLOSE;

 
#ifdef DEBUG
    prnVetor (res, n);
    prnMat (resMat, n, n);
#endif /* DEBUG */

  liberaVetor ((void*) mRow_1);
  liberaVetor ((void*) mRow_2);
  liberaVetor ((void*) resMat);
  liberaVetor ((void*) vet);
  liberaVetor ((void*) res);

  return 0;
}
