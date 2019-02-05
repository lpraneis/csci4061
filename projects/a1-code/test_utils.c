#include "commando.h"
#include "test_utils.h"

// Fail with given message
void fail(char *msg, int strerr){
   printf("FAIL\n");
   if(strerr){
     char *errstr = strerror(errno);
     printf("%s : %s\n\n",msg,errstr);
   }
   else{
     printf("%s\n\n",msg);
   }
   return;
}

// Utility to check if two ints are equal; if unequal print errors and
// return 1
int assert_int_equals(char *msg, int expect, int actual){
  if(expect != actual){
    printf("FAIL\n");
    printf("%s : expect= %d actual= %d\n",msg,expect,actual);
    printf("\n");
    return 1;
  }
  return 0;
}

// Utility to check if two ints are equal; if unequal print errors and
// return 1
int assert_int_positive(char *msg, int actual){
  if(actual <= 0){
    printf("FAIL\n");
    printf("%s : expect greater than 0, actual= %d\n",msg,actual);
    printf("\n");
    return 1;
  }
  return 0;
}
  
// Utility to check if two strings are equal; if unequal print errors
// and return 1
int assert_str_equals(char *msg, char *expect, char *actual){
  if(strcmp(expect,actual) != 0){
    printf("FAIL\n");
    printf("%s : expect= %s actual= %s\n",msg,expect,actual);
    printf("\n");
    return 1;
  }
  return 0;
}
// Utility to check if two strings are equal; if unequal print errors
// and return 1; prints expect and actual on their own lines for
// multiline strings
int assert_strn_equals(char *msg, char *expect, char *actual){
  if(strcmp(expect,actual) != 0){
    printf("FAIL\n");
    printf("%s :\nEXPECT:\n%s===\nACTUAL:\n%s===\n",msg,expect,actual);
    printf("\n");
    return 1;
  }
  return 0;
}
  
// Utility to check if pointer is null; if not print errors
// and return 1
int assert_null(char *msg, void *actual){
  if(actual != NULL){
    printf("FAIL\n");
    printf("%s : expect= NULL actual= %p\n",msg,actual);
    printf("\n");
    return 1;
  }
  return 0;
}
  
// Utility to check if pointers are unique or point at the same
// location; if not print errors and return 1
int assert_unique_pointers(char *msg, void *x, void *y){
  if(x == y){
    printf("FAIL\n");
    printf("%s : pointers to same location, x: %p  y: %p\n",msg,x,y);
    printf("\n");
    return 1;
  }
  return 0;
}

#define BUFSZ 4096
static char STDOUT_BUF[BUFSZ+1]; // Buffer to capture standard out
static int  STDOUT_PIPE[2];      // Pipe to capture standard out
static int  STDOUT_BAK;          // Backup FD to swap stdout back in

// Temporarily direct stdout to a pipe which is captured for later
// use.
void catch_stdout(){
  pipe(STDOUT_PIPE);
  STDOUT_BAK = dup(STDOUT_FILENO);
  dup2(STDOUT_PIPE[PWRITE],STDOUT_FILENO);
  // printf() and its like will now print to the pipe
}  

// Restore stdout redirected via catch_stdout(); fill internal buffer
// to allow captured output to be retrieved
void restore_stdout(){
  dup2(STDOUT_BAK, STDOUT_FILENO);
  close(STDOUT_PIPE[PWRITE]);
  int bytes_read = read(STDOUT_PIPE[PREAD], STDOUT_BUF, BUFSZ);
  if(bytes_read >= BUFSZ){
    printf("Warning: could not capture all of standard output: insufficient buffer size\n");
  }
  STDOUT_BUF[bytes_read] = '\0';
  close(STDOUT_PIPE[PREAD]);
}

// Get a pointer to the output captured from stdout. Uses an internal
// buffer so overwritten each call to catch/restore, do not free().
char *captured_stdout(){
  return STDOUT_BUF;
}


