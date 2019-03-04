// A C program that does not terminate from an interrupt signal.
// Usually pressing Ctrl-C sends this to the foreground program.
//
// To stop this program from running, open another terminal and try
//  > pkill -9 a.out
// assuming you named the output program a.out
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Function run when a SIGINT is sent to the program
void handle_SIGSTOP(int sig_num) {
  // Reset handler to catch SIGINT next time.
  signal(SIGSTOP, handle_SIGSTOP); 
  printf("\nThere's no SIGSTOPping me!\n");
  fflush(stdout);
}

int main () {
  // Set handling functions for programs
   signal(SIGSTOP, handle_SIGSTOP); 
 
  /* Infinite loop */
  while(1) {        
    sleep(1);
    printf("I'm awake, I'm awake!\n");
    fflush(stdout);
  }
  return 0;
}
