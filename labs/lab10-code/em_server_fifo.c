// em_server.c: server code which contains a name/email pairs and will
// fulfill requests from a client through FIFOs.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

// data the server has and clients want: pairings of name and email
char *data[][2] = {
  {"Chris Kauffman"       ,"kauffman@umn.edu"},
  {"Christopher Jonathan" ,"jonat003@umn.edu"},
  {"Amy Larson"           ,"larson@cs.umn.edu"},
  {"Chris Dovolis"        ,"dovolis@cs.umn.edu"},
  {"Dan Knights"          ,"knights@cs.umn.edu"},
  {"George Karypis"       ,"karypis@cs.umn.edu"},
  {"Steven Jensen"        ,"sjensen@cs.umn.edu"},
  {"Daniel Keefe"         ,"dfk@umn.edu"},
  {"Michael W. Whalen"    ,"whalen@cs.umn.edu"},
  {"Catherine Qi Zhao"    ,"qzhao@umn.edu"},
  {"Dan Challou"          ,"challou@cs.umn.edu"},
  {"Steven Wu"            ,"zsw@umn.edu"},
  {"Michael Steinbach"    ,"steinbac@cs.umn.edu"},
  {"Jon Weissman"         ,"jon@cs.umn.edu"},
  {"Victoria Interrante"  ,"interran@cs.umn.edu"},
  {"Shana Watters"        ,"watt0087@umn.edu"},
  {"James Parker"         ,"jparker@cs.umn.edu"},
  {"James Moen"           ,"moen0017@cs.umn.edu"},
  {"Daniel Giesel"        ,"giese138@umn.edu"},
  {"Jon Read"             ,"readx028@umn.edu"},
  {"Sara Stokowski"       ,"stoko004@umn.edu"},     
  {NULL                   , NULL},
};

typedef struct {
  char client_fifo[256];        // filename of FIFO on which to respond
  char query_name[256];         // look up this person's email
} request_t;

int main() {
  setvbuf(stdout, NULL, _IONBF, 0); 

  printf("SERVER %5d: starting up\n", getpid());

  remove("requests.fifo");                                       // remove old requests FIFO, ensures starting in a good state
  mkfifo("requests.fifo", S_IRUSR | S_IWUSR);                    // create requests FIFO for client requests

  printf("SERVER %5d: created new requests.fifo, now opening it\n", getpid());
  int requests_fd = open("requests.fifo", O_RDWR);               // open FIFO read/write to avoid blocking
  // int requests_fd = open("requests.fifo", O_RDONLY);             // ALTERNATE OPEN: read only

  printf("SERVER %5d: opened requests.fifo, listening for requests\n", getpid());

  while(1){                                                      // loop forever awaiting client requests
    request_t request;
    int nread = read(requests_fd, &request, sizeof(request_t));              // read a single request from the requests FIFO
    if(nread != sizeof(request_t)){
      printf("SERVER: read %d bytes from requests.fifo; empty pipe, exiting\n",nread);
      break;
    }
    printf("SERVER %5d: received request {client_fifo='%s' query_name='%s' }\n",
           getpid(), request.client_fifo, request.query_name);
    
    char *email = "NOT FOUND";                                   // search for the name/email in the 'database' of records
    for(int i=0; data[i][0] != NULL; i++){
      if( strcmp(request.query_name, data[i][0])==0 ){
        email = data[i][1];                                      // found name, assigne email 
      }
    }

    printf("SERVER %5d: opening client FIFO '%s'\n", getpid(), request.client_fifo);
    int client_fifo_fd = open(request.client_fifo, O_WRONLY);

    printf("SERVER %5d: writing email '%s' for query_name '%s'\n",
           getpid(), email,request.query_name);
    write(client_fifo_fd, email, strlen(email));                 // reply with results to client

    printf("SERVER %5d: closing connection to fifo '%s'\n", getpid(), request.client_fifo);
    close(client_fifo_fd);
  }

  exit(0);
}
