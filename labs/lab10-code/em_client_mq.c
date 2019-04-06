// em_client_mq.c: client code to look up an email based on name using
// POSIX IPC message queues.
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define NAME_LENGTH 256

typedef struct {                                               // structure to store a request
  char client_queue_name[NAME_LENGTH];                         // name for the client message queue
  char query_name[NAME_LENGTH];                                // look up this person's email
} request_t;

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s <name>\n",argv[0]);
    exit(1);
  }

  request_t request;
  sprintf(request.client_queue_name, "/%d_q", getpid());       // queue named after pid (mildly unsafe naming)
  strcpy(request.query_name, argv[1]);                         // copy command line arg in as query name

  struct mq_attr attr = {
    .mq_maxmsg = 1,                                            // only expect 1 message back from server
    .mq_msgsize = NAME_LENGTH,                                 // max size of email addresses
  };
  mqd_t client_qd = mq_open(request.client_queue_name,         // descriptor for client queue
                            O_CREAT|O_RDONLY, S_IRUSR|S_IWUSR, // flags and permissions
                            &attr);                            // set up usin provided attr or NULL for system defaults

  printf("CLIENT %5d: sending request: {client_queue_name='%s' query_name='%s' }\n",
         getpid(), request.client_queue_name, request.query_name);

  mqd_t server_qd = mq_open("/em_server_q", O_WRONLY);         // open existin server queue

  mq_send(server_qd, (char *) &request, sizeof(request_t), 0); // send request to server

  char response[NAME_LENGTH];                                  // space for response from server
  printf("CLIENT %5d: awaiting server response\n",getpid());

  mq_receive(client_qd, response, NAME_LENGTH, NULL);          // read response from server, no casting required
  printf("CLIENT %5d: response for name '%s' is email '%s'\n", // report response
         getpid(), request.query_name, response);

  mq_close(server_qd);                                         // close server queue
  mq_close(client_qd);                                         // close client queue
  mq_unlink(request.client_queue_name);                        // unlink only client queue

  exit(0);
}
