#include "commando.h"

void print_help();

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0); // Turn off output buffering via instructions
  char user_raw_input[MAX_LINE];
  char **user_input = malloc(ARG_MAX * NAME_MAX);

  int num_tokens;
  static int echoing;


  if ( argc > 1 && !strcmp(argv[1], "--echo")){ //set the echoing variable correctly
    echoing = 1;
  } else if(getenv("COMMAND_ECHO") != NULL) { //if getenv is not null/0, will also set echoing
    echoing =1;
  }

  while(1){
    printf("@> ");
    fgets(user_raw_input, MAX_LINE, stdin);
    parse_into_tokens(user_raw_input, user_input, &num_tokens);
    if (user_input[0] == NULL  ) {
      // the user just hit enter
      continue;
    } 

    if(echoing){ //prints the command sent to commando if not stdin 
      for(int i = 0; i < num_tokens; i++) {
        printf("%s ", user_input[i]);
      } 
      printf("\n");
    }


    if (strcmp("help", user_input[0]) == 0) {
      //user pressed help
      print_help();
    } else if (strcmp("exit", user_input[0]) == 0){
      break;
    } else if (strcmp("list", user_input[0]) == 0){
      break;
    } else if (strcmp("pause", user_input[0]) == 0){
      pause_for(atoi(user_input[1]), atoi(user_input[2]));
    } else if (strcmp("output-for", user_input[0]) == 0){
      break;
    } else if (strcmp("output-all", user_input[0]) == 0){
      break;
    } else if (strcmp("wait-for", user_input[0]) == 0){
      break;
    } else if (strcmp("wait-all", user_input[0]) == 0){
      break;
    } else if (strcmp("command", user_input[0]) == 0){
      break;
    } else {
      //user command
      break;
    }
  }


  //free all dynamic memory
  free(user_input);
  return 0;
};

void print_help(){
  printf("%-18s\n", "COMMANDO COMMANDS");
  printf("%-18s : %s\n", "help","show this message");
  printf("%-18s : %s\n", "exit","exit the program");
  printf("%-18s : %s\n", "list","list all jobs that have been started giving information on each");
  printf("%-18s : %s\n", "pause nanos secs","pause for the given number of nanseconds and seconds");
  printf("%-18s : %s\n", "output-for int","print the output for given job number");
  printf("%-18s : %s\n", "output-all","print output for all jobs");
  printf("%-18s : %s\n", "wait-for int","wait until the given job number finishes");
  printf("%-18s : %s\n", "wait-all","wait for all jobs to finish");
  printf("%-18s : %s\n", "command arg1 ...","non-built-in is run as a job");
}
