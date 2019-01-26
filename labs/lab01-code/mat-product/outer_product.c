#include "mat.h"
#include <stdlib.h>

double **outer_product(double *x, int nx,
		       double *y, int ny){
  double **mat = (double **) malloc(sizeof(double *) * nx);
  int i,j;
  for(i=0; i<nx; i++){
    mat[i] = (double *) malloc(sizeof(double) * ny);
  }
  
  for(i=0; i<nx; i++){
    for(j=0; j<ny; j++){
      mat[i][j] = x[i] * y[j];
    }
  }

  return mat;
}
