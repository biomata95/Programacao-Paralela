#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

/*Nome: Icaro Neves Melo */

/* Multiplicacao de Matrizes Paralela */

struct timeval inicio,fim;  /* contadores de tempo */
 

double **alocaMatriz(int N)
{
  int i,j,k;  /* i,j,k variaveis utilizadas como indice */
  double **mat=NULL;  /* ponteiro para a matriz */

  /* IF: Aloca linhas da  matriz */
  if ( ! (mat = (double **) malloc(N*sizeof(double))) ){
    return (NULL); 
  }
  /* Laço para alocar colunas */
    	for ( i = 0; i <N; i++ ) {
      		mat[i] = (double*) malloc (N*sizeof(double)); /*aloca mais colunas*/
      	if (mat[i] == NULL) {
         	printf ("** Erro: Memoria Insuficiente **");
         	return (NULL);
       	}
    }
    return (mat);
}



int main (char argc,char *argv[])
{
	int n = 1500;
  	double **A,**B,**C;
	int i,j,k,threadsNum;
	double soma;
	A=alocaMatriz(n);
	B=alocaMatriz(n);
	C=alocaMatriz(n);
	threadsNum = omp_get_max_threads ( ); 
	printf ( "\n" );
	printf ( " Numero de processadores = %d\n", omp_get_num_procs ( ) );
	printf ( " Threads Disponiveis   = %d\n", threadsNum);

	gettimeofday(&inicio,NULL);
	# pragma omp parallel shared (A,B,C, n, soma) private (i, j, k ) 
	/* Compartilhamento das matrizes A,B,C; tamanho da matriz n; e a somatoria de todos os elementos da matriz resultante */
	/* indices da matrizes como privados */
	{
	 	# pragma omp for /* parelizacao do for para atribuicao das matrizes */
	  		for ( i = 0; i < n; i++ )
	  		{
	    			for ( j = 0; j < n; j++ )
	    			{
					A[i][j]=i+j;
					B[i][j]=i+j;
	    			}
		  	}

	  	# pragma omp for  /* parelizacao do for para multiplicacao das matrizes */
	  	for ( i = 0; i < n; i++ )
	  	{
	    		for ( j = 0; j < n; j++ )
	    		{
	      			for ( k = 0; k < n; k++ )
	      			{
					C[i][j] = C[i][j] + A[i][k] * B[k][j];
	      			}
	    		}
		}
	}
	soma=0;
  	# pragma omp for  /* somatoria de todos os elementos da matriz resultante */
  	for ( i = 0; i < n; i++ )
  	{
    		for ( j = 0; j < n; j++ )
    		{
			soma=soma+C[i][j];
     		}
	}
	
	gettimeofday(&fim,NULL);
  	printf ( "\n \n \n" );
  	printf ( "--Resultado--\n");
  	printf ( " Somatoria: %lf\n",soma);

  	printf ( "\n \n" );
	printf("Tempo da multiplicacao de matrizes = %ld microsegundo(s); %ld milisegundo(s); %ld segundo(s)",((fim.tv_sec*1000000+fim.tv_usec)-(inicio.tv_sec*1000000+inicio.tv_usec)),((fim.tv_sec*1000000+fim.tv_usec)-(inicio.tv_sec*1000000+inicio.tv_usec))/1000,((fim.tv_sec*1000000+fim.tv_usec)-(inicio.tv_sec*1000000+inicio.tv_usec))/1000000);
  	printf ( "\n \n" );
  return 0;
}

