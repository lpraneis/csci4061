#include "blather.h"


int signalled =0; // when to stop the while loop

void handle_signal(int signum){
  signalled = 1;
}

int main(int argc, char** argv){
  if (argc < 2){
    perror("No server name given");
    return 1;
  }
  server_t server = {};

  struct sigaction sa = {
  .sa_handler = handle_signal,
  .sa_flags = SA_RESTART,
  }; // Set up signal handler

  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);

  server_start(&server, argv[1], DEFAULT_PERMS);
  dbg_printf("Server Started\n");

  while(!signalled) {
    server_check_sources(&server); // check all sources and set flags

    if (server.join_ready == 1){
      // check if there is a new client waiting to join
      server_handle_join(&server);
    }

    for (int i = 0; i < server.n_clients; i++){
      if(server_get_client(&server, i)->data_ready){
        if(server_handle_client(&server, i) < 0){
          dbg_printf("Error in handling client %d\n", i);
          perror("Error handling client");
          return -1;
        }
      }
    }
  }

  dbg_printf("Server Recieved Signal, Shutting Down\n");
  server_shutdown(&server); 
  return 0;
}

