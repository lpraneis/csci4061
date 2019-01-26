// Test the total memory available in a single malloc by repeatedly
// increasing the limit of the request

#include <stdio.h>
#include <stdlib.h>

int main(){
  long n = 1;                           // int cannot hold large enough numbes
  void *mem = NULL;                     // Pointer to memory
  while( (mem = malloc(n)) != NULL){    // allocate and check result
    printf("%12ld bytes: Success\n",n); // %ld to print a long, %d for int
    free(mem);                          // free last allocation
    n *= 2;                             // double size of next request
  }                                     // 
  printf("%12ld bytes: Fail\n",n);      // failed last allocation, no need to free
  n /= 2;                               // back up one step for max success

  long kb = n / 1024;                   // sizes of allocations
  long mb = kb / 1024;
  long gb = mb / 1024;
  
  printf("\n");
  printf("%12ld b  limit\n",n);         // Output human readable sizes
  printf("%12ld KB limit\n",kb);
  printf("%12ld MB limit\n",mb);
  printf("%12ld GB limit\n",gb);
  return 0;                             // return 0 to indicate succesful completion
}
