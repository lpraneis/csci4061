#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define PREAD 0                 // index of read end of pipe
#define PWRITE 1                // index of write end of pipe
#define BUFSIZE 1024

int main(int argc, char *argv[]) {

  int par_child_pipe[2];
  int pipe_result = pipe(par_child_pipe);
  if(pipe_result != 0) {
    perror("Failed to create pipe");
    exit(1);
  }

  printf("Parent creating child process\n");
  pid_t child_pid = fork();
  if(child_pid <0){
    perror("Failed to fork");
    exit(1);
  }

  // CHILD CODE
  if(child_pid == 0){
    char *msg = "Send $$$ please!";
    int msg_len = strlen(msg)+1;
    int bytes_written = write(par_child_pipe[PWRITE], msg, msg_len);
    printf("Child wrote %d bytes\n",bytes_written);
    fflush(stdout);

    close(par_child_pipe[PWRITE]);
    close(par_child_pipe[PREAD]);
    exit(0);
  }

  // PARENT CODE
  char buffer[BUFSIZE];
  int bytes_read = read(par_child_pipe[PREAD], buffer, BUFSIZE);
  close(par_child_pipe[PWRITE]);
  close(par_child_pipe[PREAD]);

  printf("Parent read %d bytes\n",bytes_read);
  printf("Child said: '%s'\n",buffer);
  wait(NULL);

  return 0;
}
