#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>


/* EXERCÍCIO 1
Escreva um programa usando  pthreads, que cria 8 threads e utiliza  essas threads para fazer a soma vetorial 
R[0..N]=A[0..N]+B[0..N]. Onde todos vetores tenham 2^27 elementos e os vetores A e B devem 
ser iniciados com A[i]=B[i]=i
*/

long N;
int NTHREADS=8;
long* R;
long* A;
long* B;

void *somatoria(void *region_ptr){
	long region=(long) region_ptr;
	long somas;
	for(long i=0;i<10;i++){
		somas=A[i]+B[i];
	}
	R[region]=somas;
	return NULL;

}

int main(){
	N=pow(2,27);
	A=(long*)malloc(N*sizeof(long));
	B=(long*)malloc(N*sizeof(long));
	R=(long*)malloc(N*sizeof(long));
	long soma; // resultado da soma//
	pthread_t thread[8];
	for(long i=0;i<NTHREADS;i++){ // criacao das threads//
		pthread_create (&thread[i],NULL,somatoria,(void *)i);
	}

	for(long i=0;i<10;i++){ // atribuicao dos valores A//
		A[i]=i;
	}
	for(long j=0;j<10;j++){ //atribuicao B//
		B[j]=j;
	}
	soma=0;
	for(long i=0;i<NTHREADS;i++){ // criacao das threads//
		pthread_join(thread[i],NULL);
		soma+=R[i];
	}
	for(long i=0;i<10;i++){
		printf("%ld ",R[i]);
	}

}

