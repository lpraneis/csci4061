// faulty_em_server_mq.c: Server code which contains a name/email pairs and
// will fulfill requests from a client through POSIX IPC message
// queues.
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>

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

#define NAME_LENGTH 256

typedef struct {                                                 // structure to store a request
  char client_queue_name[NAME_LENGTH];                           // name client queue on which to respond
  char query_name[NAME_LENGTH];                                  // look up this person's email
} request_t;

int signalled = 0;                                               // indicates if the server should shut down at next opportunity
void handle_signals(int sig_num){
  signalled = 1;
}

int main() {
  setvbuf(stdout, NULL, _IONBF, 0); 
  struct sigaction my_sa = {
    .sa_handler = handle_signals,                                // run function handle_signals
  };
  sigaction(SIGTERM, &my_sa, NULL);                              // handle SIGTERM and
  sigaction(SIGINT,  &my_sa, NULL);                              // SIGINT by shutting down

  printf("SERVER %5d: starting up\n", getpid());

  struct mq_attr attr = {                                        // attributes of the message queue
    .mq_maxmsg = 10,                                             // queue holds 10 messages before sending blocks (standard system max)
    .mq_msgsize = NAME_LENGTH,                                   // maximum size of messages
  };
  mqd_t server_qd = mq_open("/em_server_q",                      // descriptor for message queue
                            O_CREAT|O_RDONLY, S_IRUSR|S_IWUSR,   // flags and permissions
                            &attr);                              // set up usin provided attr or NULL for system defaults

  mqd_t reply_qd = mq_open("/em_reply_q",                        // descriptor for reply queue to clients
                            O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR,   // flags and permissions
                            &attr);                              // set up usin provided attr or NULL for system defaults
  
  printf("SERVER %5d: created message queues, listening for requests\n", getpid());

  while(!signalled){                                             // loop while no signal is received
    char query_name[NAME_LENGTH];                                // look up this person's email

    int ret;
    ret = mq_receive(server_qd, query_name,                      // receive a request name for lookup
                     NAME_LENGTH, NULL);

    if(ret==-1 && errno==EINTR && signalled==1){                 // signal interrupted receiving
      break;
    }

    printf("SERVER %5d: received request query_name='%s'\n",
           getpid(), query_name);

    char *email = "NOT FOUND";                                   // search for the name/email in the 'database' of records
    for(int i=0; data[i][0] != NULL; i++){
      if( strcmp(query_name, data[i][0])==0 ){
        email = data[i][1];                                      // found name, assigne email 
      }
    }

    printf("SERVER %5d: writing email '%s' on em_reply_q for name '%s'\n",
           getpid(), email, query_name);
    mq_send(reply_qd, email, NAME_LENGTH, 0);                    // reply with results to client
  }

  printf("SERVER %5d: signalled, closing and unlinking queue\n", getpid());
  mq_close(server_qd);                                           // close the connection to the queues
  mq_close(reply_qd);
  mq_unlink("/em_server_q");                                     // remove the queue 
  mq_unlink("/em_reply_q");
  
  return 0;
}
