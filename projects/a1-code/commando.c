#include "commando.h"

void print_help();

int main(int argc, char **argv) {

  setvbuf(stdout, NULL, _IONBF, 0);

  // allocate basic structures, variables
  char user_raw_input[MAX_LINE];
  char **user_input = malloc(ARG_MAX * NAME_MAX);
  int num_tokens;
  static int echoing;
  cmdcol_t *cmdcol = (cmdcol_t *)malloc(MAX_CMDS * sizeof(cmd_t));
  cmdcol->size = 0; // to prevent segfaults on output of no commands
  char *line = "----------------------------------------";

  if (argc > 1 && !strcmp(argv[1], "--echo")) {
    // set the echoing variable correctly
    echoing = 1;
  } else if (getenv("COMMAND_ECHO") != NULL) {
    // if getenv is not null/0, will also set echoing
    echoing = 1;
  }

  while (1) {
    printf("@> ");
    if (!fgets(user_raw_input, MAX_LINE, stdin)) {
      // EOF
      printf("\nEnd of input\n");
      break;
    }
    parse_into_tokens(user_raw_input, user_input, &num_tokens);

    if (echoing) { // prints the command sent to commando if not stdin
      for (int i = 0; i < num_tokens; i++) {
        printf("%s ", user_input[i]);
      }
      printf("\n");
    }

    if (user_input[0] == NULL) {
      // the user just hit enter
    } else if (strcmp("help", user_input[0]) == 0) { // user pressed help
      print_help();
    } else if (strcmp("exit", user_input[0]) == 0) {
      // breaking out of loop, freeing memory
      break;
    } else if (strcmp("list", user_input[0]) == 0) {
      // listing all by cmdcol_print
      cmdcol_print(cmdcol);
    } else if (strcmp("pause", user_input[0]) == 0) {
      // pauing and then update_state
      pause_for(atoi(user_input[1]), atoi(user_input[2]));
      cmdcol_update_state(cmdcol, NOBLOCK);

    } else if (strcmp("output-for", user_input[0]) == 0) {

      int jobid = atoi(user_input[1]);
      if (jobid >= cmdcol->size) {
        printf("ERROR: NO JOB %d\n", jobid);
      } else {
        cmd_t *curr_cmd = cmdcol->cmd[jobid]; // terse pointer to curr_cmd

        printf("@<<< Output for %s[#%d] (%d bytes):\n", curr_cmd->name,
               cmdcol->cmd[jobid]->pid, curr_cmd->output_size);
        printf("%s\n", line);
        cmd_print_output(curr_cmd);
        printf("%s\n", line);
      }

    } else if (strcmp("output-all", user_input[0]) == 0) {
      for (int i = 0; i < cmdcol->size; i++) {
        printf("@<<< Output for %s[#%d] (%d bytes):\n", cmdcol->cmd[i]->name,
               cmdcol->cmd[i]->pid, cmdcol->cmd[i]->output_size);
        printf("%s\n", line);
        cmd_print_output(cmdcol->cmd[i]);
        printf("%s\n", line);
      }
    } else if (strcmp("wait-for", user_input[0]) == 0) {

      int jobid = atoi(user_input[1]);
      if (jobid >= cmdcol->size) {
        printf("ERROR: NO JOB %d\n", jobid);
      } else {
        cmd_update_state(cmdcol->cmd[jobid], DOBLOCK);
      }

    } else if (strcmp("wait-all", user_input[0]) == 0) {
      for (int i = 0; i < cmdcol->size; i++) {
        cmd_update_state(cmdcol->cmd[i], DOBLOCK);
      }

    } else {
      // user command
      cmd_t *new = cmd_new(user_input);
      cmd_start(new);
      cmdcol_add(cmdcol, new);
    }
    // update state of all
    cmdcol_update_state(cmdcol, NOBLOCK);
  }

  // free all dynamic memory
  free(user_input);
  cmdcol_freeall(cmdcol);
  free(cmdcol);
  return 0;
};

void print_help() { // helper function for printing help
  printf("%-18s\n", "COMMANDO COMMANDS");
  printf("%-18s : %s\n", "help", "show this message");
  printf("%-18s : %s\n", "exit", "exit the program");
  printf("%-18s : %s\n", "list",
         "list all jobs that have been started giving information on each");
  printf("%-18s : %s\n", "pause nanos secs",
         "pause for the given number of nanseconds and seconds");
  printf("%-18s : %s\n", "output-for int",
         "print the output for given job number");
  printf("%-18s : %s\n", "output-all", "print output for all jobs");
  printf("%-18s : %s\n", "wait-for int",
         "wait until the given job number finishes");
  printf("%-18s : %s\n", "wait-all", "wait for all jobs to finish");
  printf("%-18s : %s\n", "command arg1 ...", "non-built-in is run as a job");
}
