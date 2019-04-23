#include "blather.h"

simpio_t simpio_actual;
simpio_t *simpio = &simpio_actual;

client_t client_actual;
client_t *client = &client_actual;

pthread_t user_thread;          // thread managing user input
pthread_t background_thread;

// Worker thread to manage user input
void *user_worker(void *arg){
  int count = 1;
  while(!simpio->end_of_input){
    simpio_reset(simpio);
    iprintf(simpio, "");                                          // print prompt
    while(!simpio->line_ready && !simpio->end_of_input){          // read until line is complete
      simpio_get_char(simpio);
    }
    if(simpio->line_ready){
      iprintf(simpio, "%2d You entered: %s\n",count,simpio->buf);
      count++;
    }
  }

  pthread_cancel(background_thread); // kill the background thread
  return NULL;
}

// Worker thread to listen to the info from the server.
void *background_worker(void *arg){
  char *text[3] = {
    "Background text #1",
    "Background text #2",
    "Background text #3",
  };
  for(int i=0; ; i++){
    sleep(3);
    iprintf(simpio, "BKGND: %s\n",text[i % 3]);
  }
  return NULL;
}

int main(int argc, char *argv[]){
  char prompt[MAXNAME];
  snprintf(prompt, MAXNAME, "%s>> ","fgnd"); // create a prompt string
  simpio_set_prompt(simpio, prompt);         // set the prompt
  simpio_reset(simpio);                      // initialize io
  simpio_noncanonical_terminal_mode();       // set the terminal into a compatible mode

  pthread_create(&user_thread,   NULL, user_worker,   NULL);     // start user thread to read input
  pthread_create(&background_thread, NULL, background_worker, NULL);
  pthread_join(user_thread, NULL);
  pthread_join(background_thread, NULL);
  
  simpio_reset_terminal_mode();
  printf("\n");                 // newline just to make returning to the terminal prettier
  return 0;
}
  
