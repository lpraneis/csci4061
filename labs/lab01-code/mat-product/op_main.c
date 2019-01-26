#include <stdio.h>
#include <stdlib.h>
#include "mat.h"

int main(int argc, char **argv){
  if(argc < 3){
    printf("usage: %s {nx} {x1 x2 ...} {ny} {y1 y2 ...}\n",argv[0]);
    exit(1);
  }
  int i,j;
  int nx = atoi(argv[1]);
  double *x = (double *) malloc(sizeof(double) * nx);
  for(i=0; i<nx; i++){
    x[i] = atof(argv[i+2]);
  }
  int ny = atoi(argv[2+nx]);
  double *y = (double *) malloc(sizeof(double) * ny);
  for(i=0; i<ny; i++){
    y[i] = atof(argv[i+3+nx]);
  }

  double **mat = outer_product(x,nx, y,ny);
  for(i=0; i<nx; i++){
    for(j=0; j<ny; j++){
      printf("%8.2f ",mat[i][j]);
    }
    printf("\n");
  }

  /* Free memory */
  free(x);
  free(y);
  free_matrix(mat, nx);
  return 0;
}
