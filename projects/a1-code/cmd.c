#include "commando.h"
#define BUF_READ_SIZE  128 //this is for the amount that is read at a time by the read_all function
// cmd.c: functions related the cmd_t struct abstracting a
// command. Most functions maninpulate cmd_t structs.

cmd_t *cmd_new(char *argv[]) {
  cmd_t *new;
  int i;
  char *init = "INIT";
  new = (cmd_t *)malloc(sizeof(cmd_t));

  for (i = 0; i < ARG_MAX + 1; i++) {
    if (argv[i] != NULL) {
      new->argv[i] = strdup(argv[i]);
    } else {
      new->argv[i] = NULL; // sets the last element to NULL
      break;
    }
  }

  strcpy(new->name, new->argv[0]);          // copies the argv[0] to name
  snprintf(new->str_status, 5, "%s", init); // set str_status to INIT
  new->pid = -1;
  new->out_pipe[0] = -1;
  new->out_pipe[1] = -1;
  new->finished = 0;
  new->status = -1;
  new->output = NULL;
  new->output_size = -1;
  return new;
}


// Allocates a new cmd_t with the given argv[] array. Makes string
// copies of each of the strings contained within argv[] using
// strdup() as they likely come from a source that will be
// altered. Ensures that cmd->argv[] is ended with NULL. Sets the name
// field to be the argv[0]. Sets finished to 0 (not finished yet). Set
// str_status to be "INIT" using snprintf(). Initializes the remaining
// fields to obvious default values such as -1s, and NULLs.

void cmd_free(cmd_t *cmd) {
  for (int i = 0; i < ARG_MAX + 1; i++) { // free each string in argv
    if (cmd->argv[i] != NULL) {
      free(cmd->argv[i]);
    } else {
      break;
    }
  }

  if (cmd->output != NULL)
    free(cmd->output);

  free(cmd);
}
// Deallocates a cmd structure. Deallocates the strings in the argv[]
// array. Also deallocats the output buffer if it is not
// NULL. Finally, deallocates cmd itself.

void cmd_start(cmd_t *cmd) {
  // first, create a pipe
  pipe(cmd->out_pipe);
  snprintf(cmd->str_status, 4, "%s", "RUN"); // copy RUN to str_status
  cmd->pid = fork();
  if (cmd->pid == 0) { // child process
    dup2(STDOUT_FILENO,
         cmd->out_pipe[PWRITE]); // alter output from stdout to pipe
    close(cmd->out_pipe[PREAD]); // close the reading end
    execvp(cmd->name, cmd->argv);

  } else {                        // parent process
    close(cmd->out_pipe[PWRITE]); // close the writing end
    // ensures that the pid field is set to the child PID?
  }
}

// Forks a process and starts executes command in cmd in the process.
// Changes the str_status field to "RUN" using snprintf().  Creates a
// pipe for out_pipe to capture standard output.  In the parent
// process, ensures that the pid field is set to the child PID. In the
// child process, directs standard output to the pipe using the dup2()
// command. For both parent and child, ensures that unused file
// descriptors for the pipe are closed (write in the parent, read in
// the child).

void cmd_update_state(cmd_t *cmd, int block) {
  if (cmd->finished) // if the finished is 1, does nothing
    return;

  if (block) { // normal waitpid call
    waitpid(cmd->pid, &cmd->status, NOBLOCK);
  } else { // non-blocking
    waitpid(cmd->pid, &cmd->status, DOBLOCK);
    if (WIFEXITED(cmd->status)) {
      cmd->finished = 1;
      cmd->status = WEXITSTATUS(cmd->status);
      printf("@!!! %s[#%d]: EXIT[%d]\n", cmd->name, cmd->pid,
             cmd->status); // set str_status to INIT
    }
  }
  cmd_fetch_output(cmd);
}
// If the finished flag is 1, does nothing. Otherwise, updates the
// state of cmd.  Uses waitpid() and the pid field of command to wait
// selectively for the given process. Passes block (one of DOBLOCK or
// NOBLOCK) to waitpid() to cause either non-blocking or blocking
// waits.  Uses the macro WIFEXITED to check the returned status for
// whether the command has exited. If so, sets the finished field to 1
// and sets the cmd->status field to the exit status of the cmd using
// the WEXITSTATUS macro. Calls cmd_fetch_output() to fill up the
// output buffer for later printing.
//
// When a command finishes (the first time), prints a status update
// message of the form
//
// @!!! ls[#17331]: EXIT(0)
//
// which includes the command name, PID, and exit status.

char *read_all(int fd, int *nread){
    int buf_size = BUFSIZE; //beginning buffer size
    char *buffer = (char*)malloc(buf_size);
    int curr_bytes = 0; 
    int read_bytes;
    while(1){
      if (curr_bytes + BUF_READ_SIZE > buf_size -1){ //resize array
        buf_size *=2;
        buffer = (char*)realloc(buffer, buf_size);
      } 
      read_bytes = read(fd, buffer, BUF_READ_SIZE );
      curr_bytes+=read_bytes;
      if(read_bytes < BUF_READ_SIZE) //if at end, break
        break;
    }
    buffer[curr_bytes] = '\0'; //set to null-terminated
    *nread = curr_bytes;
    return buffer;
}
// Reads all input from the open file descriptor fd. Stores the
// results in a dynamically allocated buffer which may need to grow as
// more data is read.  Uses an efficient growth scheme such as
// doubling the size of the buffer when additional space is
// needed. Uses realloc() for resizing.  When no data is left in fd,
// sets the integer pointed to by nread to the number of bytes read
// and return a pointer to the allocated buffer. Ensures the return
// string is null-terminated. Does not call close() on the fd as this
// is done elsewhere.

void cmd_fetch_output(cmd_t *cmd) {
  if (!cmd->finished) { // if not finished
    printf("%s[#%d] not finished yet\n", cmd->name, cmd->pid);
  }
  cmd->output = read_all(cmd->out_pipe[PREAD], &cmd->output_size);
  close(cmd->out_pipe[PREAD]);
}

// If cmd->finished is zero, prints an error message with the format
//
// ls[#12341] not finished yet
//
// Otherwise retrieves output from the cmd->out_pipe and fills
// cmd->output setting cmd->output_size to number of bytes in
// output. Makes use of read_all() to efficiently capture
// output. Closes the pipe associated with the command after reading
// all input.

void cmd_print_output(cmd_t *cmd) {
  if (cmd->output == NULL) {
    printf("%s[#%d] : output not ready\n", cmd->name, cmd->pid);
  }
  write(STDOUT_FILENO, cmd->output, cmd->output_size);
}

// Prints the output of the cmd contained in the output field if it is
// non-null. Prints the error message
//
// ls[#17251] : output not ready
//
// if output is NULL. The message includes the command name and PID.
