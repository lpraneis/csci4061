#ifndef MAT_H
#define MAT_H 1

double **outer_product(double *x, int nx, double *y, int ny);

void free_matrix(double **mat, int nrows);

#endif
