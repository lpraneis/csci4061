// Redirect standard output temporarily to a pipe, then restore
// standard output and retrieve the captured output
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PREAD 0                 // index of read end of pipe
#define PWRITE 1                // index of write end of pipe

int main(){
  setvbuf(stdout, NULL, _IONBF, 0); // Turn off buffering

  printf("Piping\n");
  int my_pipe[2];
  pipe(my_pipe);
  int stdout_bak = dup(STDOUT_FILENO);
  // int stderr_bak = dup(STDERR_FILENO);
  dup2(my_pipe[PWRITE],STDOUT_FILENO);
  printf("In the my_pipe, five by five");
  dup2(stdout_bak, STDOUT_FILENO);
  close(my_pipe[PWRITE]);
  printf("Read from the my_pipe\n");
  char buf[2048];
  int bytes_read = read(my_pipe[PREAD], buf, 2048);
  buf[bytes_read] = '\0';
  close(my_pipe[PREAD]);
  printf("Read: '%s'\n",buf);
  
  return 0;
}
