#include <stdio.h>
#include "collatz.h"

int main(int argc, char **argv){
  int nstart;
  printf("Enter the starting integer: ");
  scanf("%d", &nstart);

  int nnext = collatz_next(nstart);
  printf("The next value in the Collatz sequence is %d\n",nnext);

  int verbose;
  printf("Show output of steps (0:NO, any other int: yes): ");
  scanf("%d", &verbose);

  int steps = collatz_steps(nstart,verbose);
  printf("The starting value %d converged to 1 in %d steps\n",
         nstart,steps);
  return 0;    
}

