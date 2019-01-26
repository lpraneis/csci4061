#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "mat.h"

int failures = 0;
int testn = 0;

#define TOLERANCE 1e-4

#define GENTEST 0

/* Utility to print matrix of doubles */
void print_matrix(double **mat, int rows, int cols){
  int i,j;
  for(i=0; i<rows; i++){
    for(j=0; j<cols; j++){
      printf("%8.4f ",mat[i][j]);
    }
    printf("\n");
  }
}

/* Utility to print an array of doubles */
void print_array(double *a, int len){
  int i;
  for(i=0; i<len; i++){
    printf("%8.4f ",a[i]);
  }
  printf("\n");
}

/* Test the outer_product() function */
void test_outer_product(double *x, int nx, double *y, int ny, double **expect){
  testn++;
  printf("Test %3d : ",testn);
  double **actual = outer_product(x,nx,y,ny);
  int i,j;
  for(i=0; i<nx; i++){
    for(j=0; j<ny; j++){
      /* printf("Checking %d %d %lf %lf\n",i,j,actual[i][j],expect[i][j]); */
      if( fabs(actual[i][j]-expect[i][j]) > TOLERANCE ){
        printf("FAILED\n", testn);
        printf("Difference at element (%d,%d) of output matrix\n",i,j);
        printf("x[%d] = ",nx); print_array(x,nx);
        printf("y[%d] = ",ny); print_array(y,ny);
        printf("Expect:\n");
        print_matrix(expect,nx,ny);
        printf("Actual:\n");
        print_matrix(actual,nx,ny);
        failures++;
        return;
      }
    }
  }
  printf("passed\n");
}


int main(int argc, char **argv){
  { int nx = 1;
    double x[1] = {2.5, };
    int ny = 1;
    double y[1] = {2.0, };
    double expectM[1][1] = {
      {   5.00, },
    };
    double *expect[nx];
    int i;
    for(i=0; i<nx; i++){
      expect[i] = expectM[i];
    }
    test_outer_product(x,nx,y,ny,expect);
  }
  { int nx = 4;
    double x[4] = {1.0, 2.0, 3.0, 4.0, };
    int ny = 2;
    double y[2] = {5.0, 6.0, };
    double expectM[4][2] = {
      {   5.00,     6.00, },
      {  10.00,    12.00, },
      {  15.00,    18.00, },
      {  20.00,    24.00, },
    };
    double *expect[nx];
    int i;
    for(i=0; i<nx; i++){
      expect[i] = expectM[i];
    }
    test_outer_product(x,nx,y,ny,expect);
  }
  { int nx = 2;
    double x[2] = {5.0, 6.0, };
    int ny = 4;
    double y[4] = {1.0, 2.0, 3.0, 4.0, };
    double expectM[2][4] = {
      { 5.00,    10.00,    15.00,    20.00, },
      { 6.00,    12.00,    18.00,    24.00, },
    };
    double *expect[nx];
    int i;
    for(i=0; i<nx; i++){
      expect[i] = expectM[i];
    }
    test_outer_product(x,nx,y,ny,expect);
  }
  { int nx = 3;
    double x[3] = {1.0, 2.0, 3.0, };
    int ny = 4;
    double y[4] = {4.0, 5.0, 6.0, 7.0, };
    double expectM[3][4] = {
      {  4.00,     5.00,     6.00,     7.00, }, 
      {  8.00,    10.00,    12.00,    14.00, }, 
      { 12.00,    15.00,    18.00,    21.00, }, 
    };
    double *expect[nx];
    int i;
    for(i=0; i<nx; i++){
      expect[i] = expectM[i];
    }
    test_outer_product(x,nx,y,ny,expect);
  }
  { int nx = 4;
    double x[4] = {4.0, 5.0, 6.0, 7.0, };
    int ny = 3;
    double y[3] = {1.0, 2.0, 3.0, };
    double expectM[4][3] = {
      {    4.00,     8.00,    12.00, },
      {    5.00,    10.00,    15.00, },
      {    6.00,    12.00,    18.00, },
      {    7.00,    14.00,    21.00, },
    };
    double *expect[nx];
    int i;
    for(i=0; i<nx; i++){
      expect[i] = expectM[i];
    }
    test_outer_product(x,nx,y,ny,expect);
  }  
  { int nx = 8;
    double x[8] = {0.16229240, 0.00700059, 0.17871444, 0.02070065, 0.99401732, 0.01247836, 0.53994400, 0.66760519, };
    int ny = 11;
    double y[11] ={0.15270433, 0.52950222, 0.37289392, 0.04972703, 0.37773929, 0.33094647, 0.64669236, 0.61691215, 0.06093820, 0.71302022, 0.40533052, };
    double expectM[8][11] = {
      { 0.02478275, 0.08593419, 0.06051785, 0.00807032, 0.06130422, 0.05371010, 0.10495326, 0.10012016, 0.00988981, 0.11571777, 0.06578206, },
      { 0.00106902, 0.00370683, 0.00261048, 0.00034812, 0.00264440, 0.00231682, 0.00452723, 0.00431875, 0.00042660, 0.00499156, 0.00283755, },
      { 0.02729047, 0.09462969, 0.06664153, 0.00888694, 0.06750747, 0.05914491, 0.11557326, 0.11025111, 0.01089054, 0.12742701, 0.07243842, },
      { 0.00316108, 0.01096104, 0.00771915, 0.00102938, 0.00781945, 0.00685081, 0.01338695, 0.01277048, 0.00126146, 0.01475998, 0.00839061, },
      { 0.15179075, 0.52633438, 0.37066302, 0.04942953, 0.37547940, 0.32896652, 0.64282341, 0.61322137, 0.06057362, 0.70875445, 0.40290556, },
      { 0.00190550, 0.00660732, 0.00465310, 0.00062051, 0.00471357, 0.00412967, 0.00806966, 0.00769805, 0.00076041, 0.00889732, 0.00505786, },
      { 0.08245179, 0.28590155, 0.20134184, 0.02684981, 0.20395807, 0.17869256, 0.34917766, 0.33309802, 0.03290321, 0.38499099, 0.21885579, },
      { 0.10194620, 0.35349843, 0.24894592, 0.03319802, 0.25218071, 0.22094158, 0.43173518, 0.41185376, 0.04068266, 0.47601600, 0.27060076, },
    };
    double *expect[nx];
    int i;
    for(i=0; i<nx; i++){
      expect[i] = expectM[i];
    }
    test_outer_product(x,nx,y,ny,expect);
  }  
  { int nx = 4;
    double x[4] = {1.0, 2.0, 3.0, 4.0, };
    int ny = 1;
    double y[1] = {6.0, };
    double expectM[4][1] = {
      {   6.00, },
      {  12.00, },
      {  18.00, },
      {  24.00, },
    };
    double *expect[nx];
    int i;
    for(i=0; i<nx; i++){
      expect[i] = expectM[i];
    }
    test_outer_product(x,nx,y,ny,expect);
  }
  {
    int nx = 1;
    double x[1] = {6.0, };
    int ny = 5;
    double y[5] = {1.0, 2.0, 3.0, 4.0, 5.0 };
    double expectM[1][5] = {
      {   6.00, 12.00, 18.00, 24.00, 30.00, },
    };
    double *expect[nx];
    int i;
    for(i=0; i<nx; i++){
      expect[i] = expectM[i];
    }
    test_outer_product(x,nx,y,ny,expect);
  }

  printf("-----------------------------\n");
  printf("Overall: %d / %d tests passed\n",(testn-failures),testn);

  return failures;
}
