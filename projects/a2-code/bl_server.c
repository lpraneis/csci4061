#include "blather.h"


int signalled =0;

void handle_signal(int signum){
  signalled = 1;
}


int main(int argc, char** argv){
  if (argc < 2){
    perror("No server name given");
    return 1;
  }
  server_t server;

  // Set up signal handling
  struct sigaction sa = {};
  sa.sa_handler = handle_signal;
  sigaction(SIGTERM, &sa, NULL);

  sa.sa_handler = handle_signal;
  sigaction(SIGINT, &sa, NULL);


  server_start(&server, argv[1], DEFAULT_PERMS);
  while(!signalled) {
    /* check all sources */
    /*   handle a join request if on is ready */
    /*   for each client{ */
    /*     if the client is ready handle data from it */
    
  }

  server_shutdown(&server); 
  return 0;
}

