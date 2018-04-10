#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


int **alocaMatriz(unsigned int n,unsigned int m)
{
  int i,j,k;  /* i,j,k variaveis utilizadas como indice */
  int **mat=NULL;  /* ponteiro para a matriz */
  /* IF: Aloca linhas da  matriz */
  if ( ! (mat = (int **) malloc(n*sizeof(int))) ){
    return (NULL); 
  }
  /* Laço para alocar colunas */
    	for ( i = 0; i < n; i++ ) {
      		mat[i] = (int*) calloc (m, sizeof(int)); /*aloca mais colunas*/
      	if (mat[i] == NULL) {
         	printf ("** Erro: Memoria Insuficiente **");
         	return (NULL);
       	}
    }

  for(i=0;i<n;i++){
	for(j=0;j<m;j++){

			mat[i,j]=0;

	}
  }

  return (mat); /* retorna o ponteiro para a matriz */
}

int maximo(int x,int y,int z){
	int i,j,k,aux;
	int vet[3]={x,y,z};
	for(i=0;i<3;i++){		
		for(j=i+1;j<3;j++){
			if(vet[i]>vet[j]){
				aux=vet[i];
				vet[i]=vet[j];
				vet[j]=aux;
			}
		}
	}
	return vet[2];
	
}

int **smith(int **matriz,int n, int m,char *seq1,char *seq2)
{
	int i,j,retorno;
	int a;
	int b;
	int c;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("%d ",matriz[i,j]);
		}
		printf("\n");
	}

	for(i=1;i<n;i++){
		for(j=1;j<m;j++){
			printf("%d %d m:%d \n",i-1,j,matriz[i-1,j]);
			printf("%d %d m:%d \n",i-1,j-1,matriz[i-1,j-1]);
			printf("%d %d m:%d \n",i,j-1,matriz[i,j-1]);	
			a=*matriz[i-1,j];
			b=*matriz[i-1,j-1];
			c=*matriz[i,j-1];
			retorno=maximo(a,b,c);
			if(seq1[i]==seq2[j]){
				*matriz[i][j]=retorno+2;	
			}
			else{
				*matriz[i][j]=retorno-1;	
			}
		}
	}
	return (matriz);
}




int main(char argc,char *argv[]){

	int **matriz;
	char *seq1;
	char *seq2;
	int i,j,n,m,k,l,h;
	seq1=argv[1];
	seq2=argv[2];
	n=strlen(seq1)+1;
	m=strlen(seq2)+1;
	srand( 20172 );
	matriz=alocaMatriz(n,m); /* matriz original */

	matriz=smith(matriz,n,m,seq1,seq2);

	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("Olas %d %d \n",i,j);
			printf("%d ",matriz[i][j]);
		}
		printf("\n");
	}
	free(matriz);	
	return 0;
}
