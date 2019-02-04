#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void){

  char *child_argv[] = {"ls","-ah", "..",NULL};
  char *child_cmd = "ls";

  printf("Running command '%s'\n",child_cmd);
  printf("------------------\n");

  if(fork() == 0){
    execvp(child_cmd,child_argv);
  } else {
    wait(0);
    printf("------------------\n");
    printf("Finished\n");
  } 
  return 0;
}
  
