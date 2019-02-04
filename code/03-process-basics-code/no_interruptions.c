// A C program that does not terminate from an interrupt signal is sent
// Usually pressing Ctrl+C sends this to the foreground program
//
// To stop this program from running, open another terminal and try
//  > pkill -9 a.out
// assuming you named the output program a.out
#include <stdio.h>
#include <signal.h>
#include <limits.h>

// Function run when a SIGINT is sent to the program
void handle_SIGINT(int sig_num) {
  // Reset handler to catch SIGINT next time.
  signal(SIGINT, handle_SIGINT); 
  printf("\nNo SIGINT-erruptions allowed.\n");
  fflush(stdout);
}
 
// Function run when a SIGTERM is sent to the program
void handle_SIGTERM(int sig_num) {
  // Reset handler to catch SIGTERM next time.
  signal(SIGTERM, handle_SIGTERM); 
  printf("\nTry to SIGTERM me? Piss off!\n");
  fflush(stdout);
}

int main () {
  // Set handling functions for programs
  signal(SIGINT, handle_SIGINT); 
  signal(SIGTERM, handle_SIGTERM);
 
  /* Infinite loop */
  unsigned int count = 0;
  while(1) {        
    if(count == 0){
      printf("Ma-na na-na!\n");
    }
    count++;
  }
  return 0;
}
