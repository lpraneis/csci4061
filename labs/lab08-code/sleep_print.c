// sleep_print.c: In an infinite loop, sleep for an interval and then
// print a message.  Catch interrupt and terminate signals and cause
// the loop program to end by adjusting a global flag.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>

int signaled = 0;                                           // global variable controls exit from main loop
void handle_signals(int signo){                             // handler for some signals 
  char *msg = "sleep_print: signaled, setting flag\n";      // print a message about the signal 
  write(STDERR_FILENO,msg,strlen(msg));                     // avoid fprintf() as it is not reentrant
  signaled = 1;                                             // set global variable to indicate signal received
  return;
}

int main(int argc, char *argv[]){
  if(argc < 3){
    printf("usage: %s <delay> <message>\n", argv[0]);
    exit(1);
  }

  struct sigaction my_sa = {};                              // portable signal handling setup with sigaction()
  my_sa.sa_handler = handle_signals;                        // run function handle_signals
  sigemptyset(&my_sa.sa_mask);                              // don't block any other signals during handling
  my_sa.sa_flags = SA_RESTART;                              // always restart system calls on signals possible 
  sigaction(SIGTERM, &my_sa, NULL);                         // register SIGTERM with given action
  sigaction(SIGINT,  &my_sa, NULL);                         // register SIGINT with given action

  int delay = atoi(argv[1]);                                // sleep delay
  char *message = argv[2];                                  // message to print
  int count=1;
  while(!signaled){
    sleep(delay);
    printf("%2d : %s ",count,message);
    fflush(stdout);
    count++;
  }
  fprintf(stderr,"sleep_print: finishing\n");               // print to stderr in case stdout is going to a pipe
  return 0;
}
