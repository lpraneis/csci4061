#include "blather.h"

void *user_thread(void *data);
void *server_thread(void *data);


simpio_t simpio_actual;
simpio_t *simpio = &simpio_actual;

pthread_t user_thd;
pthread_t server_thd;

join_t join_request;

int main(int argc, char** argv){

  char tofifo[MAXNAME+10];
  char fromfifo[MAXNAME+16];
  char server_fifo[MAXNAME+10];

  if (argc < 3){
    printf("ERROR: Not enough arguments\n");
    return -1;
  }


  snprintf(tofifo, MAXNAME+10,  "%d.to.fifo", getpid()); //from server to client
  snprintf(fromfifo, MAXNAME+16,  "%d.from.fifo", getpid()); // to server from client

  snprintf(server_fifo, MAXNAME+10, "%s.fifo", argv[1]);

  // Create the FIFOs
  if(mkfifo(tofifo, DEFAULT_PERMS) < 0){
    perror("Unable to make fifo");
    return -1;
  }
  if(mkfifo(fromfifo, DEFAULT_PERMS) < 0){
    perror("Unable to make fifo");
    return -1;
  }

  // Fill join request
  strcpy(join_request.to_client_fname, tofifo); 
  strcpy(join_request.to_server_fname, fromfifo);
  strncpy(join_request.name, argv[2], MAXNAME);

  int server_fd = open(server_fifo, O_WRONLY, DEFAULT_PERMS);
  int nbytes = write(server_fd, &join_request, sizeof(join_t));
  check_fail(nbytes < 0, 0, "Incorrect size join request\n");



  //set prompt
  char prompt[MAXNAME +2];
  sprintf(prompt, "%s>>", join_request.name);
  simpio_set_prompt(simpio, prompt);
  simpio_reset(simpio);
  simpio_noncanonical_terminal_mode();

  //create threads
  pthread_create(&user_thd, NULL, user_thread, NULL); //create user thread
  pthread_create(&server_thd, NULL, server_thread, NULL); // create server thread


  //wait for threads to return
  pthread_join(user_thd, NULL);
  pthread_join(server_thd,NULL);


  //reset prompt
  simpio_reset_terminal_mode();
  printf("\n");

  return 0;
}

void *user_thread(void *data){

  int server_fd = open(join_request.to_server_fname, O_WRONLY, DEFAULT_PERMS);

  while(!simpio->end_of_input){
    simpio_reset(simpio);
    iprintf(simpio, "");                                          // print prompt
    while(!simpio->line_ready && !simpio->end_of_input){          // read until line is complete
      simpio_get_char(simpio);
    }
    if(simpio->line_ready){
      mesg_t newmsg;

      newmsg.kind = BL_MESG;
      strcpy(newmsg.body, simpio->buf); // copy from line to mesg_t
      strcpy(newmsg.name, join_request.name); // copy name from join_request

      int nbytes = write(server_fd, &newmsg, sizeof(mesg_t));
      check_fail(nbytes < 0, 1, "Error writing to %s from %s\n", 
          join_request.to_server_fname, join_request.name);
    }
  }

  // Write a departed message
  pthread_cancel(server_thd); // kill the server thread
  mesg_t depart;
  strcpy(depart.name, join_request.name); // copy name from join_request
  depart.kind = BL_DEPARTED;
  int nbytes = write(server_fd, &depart, sizeof(mesg_t));
  if (nbytes < 0 ){
    // couldn't be written correctly
    dbg_printf("ERROR: Cannot write depart message\n");
    return (void*) -1;
  }
  return (void*) 0;
}

void *server_thread(void* data){
  // Open the fifos
  int client_fd = open(join_request.to_client_fname, O_RDONLY, DEFAULT_PERMS);
  check_fail(client_fd < 0, 1, "Error opening %s\n", join_request.to_client_fname);
  while(1){
    mesg_t incoming;
    int nbytes = read(client_fd, &incoming, sizeof(mesg_t));
    check_fail(nbytes < 0, 1, "Error: Cannot read from %s\n", join_request.to_client_fname);
    if (nbytes > 0){

      if (incoming.kind == BL_MESG){
        iprintf(simpio, "[%s] : %s\n", incoming.name, incoming.body);
      } 
      else if (incoming.kind == BL_JOINED){
        iprintf(simpio, "-- %s JOINED --\n", incoming.name);
      }
      else if (incoming.kind == BL_DEPARTED){
        iprintf(simpio, "-- %s DEPARTED --\n", incoming.name);

      } 
      else if (incoming.kind == BL_SHUTDOWN){
        break;

      } else{
        // error, kind not supported
        dbg_printf("Unknown type for incoming message\n");
        return (void *) -1;
      }
    }
  }

  // Broken, shudown message recieved
  iprintf(simpio, "!!! server is shutting down !!!\n");
  pthread_cancel(user_thd);

  return (void *) 0;
}

