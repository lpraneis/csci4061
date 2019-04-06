// faulty_em_client_mq.c: client code to look up an email based on name using
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
} request_t;

int main(int argc, char *argv[]){
  setvbuf(stdout, NULL, _IONBF, 0); 

  if(argc < 2){
    printf("usage: %s <name> [expect-email]\n",argv[0]);
    exit(1);
  }
  char *query_name = argv[1];                                  // name of query from command line
  
  printf("CLIENT %5d: sending request: query_name='%s'\n",
         getpid(), query_name);

  mqd_t server_qd = mq_open("/em_server_q", O_WRONLY);         // open existing server queue

  mq_send(server_qd, query_name, strlen(query_name)+1, 0);     // send request to server

  char response[NAME_LENGTH];                                  // space for response from server
  printf("CLIENT %5d: awaiting server response\n",getpid());

  mqd_t reply_qd = mq_open("/em_reply_q", O_RDONLY);           // open existing reply queue

  mq_receive(reply_qd, response, NAME_LENGTH, NULL);           // read response from server, no casting required

  printf("CLIENT %5d: response for name '%s' is email '%s'\n", // report response
         getpid(), query_name, response);

  // ERROR CHECKING used in run_simulation_faulty.sh
  if(argc >= 3){                                               // if 2 command line args provided
    if(strcmp(response, argv[2]) != 0){                        // argv[2] is expected email address
      char buf[2048];                                          // build up an error string
      int pos = 0;
      pos += sprintf(buf+pos,"---ERROR for CLIENT %5d---------------\n",getpid());
      pos += sprintf(buf+pos,"|  Query Name:   %s\n",query_name);
      pos += sprintf(buf+pos,"|  Expect email: %s\n",argv[2]);
      pos += sprintf(buf+pos,"|  Actual email: %s\n",response);
      pos += sprintf(buf+pos,"----------------------------------------\n");
      write(STDOUT_FILENO,buf,strlen(buf));                    // write once, avoids interleaved output
    }
  }

  mq_close(server_qd);                                         // close queues
  mq_close(reply_qd);                                          

  exit(0);
}
