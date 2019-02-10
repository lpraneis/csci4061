#include "commando.h"
// cmdcol.c: functions related to cmdcol_t collections of commands.

void cmdcol_add(cmdcol_t *col, cmd_t *cmd) {
  if (col->size + 1 > MAX_CMDS) { // error! too many commands
    printf("Error! Too many commands\n");
    exit(1);
  }
  col->cmd[col->size++] = cmd;
}
// Add the given cmd to the col structure. Update the cmd[] array and
// size field. Report an error if adding would cause size to exceed
// MAX_CMDS, the maximum number commands supported.

void cmdcol_print(cmdcol_t *col) {
  printf("%-4s #%-8s %4s %10s %4s %-s\n", "JOB", "PID", "STAT", "STR_STAT",
         "OUTB", "COMMAND");
  for (int i = 0; i < col->size; i++) {
    cmd_t *curr = col->cmd[i];
    /* extern int num_tokens; // use num_tokens from commando.c */
    printf("%-4d #%-8d %4d %10s %4d ", i, curr->pid, curr->status,
           curr->str_status, curr->output_size);
    for (int j = 0; j < ARG_MAX ; j++) {
      if (curr->argv[j] == NULL)
        break;
      printf("%s ", curr->argv[j]);
    }
    printf("\n");
  }
}

// Print all cmd elements in the given col structure.  The format of
// the table is
//
// JOB  #PID      STAT   STR_STAT OUTB COMMAND
// 0    #17434       0    EXIT(0) 2239 ls -l -a -F
// 1    #17435       0    EXIT(0) 3936 gcc --help
// 2    #17436      -1        RUN   -1 sleep 2
// 3    #17437       0    EXIT(0)  921 cat Makefile
//
// Widths of the fields and justification are as follows
//
// JOB  #PID      STAT   STR_STAT OUTB COMMAND
// 1234  12345678 1234 1234567890 1234 Remaining
// left  left    right      right rigt left
// int   int       int     string  int string
//
// The final field should be the contents of cmd->argv[] with a space
// between each element of the array.

void cmdcol_update_state(cmdcol_t *col, int block){
  for(int i = 0; i < col->size; i++){
    cmd_update_state(col->cmd[i], block);
  }
}
// Update each cmd in col by calling cmd_update_state() which is also
// passed the block argument (either NOBLOCK or DOBLOCK)

void cmdcol_freeall(cmdcol_t *col) {
  for (int i = 0; i < col->size; i++) {
    cmd_free(col->cmd[i]);
  }
};
// Call cmd_free() on all of the constituent cmd_t's.
