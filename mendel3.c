/*
**  PROGRAM: Mandelbrot area
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           Correct answer should be around 1.510659.
**           WARNING: this program may contain errors
**
**  USAGE:   Program runs without input ... just run the executable
**            
**  HISTORY: Written:  (Mark Bull, August 2011).
**           Changed "comples" to "d_comples" to avoid collsion with 
**           math.h complex type (Tim Mattson, September 2011)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

# define NPOINTS 1000
# define MAXITER 1000


struct d_complex{
   double r;
   double i;
};
void static inline testpoint(struct d_complex c,int *numoutside);

struct d_complex c,c2;
int numoutside = 0, numoutside2 = 0;

int main(){
   int i, j=0;
   double area, error, eps  = 1.0e-5, a = 1.0/(double)(NPOINTS);


//ERROS E SUAS CORRECOES//
//privatiza eps e nao coloca valor para dentro dela. Compartilhar ela; sem necessidade de privatizar//
//variavel j eh declarada fora, e tambem eh compartilhada. Logo j eh problema. Entao j deve ser privatizado//
//parallel for ja privatiza i//
//passa variavel c como parametro//
//Linha 90 colocar break//




//   Loop over grid of points in the complex plane which contains the Mandelbrot set,
//   testing each point to see whether it is inside or outside the set.

//private(c,j)Declara um ou mais itens da lista como privados para uma tarefa
//schedule(dynamic):cada thread executa um conjunto de iteracoes
/*reduction(+:numoutside):Declara acumulação nos itens da lista usando o operador associativo indicado. 
A acumulação ocorre em uma cópia privada para cada item da lista que é então combinado com
o item original*/

#pragma omp parallel for schedule(dynamic) default(shared) private(c,j) reduction(+:numoutside) num_threads(4)
   for (i=0; i<NPOINTS; i+=1) {
     for (j=0; j<NPOINTS; j+=1) {
       c.r = -2.0+2.5*(double)(i)*a+eps;
       c.i = 1.125*(double)(j)*a+eps;
       testpoint(c,&numoutside);
     }
   }
   
// Calculate area of set and error estimate and output the results
   
area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS-numoutside)/(double)(NPOINTS*NPOINTS);
   error=area*a;

   printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n",area,error);
   printf("Correct answer should be around 1.510659\n");

}

void static inline testpoint(struct d_complex c, int *numoutside){

// Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside set
// If loop count reaches MAXITER, point is considered to be inside the set

       struct d_complex z;
       int iter;
       double temp;

       z=c;
       for (iter=0; iter<MAXITER; iter++){
         temp = (z.r*z.r)-(z.i*z.i)+c.r;
         z.i = z.r*z.i*2+c.i;
         z.r = temp;
         if ((z.r*z.r+z.i*z.i)>4.0) {
           *numoutside+=1;
           break;
         }
       }

}

