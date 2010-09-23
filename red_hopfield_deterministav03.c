#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "ran2.h"
#include "matrix.h"

#define MAX_ITER 10
#define N 416	//Cantidad de neuronas;
#define PMAX (N/2)
#define FIRING 1
#define NOT_FIRING -1

float uniform_gen(void);
matrix memories(matrix xi, int p);
void synapses(matrix w, const matrix xi, int P);
int firing(void);
matrix initialize(matrix s, const matrix xi);
int compare_vect(const matrix s, const matrix saux);
int neuron_d(const matrix w, const matrix s, int i);

int main(void){
  matrix w, xi, s, saux;
  int i= 0, j= 0, m= 0, p= 0, k= 0, m_medio= 0;

  w= create_matrix(N, N);
  s= create_matrix(N, 1);
  saux= create_matrix(N, 1);

  for(p=4;p<=PMAX;p+=4){
    xi= create_matrix(N, p);
  
    m_medio= 0;
    for(k=0;k<MAX_ITER;k++){
      xi= memories(xi, p);
      synapses(w, xi, p);
      initialize(s, xi);
      while(compare_vect(s, saux)){
	copy_matrix(s,saux);

	for(i=0;i<N;i++){
	  s= set(i, 0, neuron_d(w,s,i), s);
	}
      }
      m= 0;
      for(j=0;j<N;j++){
	m += get(j,0,s)*get(j,0,xi);
      }
      m_medio += abs(m);
    }
    printf("%f\t%f\n", ((float)p)/N, ((float)m_medio)/(N*k));

    destroy_matrix(xi);
  }
    destroy_matrix(w);
    destroy_matrix(s);
    destroy_matrix(saux);

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
matrix memories(matrix xi, int p){
  int m=0, j=0;
  for(j=0;j<N;j++){
    for(m=0;m<p;m++){
      xi = set(j, m, firing(), xi);
    }
  }
  return xi;
}

/* Calcula la matriz sinaptica. */
void synapses(matrix w, const matrix xi, int p){
  int i= 0, j= 0, mu= 0, a= 0;

  for(i=0;i<N;i++){
    w= set(i,i,0,w);
    for(j=0;j<i;j++){
      a= 0;
      for(mu=0;mu<p;mu++){
        a+= get(i,mu,xi)*get(j,mu,xi);
      }
      w= set(i,j,a,w);
      w= set(j,i,a,w);
    }
  }
}

/* Valor inicial del S. */
matrix initialize(matrix s, const matrix xi){
  int i= 0;

  for(i=0;i<N;i++){
    if(uniform_gen() < .85){
      s= set(i, 0, get(i, 0, xi), s);
    }else{
      s= set(i, 0, -get(i, 0, xi), s);
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

/* Deterministica. */
int neuron_d(const matrix w, const matrix s, int i){
    float h = 0.;
    int j= 0;

    for (j=0; j<N; j++){
        h += get(i,j,w) * get(j,0,s);
    }
    return h < 0. ? NOT_FIRING : FIRING;
}
