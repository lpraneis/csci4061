// Start two child processes which run sleep_print with differing
// delays and messages. Their output is redirected through pipes
// readable by the parent.  Use select() to grab input from either
// pipe as it becomes available.  Handle interrupt and terminate
// signals gracefully by setting a flag to shut down.

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
#include <errno.h>

#define PREAD  0                                                     // read and write ends of pipes
#define PWRITE 1

int signaled = 0;                                                    // global variable controls exit from main loop

void handle_signals(int signo){                                      // handler for some signals 
  char *msg = "select_AB: signaled, setting flag\n";                 // print a message about the signal 
  write(STDERR_FILENO,msg,strlen(msg));                              // avoid fprintf() as it is not reentrant
  signaled = 1;                                                      // set global variable to indicate signal received
  return;
}

int make_child(int pip[], char *delay, char *msg){                   // Create a child which runs sleep_print
  pipe(pip);
  int pid = fork();
  if(pid != 0){                                                      // PARENT
    return pid;                                                      // just return child pid
  }
  dup2(pip[PWRITE], STDOUT_FILENO);                                  // CHILD: redirect standard out to pipe
  execlp("./sleep_print","./sleep_print",delay,msg,NULL);            // should not return
  perror("exec failed");
  exit(1);
}

int main() {
  struct sigaction my_sa = {};                                       // portable signal handling setup with sigaction()
  my_sa.sa_handler = handle_signals;                                 // run function handle_signals
  sigemptyset(&my_sa.sa_mask);                                       // don't block any other signals during handling
  my_sa.sa_flags = SA_RESTART;                                       // always restart system calls on signals possible 
  sigaction(SIGTERM, &my_sa, NULL);                                  // register SIGTERM with given action
  sigaction(SIGINT,  &my_sa, NULL);                                  // register SIGINT with given action

  int pipeA[2], pipeB[2];                                            // pipes for children to speak to parent
  int pidA = make_child(pipeA, "1", "AAAA");                         // create two children with different
  int pidB = make_child(pipeB, "3", "BBBB");                         // messages and delays between prints

  close(pipeA[PWRITE]);                                              // Parent doesn't use the write ends
  close(pipeB[PWRITE]);

  printf("select_AB: listening for children\n");                         

  while(!signaled){                                                  // enter a loop to listen until signaled or children die
    fd_set read_set;                                                 // set of file descriptors for select()
    FD_ZERO(&read_set);                                              // init the set
    FD_SET(pipeA[PREAD], &read_set);                                 // set fds to contain read ends of pipe
    FD_SET(pipeB[PREAD], &read_set);
    int maxfd = pipeA[PREAD];                                        // need maximum fd for select()
    maxfd = (maxfd < pipeB[PREAD]) ? pipeB[PREAD] : maxfd;

    select(maxfd+1, &read_set, NULL, NULL, NULL);                    // sleep, wake up if any pipe ready for reading

    char buf[1024];
    int n;
    if(FD_ISSET(pipeA[PREAD], &read_set)){                           // check if pipeA has anything
      n = read(pipeA[PREAD], buf, 1024);                             // read from pipe
      buf[n] = '\0';
      fprintf(stdout,"A had: |%s|\n",buf);
    }
    if(FD_ISSET(pipeB[PREAD], &read_set)){                           // check if pipeB has anything
      n = read(pipeB[PREAD], buf, 1024);                             // read from pipe
      buf[n] = '\0';
      fprintf(stdout,"B had: |%s|\n",buf);
    }
  }
  kill(pidA, SIGTERM);                                               // keyboard interrupt will signal children but a 
  kill(pidB, SIGTERM);                                               // 'kill select_AB' will not, so always signal them here

  fprintf(stderr,"select_AB: finishing\n");
  exit(0);
}
