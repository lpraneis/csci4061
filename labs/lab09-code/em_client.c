// em_client.c: client code to look up an email based on name. 

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

// structure to store a request_t. 
typedef struct {
  char client_fifo[256];                                       // filename of FIFO on which to respond
  char query_name[256];                                        // look up this person's email
} request_t;

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s <name>\n",argv[0]);
    exit(1);
  }

  request_t request;
  sprintf(request.client_fifo, "%d.fifo", getpid());           // fifo named after pid (mildly unsafe naming)
  strcpy(request.query_name, argv[1]);                         // copy command line arg in as query name

  mkfifo(request.client_fifo, S_IRUSR | S_IWUSR);              // create the client FIFO for a response

  printf("CLIENT %5d: sending request: {fifo_file='%s' query_name='%s' }\n",
         getpid(), request.client_fifo, request.query_name);
  int requests_fd = open("requests.fifo", O_RDWR);             // open FIFO read/write in case server hasn't started  

  write(requests_fd, &request, sizeof(request_t));             // wend request to server

  printf("CLIENT %5d: opening '%s'\n",
         getpid(), request.client_fifo);

  char response[256];
  int client_fifo_fd = open(request.client_fifo, O_RDWR);      // open client FIFO to receive response
  printf("CLIENT %5d: fifo opened, awaiting server response\n",getpid());

  int nread = read(client_fifo_fd, response, 255);             // read response from server
  response[nread] = '\0';                                      // null terminate the string
  printf("CLIENT %5d: response for name '%s' is email '%s'\n", // report response
         getpid(), request.query_name, response);

  close(client_fifo_fd);
  close(requests_fd);
  remove(request.client_fifo);

  exit(0);
}
