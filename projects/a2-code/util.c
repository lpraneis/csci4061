#include "blather.h"

// If condition is a truthy value, call perror() with a message
// constructed with the fmt and additional args provided then exit.
// Example:
//
// checkfail(myfd==-1, "Couldn't open file %s", filename);
void check_fail(int condition, int perr, char *fmt, ...) {
  if(!condition){
    return;
  }
  if(perr){
    char msg[MAXLINE];                  // buffer for message
    va_list myargs;                     // declare a va_list type variable 
    va_start(myargs, fmt);              // initialise the va_list variable with the ... after fmt 
    vsnprintf(msg,MAXLINE,fmt,myargs);  // forward the '...' to vsnprintf()
    va_end(myargs);                     // clean up the va_list
    perror(msg);
  }
  else{
    va_list myargs;                     // declare a va_list type variable 
    va_start(myargs, fmt);              // initialise the va_list variable with the ... after fmt 
    vfprintf(stderr,fmt,myargs);        // forward the '...' to vfprintf()
    va_end(myargs);                     // clean up the va_list
  }
  exit(1);
}

// Print like printf but only if DEBUG is defined as 1. Print to
// standard error.
void dbg_printf(char *fmt, ...){
  if(DEBUG){
    va_list myargs;                      // declare a va_list type variable 
    va_start(myargs, fmt);               // initialise the va_list variable with the ... after fmt 
    vfprintf(stderr,fmt,myargs);         // forward the '...' to vfprintf()
    va_end(myargs);                      // clean up the va_list
  }
}



// Sleep the running program for the given number of nanoseconds and
// seconds.
void pause_for(long nanos, int secs){
  struct timespec tm = {
    .tv_nsec = nanos,
    .tv_sec  = secs,
  };
  nanosleep(&tm,NULL);
}
