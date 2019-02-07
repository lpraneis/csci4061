#include "commando.h"

//Simple true/false type declaration
typedef enum {false, true} bool;

int main(){
  bool shell_active = true; // controls inner while loop

  setvbuf(stdout, NULL, _IONBF, 0); // Turn off output buffering via instructions
  char user_raw_input[MAX_LINE];
  char *user_input; //user input array
  int num_tokens;

  while(shell_active){
    printf("@> ");
    fgets(user_raw_input, MAX_LINE, stdin);
    parse_into_tokens(user_raw_input, &user_input, &num_tokens);
  }
  return 0;
};
