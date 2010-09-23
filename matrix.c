#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "matrix.h"

struct matrix_s {
    int **xi;
    int row;
    int col;
};

matrix create_matrix (int n, int m){
  int a= 0;
  matrix mat= NULL;

  assert(n > 0 && m > 0);

  mat= (matrix)malloc(sizeof(struct matrix_s));
  
  assert(mat != NULL);
  
  mat->xi= (int **)malloc(sizeof(int *)*n);
  
  assert(mat->xi != NULL);

  for(a=0;a<n;a++){
    mat->xi[a]= (int *)malloc(sizeof(int)*m);
    assert(mat->xi[a] != NULL);
  }

  mat->row= n;
  mat->col= m;

  
  return mat;
}

matrix set(int i, int j, int k, matrix mat){
  
  assert(mat != NULL);
  assert(0<=i && i<mat->row && 0<=j && j<mat->col);
  
  mat->xi[i][j]= k;

  return mat;
}

int get(int i, int j, matrix mat){
  
  assert(mat != NULL);
  assert(0<=i && i<mat->row && 0<=j && j<mat->col);
  
  return mat->xi[i][j];
}

void copy_matrix(matrix mat1, matrix mat2){
  int r= 0, c= 0;

  assert(mat1 != NULL && mat2 != NULL);
  assert((mat1->row == mat2->row) && (mat1->col == mat2->col));

  for(r=0;r<mat1->row;r++){
    for(c=0;c<mat1->col;c++){
      mat2->xi[r][c]= mat1->xi[r][c];
    }
  }
}

matrix destroy_matrix (matrix mat){
  int a= 0;
  
  assert(mat != NULL);

  for(a=0;a<mat->row;a++){
      free(mat->xi[a]);
      mat->xi[a]= NULL;
  }
  free(mat->xi);
  mat->xi = NULL;
  free(mat);
  mat = NULL;
 
  return mat;
}
