#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
  if(argc < 3){
    printf("usage: %s <program> <int>\n",argv[0]);
    exit(0);
  }
  char *child_prog = argv[1];                // program to run as a child
  int lifetime = atoi(argv[2]);              // time to wait to end child

  int pid = fork();                          // fork execution

  if(pid == 0){                              // CHILD
    execlp(child_prog, child_prog, NULL);    // execute specified program
  }

  int status, ret;                           // PARENT
  while(1){                                  // Loop until child done
    sleep(lifetime);                         // sleep for given "lifetime" of child
    ret = waitpid(pid, &status, WNOHANG);    // check on child
    if(ret == pid){                          // if child is finished
      break;                                 //   break from loop
    }
    int result = kill(pid,SIGINT);           // send a interrupt signal to child
    printf("kill result: %d\n",result);      // check on delivery
  }

  if(WIFSIGNALED(status)){                   // check if a signal ended the child
    printf("child process %d terminated with signal %d\n",
           pid,WTERMSIG(status));
  }

  exit(0);
}
