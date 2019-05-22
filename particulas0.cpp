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

#define ESQUERDA 1
#define ESQUERDA_CIMA 2
#define CIMA 3
#define DIREITA_CIMA 4
#define DIREITA 5
#define DIREITA_BAIXO 6
#define BAIXO 7
#define ESQUERDA_BAIXO 8


 // mudanca de cor nas particulas quando se colidem //

using namespace std;
using namespace cv;

 // numero de particulas no ambiente

unsigned long long linhas,colunas,N,colidiu=0;
int num_procs;

struct timeval inicioTempo,fimTempo;

typedef struct T_particula
{
	int x,y; // posicao//
	int direcao; //direcao//
	int raio; //raio//
	int cor; //cor//
	int flag;

}T_particula;

T_particula *particula;


/*================================= INICIALIZA =================================*/

void inicializa()
{

	particula=(T_particula*)malloc(N*sizeof(T_particula)); //aloca posicao inicial na lista//
	if(!particula)
	{
		printf("\n Não existe espaço na memoria \n");
	}
}

void movimenta(T_particula *p){
	switch(p->direcao){
		case ESQUERDA: p->y=p->y-1;  // particula para esquerda//
			break;
		case ESQUERDA_CIMA:	p->y=p->y-1; // particula para esquerda + cima//
							p->x=p->x-1;
			break;
		case CIMA:	p->x=p->x-1; // particula para cima//
			break;
		case DIREITA_CIMA:	p->y=p->y+1; // particula para direita + cima//
							p->x=p->x-1;
			break;
		case DIREITA: p->y=p->y+1; // particula para direita//
			break;
		case DIREITA_BAIXO:	p->y=p->y+1; // particula para direita + baixo//
							p->x=p->x+1;
			break;
		case BAIXO: p->x=p->x+1; // particula para baixo//
			break;
		case ESQUERDA_BAIXO: p->y=p->y-1; // particula para esquerda + baixo//
							p->x=p->x+1;
			break;
	}
	p->flag=1;
}


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
/*
#define E 1
#define EC 2
#define C 3
#define DC 4
#define D 5
#define DB 6
#define B 7
#define EB 8
*/
/*================================= INICIALIZA =================================*/
int colisao(unsigned long long p){
	unsigned long long i;
	for(i=0;i<N;i++){
		{
			if(particula[p].flag == 0 && particula[i].flag==0){
					if ((particula[i].x - particula[i].raio <= particula[p].x + particula[p].raio && particula[i].x + particula[i].raio >= particula[p].x - particula[p].raio) && (particula[i].y - particula[i].raio <= particula[p].y + particula[p].raio && 	particula[i].y + particula[i].raio >= particula[p].y - particula[p].raio) && i != p){
						somaDirecao(&particula[p].direcao,&particula [i].direcao);
						movimenta(&particula[p]);
						movimenta(&particula[i]);
						return 1;
					}
				}
			}
	}
	return 0;
}

/*================================= INSERE =================================*/


void insere()
{
	//int limite=5; // numero de particulas vermelhas iniciais //	
	int direcao=1;	
		unsigned long long k;
	for(k=0;k<N;k++){
		particula[k].direcao=direcao++;
		if (direcao > 8)
			direcao=1;
		particula[k].cor=0;
		particula[k].raio=6;
		particula[k].flag=0;
	}
	particula[k-1].cor=255;
	k=0;	
		for(	unsigned long long i=10;i<linhas;i+=25){
			if(k > N){
					break;
				}	
			for(	unsigned long long j=10;j<colunas;j+=25){
				if(k > N){
					break;
				}
				particula[k].x=i;
				particula[k].y=j;
				k++;	
			}
		}

}


/*================================= INSERE =================================*/

void parede(unsigned long long i){
	if(particula[i].x > linhas - particula[i].raio){
		switch(particula[i].direcao){
			case DIREITA_BAIXO: particula[i].direcao=DIREITA_CIMA; break;
			case BAIXO: particula[i].direcao=CIMA; break;
			case ESQUERDA_BAIXO: particula[i].direcao=ESQUERDA_CIMA; break;
			default: break;
		}
	}
	if(particula[i].x <= 0 + particula[i].raio ){
		switch(particula[i].direcao){
			case ESQUERDA_CIMA: particula[i].direcao=ESQUERDA_BAIXO; break;
			case CIMA: particula[i].direcao=BAIXO; break;
			case DIREITA_CIMA: particula[i].direcao=DIREITA_BAIXO; break;
			default: break;
		}
	}
	if(particula[i].y > colunas - particula[i].raio){
		switch(particula[i].direcao){
			case DIREITA_CIMA: particula[i].direcao=ESQUERDA_CIMA; break;
			case DIREITA: particula[i].direcao=ESQUERDA; break;
			case DIREITA_BAIXO: particula[i].direcao=ESQUERDA_BAIXO; break;
			default: break;
		}
	}
	if(particula[i].y <= 0 + particula[i].raio ){
		switch(particula[i].direcao){
			case ESQUERDA: particula[i].direcao=DIREITA; break;
			case ESQUERDA_CIMA: particula[i].direcao=DIREITA_CIMA; break;
			case ESQUERDA_BAIXO: particula[i].direcao=DIREITA_BAIXO; break;
			default: break;
		}
	}
}

/*================================= ATUALIZA =================================*/

void atualiza()
{
	unsigned long long i;

	for(i=0; i<N;i++){
		parede(i);
		if (!colisao(i))
			movimenta(&particula[i]);
		parede(i);
	}	

}

void libera(){
	unsigned long long i;
	for(i=0; i<N;i++)
			particula[i].flag=0;
	
}




/*================================= ATUALIZA =================================*/

int main(int argc, char** argv)
{
	N=atoi(argv[1]);
	linhas=atoi(argv[2]);
	colunas=atoi(argv[3]);
	srand (time(NULL));
	inicializa();
	insere();

	gettimeofday(&inicioTempo,NULL);
	int ciclos=1;
	while (colidiu<ciclos) // Loop demorado * aumentando numero de particulas//
    	{
		atualiza();
		libera();
		/*Mat image(linhas,colunas,CV_8UC3,Scalar(255,255,255));
		
		for(i=0;i < N;i++){			
		   circle( image, Point(particula[i].y,particula[i].x ), particula[i].raio, Scalar(0,0,particula[i].cor),-1, 8);
	  	}
		
		cv::resize(image, image, cv::Size(), 0.25, 0.25);		
        	cv::imshow("Imagem", image);
        	cv::waitKey(1);*/
		colidiu++;
	}
	gettimeofday(&fimTempo,NULL);
	printf("Ciclos:%d N:%llu",ciclos,N);
	printf("Tempo de operacao = %ld microsegundo(s); %ld milisegundo(s); %ld segundo(s)",((fimTempo.tv_sec*1000000+fimTempo.tv_usec)-(inicioTempo.tv_sec*1000000+inicioTempo.tv_usec)),((fimTempo.tv_sec*1000000+fimTempo.tv_usec)-(inicioTempo.tv_sec*1000000+inicioTempo.tv_usec))/1000,((fimTempo.tv_sec*1000000+fimTempo.tv_usec)-(inicioTempo.tv_sec*1000000+inicioTempo.tv_usec))/1000000);	
	return 0;

}
