#include "mat.h"
#include <stdlib.h>

void free_matrix(double **mat, int nrows){
  /* Freeing the dynamically allocated matrix means... */
  int i;
 for(i=0; i<nrows; i++){
    free(mat[i]);		/* Free each row */
  }
  free(mat);		/* Free pointers to rows */
  return;
}
