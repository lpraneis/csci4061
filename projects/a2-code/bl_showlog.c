#include "blather.h"

void print_who(int logfd, who_t *who){
  printf("%d CLIENTS\n", who->n_clients);
  for(int i = 0; i < who->n_clients; i++){
    printf("%d: %s\n", i, who->names[i]);
  }
}

void print_msg(int logfd, mesg_t *mesg){
    if (mesg->kind == BL_MESG){
      printf( "[%s] : %s\n", mesg->name, mesg->body);
    } 
    else if (mesg->kind == BL_JOINED){
      printf( "-- %s JOINED --\n", mesg->name);
    }
    else if (mesg->kind == BL_DEPARTED){
      printf( "-- %s DEPARTED --\n", mesg->name);
    } 
}

int main(int argc, char** argv){

  if (argc < 2){
    printf("Error, logfile must be supplied.\n./bl_sholog [logname]\n");
    return -1;
  }

  int log_fd = open(argv[1], O_RDONLY, DEFAULT_PERMS);
  if (log_fd < 0){
    perror("Error opening file\n");
    return -1;
  }
  // Read who_t
  who_t who;
  if (read(log_fd, &who, sizeof(who_t)) < 0){
    perror("Error reading who_t\n");
    return -1;
  }
  print_who(log_fd, &who);

  printf("MESSAGES\n");


  // Read all remaining messages
  mesg_t nextmsg;
  while(read(log_fd, &nextmsg, sizeof(mesg_t)) > 0){
    print_msg(log_fd, &nextmsg);
  }

  return 0;
}


