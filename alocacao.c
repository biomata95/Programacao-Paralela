#include <stdio.h>
#include <stdlib.h>

float **Alocar_matriz_real (int m, int n)
{
  float **v;  /* ponteiro para a matriz */
  int   i;    /* variavel auxiliar      */
  if (m < 1 || n < 1) { /* verifica parametros recebidos */
     printf ("** Erro: Parametro invalido **\n");
     return (NULL);
     }
  /* aloca as linhas da matriz */
  v = (float **) calloc (m, sizeof(float *));
  if (v == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
     return (NULL);
     }
  /* aloca as colunas da matriz */
  for ( i = 0; i < m; i++ ) {
      v[i] = (float*) calloc (n, sizeof(float));
      if (v[i] == NULL) {
         printf ("** Erro: Memoria Insuficiente **");
         return (NULL);
         }
      }
  return (v); /* retorna o ponteiro para a matriz */
}

float **Liberar_matriz_real (int m, int n, float **v)
{
  int  i;  /* variavel auxiliar */
  if (v == NULL){ 
	return (NULL);
  }
  if (m < 1 || n < 1) {  /* verifica parametros recebidos */
     printf ("** Erro: Parametro invalido **\n");
     return (v);
     }
  for (i=0; i<m; i++){
 	free (v[i]); /* libera as linhas da matriz */
  }
  free (v);      /* libera a matriz */
  return (NULL); /* retorna um ponteiro nulo */
}

void main (void)
{
  float **mat;  /* matriz a ser alocada */
  int   l, c;   /* numero de linhas e colunas da matriz */
    l=2;
    c=3;         /* outros comandos, inclusive inicializacao para l e c */
  mat = Alocar_matriz_real (l, c);
             /* outros comandos utilizando mat[][] normalmente */
  mat = Liberar_matriz_real (l, c, mat);
  
}
