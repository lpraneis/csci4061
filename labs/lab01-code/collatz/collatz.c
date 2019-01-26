#include <stdio.h>
#include "collatz.h"

/* Compute the next step in the collatz sequence */
int collatz_next(int n){
  if(n < 1){
    return -1;
  }
  else if(n==1){
    return 1;
  }
  else if(n % 2 == 0){
    return n / 2;
  }
  else{
    return n*3 + 1;
  }
}

// Count how many steps are in the collatz sequence
int collatz_steps(int n, int print_output){
  int steps = 0;
  if(print_output){
    printf("Step %d: %d\n",steps,n);
  }
  while(n > 1){
    n = collatz_next(n);
    steps++;
    if(print_output){
      printf("Step %d: %d\n",steps,n);
    }
  }
  return steps;
}
