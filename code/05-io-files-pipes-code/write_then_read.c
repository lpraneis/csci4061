// Basic demonstration of writing data to a file the reading it back
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 128

int main(int argc, char *argv[]){
  char *outfile = "existing.txt";                       // make sure this file exists

  printf("Opening file %s\n",outfile);
  int out_fd = open(outfile,O_WRONLY);                  // open for writing, must exist 
  if(out_fd == -1){                                     // check result and fail if not opened
    perror("Couldn't open output file");
    exit(1);
  }

  printf("Writing to file %s\n",outfile);
  char outbuf[BUFSIZE] = "here is some text to write";  // what to write
  int bytes_written = write(out_fd, outbuf, strlen(outbuf));   // do th write
  // int bytes_written = write(out_fd, outbuf, BUFSIZE);   // do th write
  if(bytes_written == -1){                              // check for errors
    perror("Failed to read from file");
    exit(1);
  }
  printf("Wrote %d bytes to %s\n",bytes_written,outfile);

  if(close(out_fd) == -1){                              // check for errors on close
    perror("Couldn't close file");
  }
  
  int in_fd = open(outfile,O_RDONLY);                   // open file for reading
  if(in_fd == -1){                                      // check result and fail if not opened
    perror("Couldn't open file");
    exit(1);
  }

  printf("Opening %s\n",outfile);

  printf("Reading up to %d bytes from %s\n",BUFSIZE,outfile);
  char buffer[BUFSIZE+1];                               // someplace to read into
  int bytes_read = read(in_fd, buffer, BUFSIZE);        // read from file, max BUFSIZE
  if(bytes_read == -1){                                 // check for errors
    perror("Failed to read from file");
    exit(1);
  }

  buffer[bytes_read] = '\0';                            // null terminate string
  printf("Read %d chars which are:\n",bytes_read);      // print bytes read
  printf("====\n%s\n====\n", buffer);                   // print what was read

  int result = close(in_fd);
  if(result == -1){
    perror("Failed to close file");
    exit(1);
  }

  return 0;
}
