#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <limits.h>             // added for NAME_MAX

#define DEBUG 1                 // turn of/off debug printing
#define PROMPT ">> "            // prompt for client UI

#define MAXLINE 1024            // max length of line that can be typed for messages
#define MAXNAME 256             // max length of user name for clients
#define MAXPATH 1024            // max length filename paths
#define MAXCLIENTS 256          // max number of clients accepted

#define EOT 4                   // ascii code of typical EOF character
#define DEL 127                 // ascii code of typical backspace key

 // default permissions for fifos
#define DEFAULT_PERMS S_IRUSR | S_IWUSR |S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
#define ALARM_INTERVAL 1                // seconds between alarm rings and pings to clients
#define DISCONNECT_SECS 5              // seconds before clients are dropped due to lack of contact

// client_t: data on a client connected to the server
typedef struct {
  char name[MAXPATH];             // name of the client
  int to_client_fd;               // file descriptor to write to to send to client
  int to_server_fd;               // file descriptor to read from to receive from client
  char to_client_fname[MAXPATH];  // name of file (FIFO) to write into send to client
  char to_server_fname[MAXPATH];  // name of file (FIFO) to read from receive from client
  int data_ready;                 // flag indicating a mesg_t can be read from to_server_fd
  int last_contact_time;          // ADVANCED: server time at which last contact was made with client
} client_t;

// server_t: data pertaining to server operations
typedef struct {
  char server_name[MAXPATH];    // name of server which dictates file names for joining and logging
  int join_fd;                  // file descriptor of join file/FIFO
  int join_ready;               // flag indicating if a join is available
  int n_clients;                // number of clients communicating with server
  client_t client[MAXCLIENTS];  // array of clients populated up to n_clients
  int time_sec;                 // ADVANCED: time in seconds since server started
  int log_fd;                   // ADVANCED: file descriptor for log
  sem_t *log_sem;               // ADVANCED: posix semaphore to control who_t section of log file
} server_t;

// join_t: structure for requests to join the chat room
typedef struct {
  char name[MAXPATH];            // name of the client joining the server
  char to_client_fname[MAXPATH]; // name of file server writes to to send to client
  char to_server_fname[MAXPATH]; // name of file client writes to to send to server
} join_t;

// mesg_kind_t: Kinds of messages between server/client
typedef enum {
  BL_MESG         = 10,         // normal messasge from client with name/body
  BL_JOINED       = 20,         // client joined the server, name only
  BL_DEPARTED     = 30,         // client leaving/left server normally, name only
  BL_SHUTDOWN     = 40,         // server to client : server is shutting down, no name/body
  BL_DISCONNECTED = 50,         // ADVANCED: client disconnected abnormally, name only
  BL_PING         = 60,         // ADVANCED: ping to ask or show liveness
} mesg_kind_t;

// mesg_t: struct for messages between server/client
typedef struct {
  mesg_kind_t kind;               // kind of message
  char name[MAXNAME];             // name of sending client or subject of event
  char body[MAXLINE];             // body text, possibly empty depending on kind
} mesg_t;

// who_t: data to write into server log for current clients (ADVANCED)
typedef struct {
  int n_clients;                   // number of clients on server
  char names[MAXCLIENTS][MAXNAME]; // names of clients
} who_t;

// simpio_t: data structure to manage terminal input/output for clients
typedef struct{
  char buf[MAXLINE];            // line of text to read
  char prompt[MAXNAME];         // prompt to be printed ahead of input
  int pos;                      // position in buf
  int line_ready;               // flag determining if a line in buf is ready for processing
  int end_of_input;             // flag determining if end of input has been indicated
  FILE *infile;                 // FILE to read from for input, usually stdin
  FILE *outfile;                // FILE to write to for output, usually stdout
} simpio_t;


// server.c
client_t *server_get_client(server_t *server, int idx);
void server_start(server_t *server, char *server_name, int perms);
void server_shutdown(server_t *server);
int server_add_client(server_t *server, join_t *join);
int server_remove_client(server_t *server, int idx);
int server_broadcast(server_t *server, mesg_t *mesg);
void server_check_sources(server_t *server);
int server_join_ready(server_t *server);
int server_handle_join(server_t *server);
int server_client_ready(server_t *server, int idx);
int server_handle_client(server_t *server, int idx);
void server_tick(server_t *server);
void server_ping_clients(server_t *server);
void server_remove_disconnected(server_t *server, int disconnect_secs);
void server_write_who(server_t *server);
void server_log_message(server_t *server, mesg_t *mesg);

// simpio.c
void simpio_noncanonical_terminal_mode();
void simpio_reset_terminal_mode();
void simpio_reset(simpio_t *simpio);
void simpio_set_prompt(simpio_t *simpio, char *prompt);
void simpio_get_char(simpio_t *simpio);
void iprintf(simpio_t *simpio, char *fmt, ...);

// util.c
void check_fail(int condition, int perr, char *fmt, ...);
void dbg_printf(char *fmt, ...);
void pause_for(long nanos, int secs);
