// util.c: provided functions to ease parsing and pausing.

#include "commando.h"

// Parse the contents of input_command so that tokens[i] will point to
// the ith space-separated string in it. Set ntok to the number of
// tokens that are found.
void parse_into_tokens(char input_command[], char *tokens[], int *ntok){
  int i = 0;
  char *tok = strtok(input_command," \n");
  while(tok!=NULL && i<ARG_MAX){
    tokens[i] = tok;            // assign tokens to found string
    i++;
    tok = strtok(NULL," \n");
  }
  tokens[i] = NULL;             // null terminate tokens to ease argv[] work
  *ntok = i;
  return;
}
  
// Sleep the running program for the given number of nanoseconds and
// seconds.
void pause_for(long nanos, int secs){
  struct timespec tm = {
    .tv_nsec = nanos,
    .tv_sec  = secs,
  };
  nanosleep(&tm,NULL);
}
