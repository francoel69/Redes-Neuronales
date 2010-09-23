#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "ran2.h"
#include "matrix.h"

#define MAX_ITER 30
#define N 832	//Cantidad de neuronas;
#define PMAX (N/2)
#define FIRING 1
#define NOT_FIRING -1

float uniform_gen(void);
matrix memories(matrix xi, int p);
// void synapses(matrix w, const matrix xi, int P);
matrix calcular_m(const matrix s, const matrix xi, matrix m, int p);
matrix actualizar_m(int dif, int i, const matrix xi, matrix m, int p);
int firing(void);
matrix initialize(matrix s, const matrix xi);
int compare_vect(const matrix s, const matrix saux);
int neuron_d(const matrix xi, const matrix m, const matrix s,int i, int p);

int main(void){
  matrix m, xi, s, saux;
  int i= 0, p= 0, k= 0, m_medio= 0, dif= 0;

//   w= create_matrix(N, N);
  s= create_matrix(N, 1);
  saux= create_matrix(N, 1);

  for(p=40;p<=PMAX;p+=4){
    m= create_matrix(p, 1);
    xi= create_matrix(N, p);
  
    m_medio= 0;
    for(k=0;k<MAX_ITER;k++){
      xi= memories(xi, p);
      initialize(s, xi);
      m= calcular_m(s, xi, m, p);
      while(compare_vect(s, saux)){
	copy_matrix(s,saux);

	for(i=0;i<N;i++){
	  s= set(i, 0, neuron_d(xi,m,s,i,p), s);
	  dif= get(i,0,s)-get(i,0,saux);
	  m= actualizar_m(dif, i, xi, m, p);
	}
      }
      m_medio += abs(get(0,0, m));
    }
    printf("%f\t%f\n", ((float)p)/N, ((float)m_medio)/(N*MAX_ITER));

    destroy_matrix(xi);
    destroy_matrix(m);
  }
//     destroy_matrix(w);
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
/*
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
}*/

matrix calcular_m(const matrix s, const matrix xi, matrix m, int p){
  int mu= 0, j= 0, sum= 0;

  for(mu=0;mu<p;mu++){
    sum= 0;
    for(j=0;j<N;j++){
      sum += get(j,0,s)*get(j,mu,xi);
    }
    m= set(mu,0,sum,m);
  }
  return m;
}

matrix actualizar_m(int dif, int i, const matrix xi, matrix m, int p){
  int mu= 0;

  for(mu=0;mu<p;mu++){
      m= set(mu,0,get(mu,0,m)+get(i,mu,xi)*dif,m);
  }
  return m;
}

/* Valor inicial del S. */
matrix initialize(matrix s, const matrix xi){
  int i= 0;

  for(i=0;i<N;i++){
    if(uniform_gen() < 1.){
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
int neuron_d(const matrix xi, const matrix m, const matrix s,int i, int p){
    int h = 0.;
    int mu= 0;

    for (mu=0; mu<p; mu++){
        h += get(i,mu,xi) * get(mu,0,m);
    }
    h = h-p*get(i,0,s);
    return h < 0. ? NOT_FIRING : FIRING;
}
