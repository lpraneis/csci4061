// Shows dangers of using a non-reentrant function with signal
// handling. Strange things can happen due to the use of the
// non-reentrant function getpwnam() being called both in the main
// loop and signal handler. Uncommenting the getpwnam() in the signal
// handler causes unpredictable things to happen like a permanent
// stall of the program on Linux boxes.
// 
// Adapted from Stevens and Rago Fig 10.5. 
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>

#define USERNAME "kauffman"

static void alarm_handler(int signo) {
  printf("in signal handler\n");
  struct passwd *rootptr = NULL;

  // NON-REENTRANT FUNCTION CALL IN SIGNAL HANDLER
  rootptr = getpwnam("root");                    // fetch record associated with root password

  if (rootptr == NULL){                          // check for presence of root
    printf("no root, that's weird...\n");
  }
  else{
    printf("root looks okay\n");
  }
  alarm(1);                                      // reset the alarm
  printf("leaving signal handler\n");
}

int main(void) {
  signal(SIGALRM, alarm_handler);                // handle alarms in with the above function
  alarm(1);
  int successes = 0;
  printf("Repeatedly checking on user '%s'\n",USERNAME);
  while(1){
    sigset_t block_all, defaults;
    sigfillset( &block_all );
    sigprocmask(SIG_SETMASK, &block_all, &defaults);
    struct passwd *ptr = getpwnam(USERNAME);     // non-reentrant call
    
    sigprocmask(SIG_SETMASK, &defaults, NULL);

    if(ptr == NULL){
      perror("couldn't find user");
      exit(1);
    }
    if(strcmp(ptr->pw_name, USERNAME) != 0){     // should always be equal to USERNAME
      printf("return value corrupted!, pw_name = %s\n",
             ptr->pw_name);
      exit(1);
    }
    // sigprocmask(SIG_SETMASK, &defaults, NULL);

    successes++;                                 // Show progress of loop
    if(successes % 100000 == 0){
      printf("%d successes so far\n",successes);
    }
  }
  exit(0);
}
