// child_wait_returnval.c: fork/exec plus parent waits for child and
// checks their status using macors. If nonzero, parent reports.

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv){
  char *child_argv[] = {"./complain",NULL};             // program returns non-zero
  char *child_cmd = "complain";

  printf("I'm %d, and I really don't feel like '%s'ing\n",
         getpid(),child_cmd);
  printf("I have a solution\n");

  pid_t child_pid = fork();

  if(child_pid == 0){
    printf("   I'm %d My pa '%d' wants me to '%s'. This sucks.\n",
           getpid(), getppid(), child_cmd);
    execvp(child_cmd, child_argv);
    printf("   I don't feel like myself anymore...\n"); // unreachable
  }
  else{
    int status;
    wait(&status);
  }
