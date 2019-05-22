#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <ctype.h>
#include <mpi.h>

//Movimentos Possiveis das Particulas
#define ESQUERDA 1
#define ESQUERDA_CIMA 2
#define CIMA 3
#define DIREITA_CIMA 4
#define DIREITA 5
#define DIREITA_BAIXO 6
#define BAIXO 7
#define ESQUERDA_BAIXO 8



using namespace std;
using namespace cv;

//Variaveis Globais
int linhas,colunas,N,colidiu=0;

struct timeval inicioTempo,fimTempo;

//Estrutura de Cada particula

//Array Global de Particulas
int **particula=NULL;
int **p=NULL;


/*================================= INICIALIZA =================================*/

void inicializa()
{
	int i;

	particula=(int**)malloc(N*sizeof(int*)); //aloca Array de N particulas//
	p=(int**)malloc(N*sizeof(int*)); //aloca Array de N particulas//
	for(i=0;i<N;i++){
		particula[i]=(int*)malloc(5*sizeof(int)); 
		p[i]=(int*)malloc(5*sizeof(int)); 
			
	}
	if(!particula)
	{
		printf("\n Não existe espaço na memoria \n");
	}
}

/*================================= MOVIMENTA =================================*/

void movimenta(int *p){
	switch(p[2]){
		case ESQUERDA: p[1]=p[1]-1;  // move particula para esquerda//
			break;
		case ESQUERDA_CIMA:	p[1]=p[1]-1; //move particula para esquerda + cima//
							p[0]=p[0]-1;
			break;
		case CIMA:	p[0]=p[0]-1; // move particula para cima//
			break;
		case DIREITA_CIMA:	p[1]=p[1]+1; // move particula para direita + cima//
							p[0]=p[0]-1;
			break;
		case DIREITA: p[1]=p[1]+1; // move particula para direita//
			break;
		case DIREITA_BAIXO:	p[1]=p[1]+1; // move particula para direita + baixo//
							p[0]=p[0]+1;
			break;
		case BAIXO: p[0]=p[0]+1; // move particula para baixo//
			break;
		case ESQUERDA_BAIXO: p[1]=p[1]-1; // move particula para esquerda + baixo//
							p[0]=p[0]+1;
			break;
	}
	p[4]=1;//Bloqueio da Particula
}

/*=================================SOMA DIRECAO=================================*/
// Dado a direcao de duas particulas que sofreram colisao se decide a nova dire- //
// cao de ambas com base na direcao atual //
void somaDirecao(int *a, int *b){
	switch(*a){
		case ESQUERDA: switch(*b){
					case ESQUERDA: *b=DIREITA; break;
					case ESQUERDA_CIMA: *a=DIREITA_CIMA; *b=DIREITA; break;            
					case CIMA: *a=DIREITA_BAIXO; *b=DIREITA; break;
					case DIREITA_CIMA: *a=BAIXO; *b=DIREITA_BAIXO; break;
					case DIREITA: *a=DIREITA_BAIXO; *b=ESQUERDA_CIMA; break;
					case DIREITA_BAIXO: *a=CIMA; *b=DIREITA_CIMA; break;
					case BAIXO: *a=CIMA; *b=DIREITA; break;
					case ESQUERDA_BAIXO: *a=DIREITA_CIMA; *b=DIREITA; break;
					default: break;
				} break;
		case ESQUERDA_CIMA:
				switch(*b){
					case ESQUERDA: *a=DIREITA; *b=DIREITA_BAIXO; break;
					case ESQUERDA_CIMA: *b=DIREITA_BAIXO; break;
					case CIMA: *a=BAIXO; *b=DIREITA_BAIXO; break;
					case DIREITA_CIMA: *a=BAIXO; *b=DIREITA_BAIXO; break;
					case DIREITA: *a=ESQUERDA_BAIXO; *b=BAIXO; break;
					case DIREITA_BAIXO: *a=BAIXO; *b=CIMA; break;
					case BAIXO: *a=DIREITA; *b=DIREITA_BAIXO; break;
					case ESQUERDA_BAIXO: *a=DIREITA; *b=DIREITA_BAIXO; break;
					default: break;
				} break;
		case CIMA:
				switch(*b){
					case ESQUERDA: *a=DIREITA_BAIXO; *b=BAIXO; break;
					case ESQUERDA_CIMA: *a=DIREITA_BAIXO; *b=BAIXO; break;
					case CIMA: *b=BAIXO; break;
					case DIREITA_CIMA: *a=ESQUERDA_BAIXO; *b=CIMA; break;
					case DIREITA: *a=DIREITA_CIMA; *b=CIMA; break;
					case DIREITA_BAIXO: *a=ESQUERDA_BAIXO; *b=BAIXO; break;
					case BAIXO: *a=DIREITA_BAIXO; *b=ESQUERDA_CIMA; break;
					case ESQUERDA_BAIXO: *a=DIREITA_BAIXO; *b=ESQUERDA_CIMA; break;
					default: break;
				} break;
		case DIREITA_CIMA:
				switch(*b){
					case ESQUERDA: *a=BAIXO; *b=DIREITA_CIMA; break;
					case ESQUERDA_CIMA: *a=BAIXO; *b=ESQUERDA_BAIXO; break;
					case CIMA: *a=BAIXO; *b=ESQUERDA_BAIXO; break;
					case DIREITA_CIMA: *b=ESQUERDA_BAIXO; break;
					case DIREITA: *a=ESQUERDA; *b=ESQUERDA_BAIXO; break;
					case DIREITA_BAIXO: *a=ESQUERDA_CIMA; *b=ESQUERDA_BAIXO; break;
					case BAIXO: *a=ESQUERDA_CIMA; *b=ESQUERDA; break;
					case ESQUERDA_BAIXO: *a=BAIXO; *b=CIMA; break;
					default: break;
				} break;
		case DIREITA:
				switch(*b){
					case ESQUERDA: *a=ESQUERDA_CIMA; *b=DIREITA_BAIXO; break;
					case ESQUERDA_CIMA: *a=BAIXO; *b=ESQUERDA; break;
					case CIMA: *a=BAIXO; *b=ESQUERDA_BAIXO; break;
					case DIREITA_CIMA: *a=ESQUERDA_BAIXO; *b=ESQUERDA; break;
					case DIREITA: *b=ESQUERDA; break;
					case DIREITA_BAIXO: *a=ESQUERDA_CIMA; *b=ESQUERDA; break;
					case BAIXO: *a=ESQUERDA_CIMA; *b=ESQUERDA; break;
					case ESQUERDA_BAIXO: *a=CIMA; *b=ESQUERDA; break;
					default: break;
				} break;			
		case DIREITA_BAIXO:
				switch(*b){
					case ESQUERDA: *a=DIREITA_CIMA; *b=ESQUERDA_CIMA; break;
					case ESQUERDA_CIMA: *a=CIMA; *b=BAIXO; break;
					case CIMA: *a=ESQUERDA_BAIXO; *b=ESQUERDA; break;
					case DIREITA_CIMA: *a=ESQUERDA_BAIXO; *b=ESQUERDA_CIMA; break;
					case DIREITA: *a=ESQUERDA; *b=ESQUERDA_CIMA; break;//
					case DIREITA_BAIXO: *b=ESQUERDA_CIMA; break;
					case BAIXO: *a=CIMA; *b=ESQUERDA_CIMA; break;
					case ESQUERDA_BAIXO: *a=DIREITA_CIMA; *b=ESQUERDA_CIMA; break;
					default: break;
				} break;
		case BAIXO:
				switch(*b){
					case ESQUERDA: *a=DIREITA_CIMA; *b=ESQUERDA_CIMA;break;
					case ESQUERDA_CIMA: *a=DIREITA; *b=CIMA;break;
					case CIMA: *a=DIREITA_CIMA; *b=ESQUERDA_BAIXO;break;
					case DIREITA_CIMA: *a=DIREITA; *b=CIMA;break;
					case DIREITA: *a=ESQUERDA; *b=CIMA;break;
					case DIREITA_BAIXO: *a=ESQUERDA_CIMA; *b=CIMA;break;
					case BAIXO: *b=CIMA; break;
					case ESQUERDA_BAIXO: *a=DIREITA_CIMA; *b=CIMA;break;
					default: break;
				} break;
		case ESQUERDA_BAIXO:
				switch(*b){
					case ESQUERDA: *a=DIREITA; *b=DIREITA_CIMA; break;
					case ESQUERDA_CIMA: *a=DIREITA_BAIXO; *b=DIREITA_CIMA; break;
					case CIMA: *a=DIREITA_BAIXO; *b=DIREITA; break;
					case DIREITA_CIMA: *a=CIMA; *b=BAIXO; break;
					case DIREITA: *a=ESQUERDA_CIMA; *b=CIMA; break;
					case DIREITA_BAIXO: *a=ESQUERDA_CIMA; *b=DIREITA_CIMA; break;
					case BAIXO: *a=CIMA; *b=DIREITA_CIMA; break;
					case ESQUERDA_BAIXO: *b=DIREITA_CIMA; break;
					default: break;
				} break;
	}

}
/*================================= COLISAO =================================*/
//Dado uma particula inicial p, percorre-se o array de particulas afim de aplicar colisoes//

int colisao(int p){
	int i;
	for(i=0;i<N;i++){
		{
			if(particula[p][4] == 0 && particula[i][4]==0){//verifica se ambas particulas estao prontas para se mover 
					//caso possam se decide se hou colisao ou nao
					if ((particula[i][0] - particula[i][3] <= particula[p][0] + particula[p][3] && particula[i][0] + particula[i][3] >= particula[p][0] - particula[p][3]) && (particula[i][1] - particula[i][3] <= particula[p][1] + particula[p][3] && 	particula[i][1] + particula[i][3] >= particula[p][1] - particula[p][3]) && i != p){
						somaDirecao(&particula[p][2],&	particula[i][2]);//atribui as novas direcoes
						movimenta(particula[p]);//movimenta
						movimenta(particula[i]);//movimenta
						return 1;//retorno de colisao verdadeiro
					}
				}
			}
	}
	return 0;//retorno de colisao falsa, nao houve colisao
}

/*================================= INSERE =================================*/
//Distribui os Elementos do Array uniformemente//

void insere()
{	
	int direcao=1;//direcao inicial
	int k;
	for(k=0;k<N;k++){
		particula[k][2]=direcao++;
		
		if (direcao > 8)
			direcao=1;
		particula[k][3]=6;
		particula[k][4]=0;
		//printf("%d \n",particula[k][2]);
	}
	k=0;	
	for(int i=10;i<linhas;i+=25){
		if(k > N){
				break;
		}	
		for(int j=10;j<colunas;j+=25){
			if(k > N){
				break;
			}
			particula[k][0]=i;
			particula[k][1]=j;
			k++;	
		}
	}
}


/*================================= DIREITABORDA =================================*/
//Verifica se a particula colidiu com a borda e aplica o uma nova direcao//
void detectaBorda(int i){
	if(particula[i][0] > linhas - particula[i][3]){
		switch(particula[i][2]){
			case DIREITA_BAIXO: particula[i][2]=DIREITA_CIMA; break;
			case BAIXO: particula[i][2]=CIMA; break;
			case ESQUERDA_BAIXO: particula[i][2]=ESQUERDA_CIMA; break;
			default: break;
		}
	}
	if(particula[i][0] <= 0 + particula[i][3] ){
		switch(particula[i][2]){
			case ESQUERDA_CIMA: particula[i][2]=ESQUERDA_BAIXO; break;
			case CIMA: particula[i][2]=BAIXO; break;
			case DIREITA_CIMA: particula[i][2]=DIREITA_BAIXO; break;
			default: break;
		}
	}
	if(particula[i][1] > colunas - particula[i][3]){
		switch(particula[i][2]){
			case DIREITA_CIMA: particula[i][2]=ESQUERDA_CIMA; break;
			case DIREITA: particula[i][2]=ESQUERDA; break;
			case DIREITA_BAIXO: particula[i][2]=ESQUERDA_BAIXO; break;
			default: break;
		}
	}
	if(particula[i][1] <= 0 + particula[i][3] ){
		switch(particula[i][2]){
			case ESQUERDA: particula[i][2]=DIREITA; break;
			case ESQUERDA_CIMA: particula[i][2]=DIREITA_CIMA; break;
			case ESQUERDA_BAIXO: particula[i][2]=DIREITA_BAIXO; break;
			default: break;
		}
	}
}

/*================================= ATUALIZA =================================*/
//Funcao principal que chama a deteccao de colisao para todos os elementos do Array//
void atualiza()
{
	int i;
	for(i=0; i<N;i++){
		detectaBorda(i);
		if (!colisao(i))
			movimenta(particula[i]);
		detectaBorda(i);
	}	
}

/*================================= ATUALIZA =================================*/
//Percorre todo o Array de Elementos e Libera o flag dos mesmo//
void libera(){
	int i;
	for(i=0; i<N;i++)
		particula[i][4]=0;
}

int main(int argc, char** argv)
{
	int linhass;
	N=atoi(argv[1]);
	srand(time(NULL));
	MPI_Status status;
	//Mat image(linhas,colunas,CV_8UC3,Scalar(255,255,255));
	linhas=N;
	colunas=5;
	inicializa();
	insere();


	MPI_Comm comm;

  	int numtasks,taskid,numworkers,source,dest,linhas,offset,i,j,k;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	numworkers = numtasks-1;
	printf("rank: %d \n",taskid);
	
	if (taskid == 0) {
		inicializa();
		insere();
		printf("teste1\n");
	}

	linhas = (int)N/numworkers;
	
    offset = 0;
printf("teste2\n");
    for (dest=1; dest<=numworkers; dest++)
    {
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&linhas, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&particula[offset][0], linhas*colunas, MPI_INT,dest,1, MPI_COMM_WORLD);
      offset = offset + linhas;
      printf("%d\n",dest);
    }
printf("teste3\n");
    /* wait for results from all worker tasks */
    for (i=1; i<=numworkers; i++)
    {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&linhas, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&p[offset][0], linhas*colunas, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
    }
printf("teste4\n");
    /*
    printf("Here is the result matrix:\n");
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++)
        printf("%%d   ", c[i][j]);
      printf ("\n");
    }

    fprintf(stdout,"Time = %.6f\n\n",
         (stop.tv_sec+stop.tv_usec*1e-6)-(start.tv_sec+start.tv_usec*1e-6));

	*/
  /*---------------------------- worker----------------------------*/
  if (taskid > 0) {
    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&linhas, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&particula, linhas*colunas, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

    /* Matrix multiplication 
    for (k=0; k<N; k++)
      for (i=0; i<linhas; i++) {
        c[i][k] = 0.0;
        for (j=0; j<N; j++)
          c[i][k] = c[i][k] + a[i][j] * b[j][k];
      }
	*/

	for(i=0; i<N;i++){
		detectaBorda(i);
		if (!colisao(i))
			movimenta(particula[i]);
		detectaBorda(i);
	}

    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&linhas, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&p, linhas*colunas, MPI_INT, 0, 2, MPI_COMM_WORLD);
  }








	gettimeofday(&inicioTempo,NULL);
	int ciclos=1000;

	while (colidiu<ciclos) // Loop demorado * aumentando numero de particulas //
    {
		atualiza();
		libera();
		/*Mat image(linhas,colunas,CV_8UC3,Scalar(255,255,255));
		
		for(i=0;i < N;i++){			
		   circle( image, Point(particula[i][1],particula[i][0] ), particula[i][3], Scalar(0,0,particula[i].cor),-1, 8);
	  	}
		
		cv::resize(image, image, cv::Size(400,400));		
        	cv::imshow("Imagem", image);
        	cv::waitKey(1);*/
		colidiu++;
	}
	gettimeofday(&fimTempo,NULL);
	printf("Ciclos:%d N:%d Linhas:%d Colunas:%d \n",ciclos,N,linhas,colunas);
	printf("Tempo de operacao = %ld microsegundo(s); %ld milisegundo(s); %ld segundo(s)\n",((fimTempo.tv_sec*1000000+fimTempo.tv_usec)-(inicioTempo.tv_sec*1000000+inicioTempo.tv_usec)),((fimTempo.tv_sec*1000000+fimTempo.tv_usec)-(inicioTempo.tv_sec*1000000+inicioTempo.tv_usec))/1000,((fimTempo.tv_sec*1000000+fimTempo.tv_usec)-(inicioTempo.tv_sec*1000000+inicioTempo.tv_usec))/1000000);	
	MPI_Finalize();
	return 0;

}
