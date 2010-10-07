#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "ran2.h"
#include "matrix.h"

#define MAX_ITER 30
#define N 416 //Cantidad de neuronas;
#define P 15
#define FIRING 1
#define NOT_FIRING -1

float uniform_gen(void);
matrix memories(matrix xi);
matrix calcular_m(const matrix s, const matrix xi, matrix m);
matrix actualizar_m(int dif, int i, const matrix xi, matrix m);
int firing(void);
matrix initialize(matrix s, const matrix xi, int mu);
int compare_vect(const matrix s, const matrix saux);
int neuron_s(const matrix xi, const matrix m, const matrix s,int i, float t);

int main(void){
  matrix m, xi, s, saux;
  int i= 0, j= 0, k= 0, m_medio= 0, dif= 0, varianza= 0;
  float mean= 0., desv= 0., t= 0.;

  s= create_matrix(N, 1);
  saux= create_matrix(N, 1);
  m= create_matrix(P, 1);
  xi= create_matrix(N, P);


  for(t=0.1;t<=1.21;t+=0.1){
    m_medio= 0;
    varianza= 0;
    for(j=0;j<MAX_ITER;j++){
      xi= memories(xi);
      initialize(s, xi, 0);
      m= calcular_m(s, xi, m);
      for(k=0;k<MAX_ITER;k++){
	copy_matrix(s,saux);

	for(i=0;i<N;i++){
	  s= set(i, 0, neuron_s(xi,m,s,i,t), s);
	  dif= get(i,0,s)-get(i,0,saux);
	  m= actualizar_m(dif, i, xi, m);
	}
      }
      for(k=0;k<MAX_ITER;k++){
	copy_matrix(s,saux);

	for(i=0;i<N;i++){
	  s= set(i, 0, neuron_s(xi,m,s,i,t), s);
	  dif= get(i,0,s)-get(i,0,saux);
	  m= actualizar_m(dif, i, xi, m);
	}
	m_medio += abs(get(0,0, m));
	varianza += get(0,0, m)*get(0,0, m);
      }
    }
    mean= ((float)m_medio)/(N*MAX_ITER*MAX_ITER);
    desv= sqrtf((((float)varianza)/((double)N*N*MAX_ITER*MAX_ITER))-(float)mean*mean);
    printf("%f\t%f\t%f\n", t, mean,desv);
  }
  destroy_matrix(s);
  destroy_matrix(saux);
  destroy_matrix(xi);
  destroy_matrix(m);

  return 0;
}

/* Generador de numeros aleatorios entre 0 y 1. */
float uniform_gen(void){
  return ran2(&seed);
}

/* Devuelve 1 o -1 aleatoriamente, con igual probabilidad. */
int firing(void){
  float f = uniform_gen();
  return f>=0.5 ? FIRING : NOT_FIRING;
}

/* Rellena la matriz xi con 1 o -1 aleatoriamente. */
matrix memories(matrix xi){
  int mu=0, j=0;
  for(j=0;j<N;j++){
    for(mu=0;mu<P;mu++){
      xi = set(j, mu, firing(), xi);
    }
  }
  return xi;
}

matrix calcular_m(const matrix s, const matrix xi, matrix m){
  int mu= 0, j= 0, sum= 0;

  for(mu=0;mu<P;mu++){
    sum= 0;
    for(j=0;j<N;j++){
      sum += get(j,0,s)*get(j,mu,xi);
    }
    m= set(mu,0,sum,m);
  }
  return m;
}

matrix actualizar_m(int dif, int i, const matrix xi, matrix m){
  int mu= 0;

  for(mu=0;mu<P;mu++){
      m= set(mu,0,get(mu,0,m)+get(i,mu,xi)*dif,m);
  }
  return m;
}

/* Valor inicial del S. Inicializa al s en el patron mu. */
matrix initialize(matrix s, const matrix xi, int mu){
  int i= 0;

  for(i=0;i<N;i++){
    if(uniform_gen() < 1.){
      s= set(i, 0, get(i, mu, xi), s);
    }else{
      s= set(i, 0, -get(i, mu, xi), s);
    }
  }
  return s;
}

/* Devuelve 0 si son iguales. */
int compare_vect(const matrix s, const matrix saux){
  int i= 0;

  while(i<N && (get(i,0,s) == get(i,0,saux))){
      i++;
  }
  return !(i==N);
}

/* Estocástica. */
int neuron_s(const matrix xi, const matrix m, const matrix s, int i, float t){
  int mu= 0;
  float h= 0., pro= 0.;
  float unif= uniform_gen();

  for (mu=0; mu<P; mu++){
    h += get(i,mu,xi) * get(mu,0,m);
  }
  h = (h-P*get(i,0,s))/(float)N;
  pro= (1.+tanhf(h/t))/2.;
  return unif < pro ? FIRING : NOT_FIRING;
}
