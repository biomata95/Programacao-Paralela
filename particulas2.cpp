#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>


//Movimentos Possiveis das Particulas
#define ESQUERDA 1
#define ESQUERDA_CIMA 2
#define CIMA 3
#define DIREITA_CIMA 4
#define DIREITA 5
#define DIREITA_BAIXO 6
#define BAIXO 7
#define ESQUERDA_BAIXO 8

#define X 0
#define Y 1
#define DIRECAO 2
#define RAIO 3
#define FLAG 4


using namespace std;
using namespace cv;

//Variaveis Globais
int linhas,colunas,N,colidiu=0;

struct timeval inicioTempo,fimTempo;

//Array Global de Particulas
int **particula=NULL;


/*================================= INICIALIZA =================================*/

void inicializa()
{

	int i;
	particula=(int**)malloc(N*sizeof(int*)); //aloca Array de N particulas//
	for(i=0;i<N;i++){
		particula[i]=(int*)malloc(5*sizeof(int)); 
	}
	if(!particula)
	{
		printf("\n Não existe espaço na memoria \n");
	}
}

/*================================= MOVIMENTA =================================*/

void movimenta(int *p){
	switch(p[DIRECAO]){
		case ESQUERDA: p[Y]=p[Y]-1;  // move particula para esquerda//
			break;
		case ESQUERDA_CIMA:	p[Y]=p[Y]-1; //move particula para esquerda + cima//
							p[X]=p[X]-1;
			break;
		case CIMA:	p[X]=p[X]-1; // move particula para cima//
			break;
		case DIREITA_CIMA:	p[Y]=p[Y]+1; // move particula para direita + cima//
							p[X]=p[X]-1;
			break;
		case DIREITA: p[Y]=p[Y]+1; // move particula para direita//
			break;
		case DIREITA_BAIXO:	p[Y]=p[Y]+1; // move particula para direita + baixo//
							p[X]=p[X]+1;
			break;
		case BAIXO: p[X]=p[X]+1; // move particula para baixo//
			break;
		case ESQUERDA_BAIXO: p[Y]=p[Y]-1; // move particula para esquerda + baixo//
							p[X]=p[X]+1;
			break;
	}
	p[FLAG]=1;//Bloqueio da Particula
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
			if(particula[p][FLAG] == 0 && particula[i][FLAG]==0){//verifica se ambas particulas estao prontas para se mover 
					//caso possam se decide se hou colisao ou nao
					if ((particula[i][X] - particula[i][RAIO] <= particula[p][X] + particula[p][RAIO] && particula[i][X] + particula[i][RAIO] >= particula[p][X] - particula[p][RAIO]) && (particula[i][Y] - particula[i][RAIO] <= particula[p][Y] + particula[p][RAIO] && 	particula[i][Y] + particula[i][RAIO] >= particula[p][Y] - particula[p][RAIO]) && i != p){
						somaDirecao(&particula[p][DIRECAO],&particula[i][DIRECAO]);//atribui as novas direcoes
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
		particula[k][DIRECAO]=direcao++;
		if (direcao > 8)
			direcao=1;
		//particula[k].cor=0;
		particula[k][RAIO]=6;
		particula[k][FLAG]=0;
	}
	k=0;	
	for(int i=10;i<linhas;i+=25){
		if(k == N)
			break;
		for(int j=10;j<colunas;j+=25){
			if(k == N)
				break;
			else{
				particula[k][X]=i;
				particula[k][Y]=j;
				k++;
			}	
		}
	}
}


/*================================= DIREITABORDA =================================*/
//Verifica se a particula colidiu com a borda e aplica o uma nova direcao//
void detectaBorda(int i){
	if(particula[i][X] > linhas - particula[i][RAIO]){
		switch(particula[i][DIRECAO]){
			case DIREITA_BAIXO: particula[i][DIRECAO]=DIREITA_CIMA; break;
			case BAIXO: particula[i][DIRECAO]=CIMA; break;
			case ESQUERDA_BAIXO: particula[i][DIRECAO]=ESQUERDA_CIMA; break;
			default: break;
		}
	}
	if(particula[i][X] <= 0 + particula[i][RAIO] ){
		switch(particula[i][DIRECAO]){
			case ESQUERDA_CIMA: particula[i][DIRECAO]=ESQUERDA_BAIXO; break;
			case CIMA: particula[i][DIRECAO]=BAIXO; break;
			case DIREITA_CIMA: particula[i][DIRECAO]=DIREITA_BAIXO; break;
			default: break;
		}
	}
	if(particula[i][Y] > colunas - particula[i][RAIO]){
		switch(particula[i][DIRECAO]){
			case DIREITA_CIMA: particula[i][DIRECAO]=ESQUERDA_CIMA; break;
			case DIREITA: particula[i][DIRECAO]=ESQUERDA; break;
			case DIREITA_BAIXO: particula[i][DIRECAO]=ESQUERDA_BAIXO; break;
			default: break;
		}
	}
	if(particula[i][Y] <= 0 + particula[i][RAIO] ){
		switch(particula[i][DIRECAO]){
			case ESQUERDA: particula[i][DIRECAO]=DIREITA; break;
			case ESQUERDA_CIMA: particula[i][DIRECAO]=DIREITA_CIMA; break;
			case ESQUERDA_BAIXO: particula[i][DIRECAO]=DIREITA_BAIXO; break;
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
		particula[i][FLAG]=0;
}


int main(int argc, char** argv)
{
  MPI_Status status;
  int me,pr;
  int i,j,k;


N=atoi(argv[1]);
srand(time(NULL));
//Mat image(linhas,colunas,CV_8UC3,Scalar(255,255,255));
linhas=N;
colunas=5;


  /* Start up MPI */

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);
  MPI_Comm_size(MPI_COMM_WORLD, &pr);

  printf("me=%d, p=%d\n", me, pr);
  	inicializa();
	insere();

/* Data distribution */
  if (me == 0) // master
  {

      // assume p = 2
      for (i=1; i<pr; i++)
      {
          printf("send to  %d with data from: %d and size:%d \n", i, (i)*linhas/pr, linhas*colunas/pr);
          MPI_Send(&particula[i * linhas / pr][0], linhas * colunas / pr, MPI_INT, i, 0, MPI_COMM_WORLD);
      }
  }
  else
  {
      printf("Recv from %d with data from: %d and size:%d \n", 0, (me)*linhas/pr, linhas*linhas/pr);
      MPI_Recv(&particula[me * linhas / pr][0], linhas * colunas / pr, MPI_INT, i, 0, MPI_COMM_WORLD, 0);

  }


  gettimeofday(&inicioTempo,NULL);
  int ciclos=1000;

	while (colidiu<ciclos) // Loop demorado * aumentando numero de particulas //
    {


		for(i=0; i<N;i++){
			detectaBorda(i);
			if (!colisao(i))
				movimenta(particula[i]);
			detectaBorda(i);
		}	




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
	if (me != 0 )
	{
	      MPI_Send(&particula[me * linhas/pr][0], linhas*colunas/pr, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		for (i=1; i<pr; i++)
		{
	    	MPI_Recv(&particula[i * linhas/pr][0], linhas*colunas/pr, MPI_INT, i, 0, MPI_COMM_WORLD,0); //AKI//
	    }
	}

  	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();
	gettimeofday(&fimTempo,NULL);
	printf("Ciclos:%d N:%d Linhas:%d Colunas:%d \n",ciclos,N,linhas,colunas);
	printf("Tempo de operacao = %ld microsegundo(s); %ld milisegundo(s); %ld segundo(s)\n",((fimTempo.tv_sec*1000000+fimTempo.tv_usec)-(inicioTempo.tv_sec*1000000+inicioTempo.tv_usec)),((fimTempo.tv_sec*1000000+fimTempo.tv_usec)-(inicioTempo.tv_sec*1000000+inicioTempo.tv_usec))/1000,((fimTempo.tv_sec*1000000+fimTempo.tv_usec)-(inicioTempo.tv_sec*1000000+inicioTempo.tv_usec))/1000000);	
	return 0;

}
