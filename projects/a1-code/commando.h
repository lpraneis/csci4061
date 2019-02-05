#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Compile time constants.
#define BUFSIZE 1024            // size of read/write buffers
#define PREAD 0                 // index of read end of pipe
#define PWRITE 1                // index of write end of pipe
#define NAME_MAX 255            // max len of commands and args
#define ARG_MAX 255             // max number of arguments
#define MAX_LINE 1024           // maximum length of input lines
#define MAX_CMDS 1024           // maximum number of child commands in commando
#define STATUS_LEN 10           // length of the str_status field in childcmd

// block options to update_cmd_status() indicating whether to block or
// not on waiting for child; passed to wait()
#define NOBLOCK (WCONTINUED | WUNTRACED | WNOHANG)
#define DOBLOCK (WCONTINUED | WUNTRACED)

#define eprintf(...) fprintf (stderr, __VA_ARGS__)

// cmd_t: struct to represent a running command/child process.
typedef struct {
  char   name[NAME_MAX+1];         // name of command like "ls" or "gcc"
  char  *argv[ARG_MAX+1];          // argv for running child, NULL terminated
  pid_t  pid;                      // PID of child
  int    out_pipe[2];              // pipe for child output
  int    finished;                 // 1 if child process finished, 0 otherwise
  int    status;                   // return value of child, -1 if not finished
  char   str_status[STATUS_LEN+1]; // describes child status such as RUN or EXIT(..)
  void  *output;                   // saved output from child, NULL initially
  int    output_size;              // number of bytes in output
} cmd_t;

// cmdcol_t: struct for tracking multiple commands
typedef struct {                
  cmd_t *cmd[MAX_CMDS];         // array of pointers to cmd_t
  int size;                     // number of cmds in the array
} cmdcol_t;

// util.c
void parse_into_tokens(char input_command[], char *tokens[], int *ntok);
void pause_for(long nanos, int secs);

// cmd.c
cmd_t *cmd_new(char *argv[]);
void cmd_free(cmd_t *cmd);
void cmd_set_stdin(cmd_t *cmd, char *input_file);
void cmd_start(cmd_t *cmd);
void cmd_fetch_output(cmd_t *cmd);
void cmd_print_output(cmd_t *cmd);
void cmd_update_state(cmd_t *cmd, int nohang);
char *read_all(int fd, int *nread); 

// cmdcol.c
void cmdcol_print(cmdcol_t *col);
void cmdcol_add(cmdcol_t *col, cmd_t *cmd);
void cmdcol_update_state(cmdcol_t *col, int nohang);
void cmdcol_freeall(cmdcol_t *col);
