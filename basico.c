#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


int main(char argc,char argv[]){
	
	int n=7;
	int a[n],t[n];
	
	#pragma omp parallel shared(a,t,n)
	{
		int tid=0;
		int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
		#ifdef _OPENMP
			tid=omp_get_thread_num();
		#endif
		#pragma omp for
		for(int i=0;i<n;i++){
			a[i]=2*i;
			t[i]=tid;
		}
	}
	printf("Processadores: %d\n",numCPU);	
	printf("eu,a(i),thread number\n");
	for(int i=0;i<n;i++){
		printf("%d %d %d \n",i,a[i],t[i]);
	}
	return 0;
}
