#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix_s *matrix;

matrix create_matrix (int n, int m);
/* 
*  PRE: n, m enteros mayores a 0.
*  Crea una matriz entero era de dimension nxm.
*  POST: Crea una matriz entera de dimension nxm.
*/
matrix set(int i, int j, int k, matrix mat);
/* 
*  PRE: debe estar definido el lugar (i,j) en mat.
*  Asigna el entero k al lugar (i,j).
*  POST: devuelve mat con el lugar (i,j) actualizado.
*/
int get(int i, int j, matrix mat);
/* 
*  PRE: debe estar definido el lugar (i,j) en mat.
*  Obtiene el valor guardado en (i,j).
*  POST: Devuelve el entero guardado en el lugar (i,j).
*/
void copy_matrix(matrix mat1, matrix mat2);
/* 
*  PRE: Las matrices deben ser de igual dimension.
*  Copia los elementos de la matriz 1 en la matriz 2.
*  POST: mat1 y mat2 tienen los mismos elementos.
*/
matrix destroy_matrix (matrix mat);
/* 
*  PRE: mat != NULL.
*  Libera el espacio de memoria asignado a la matriz mat.
*  POST: devuelve una matriz nula.
*/
#endif /*MATRIX_H*/
