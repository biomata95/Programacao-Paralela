#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

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


int main(char argc,char *argv[]){
	char *seq1;
	char *seq2;
	int i,j,n,m,k,l,h,retorno,a,b,c;
	seq1=argv[1];
	seq2=argv[2];
	n=strlen(seq1)+1;
	m=strlen(seq2)+1;
	int matriz[n][m];
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			if(i==0 || j==0){
				matriz[i][j]=0;
			}
		}
	}
	for(i=1;i<n;i++){
		for(j=1;j<m;j++){
			a=matriz[i-1][j];
			b=matriz[i-1][j-1];
			c=matriz[i][j-1];
			retorno=maximo(a,b,c);
			if(seq1[i-1]==seq2[j-1]){
				matriz[i][j]=retorno+2;	
			}
			else{
				matriz[i][j]=retorno-1;
			}
		}
	}
	
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("%d ",matriz[i][j]);
		}
		printf("\n");
	}
	
	return 0;
}	





