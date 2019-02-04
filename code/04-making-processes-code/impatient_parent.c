// impatient_parent.c: demonstrate non-blocking waitpid(), 

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv){

  char *child_argv[] = {"./complain",NULL};
  char *child_cmd = "complain";

  printf("PARENT: Junior is about to '%s', I'll keep an eye on him\n",
         child_cmd);

  pid_t child_pid = fork();

  // CHILD CODE
  if(child_pid == 0){
    printf("CHILD: I'm %d and I'm about to '%s'\n",
           getpid(), child_cmd);
    execvp(child_cmd, child_argv);
  }

  // PARENT CODE
  int status;
  int pid = waitpid(child_pid,&status,WNOHANG); // Check if child done, but don't actually wait
  if(pid == child_pid && WIFEXITED(status)){    // Child did finish
    printf("PARENT: Good job junior. You told me %d\n",WEXITSTATUS(status));
  }
  else{                                          // Child not done yet
    printf("PARENT: %d? The kid's not done yet. I'm bored\n",pid);
  }
  return 0;
}
  
