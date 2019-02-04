#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
  int i;
  for(i = 0; i<8; i++){
    pid_t child = fork();
    if(child == 0){
      break;
    }
  waitpid(child, 0,0);
  int ppid = getppid();
  printf("Parent = %d\n", ppid);
  }
  printf("I am number %d, my pid is %d\n",i,getpid());
  return 0;
}


