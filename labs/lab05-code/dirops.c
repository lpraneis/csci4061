#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#define NAMELEN 2048
#define BUFSIZE 1024

int main(int argc, char *argv[]) {
  long long max_size = 0;
  char max_name[NAMELEN] = {};
  struct stat sb;

  DIR *dir = opendir(".");
  struct dirent *file = NULL;
  while( (file = readdir(dir)) != NULL){
    stat(file->d_name, &sb);
    printf("%-30s : %6lld bytes\n",
           file->d_name, (long long) sb.st_size);
    if(sb.st_size > max_size){
      max_size = sb.st_size;
      strncpy(max_name, file->d_name, NAMELEN);
    }
  }
  closedir(dir);

  printf("Largest file: %s with %lld bytes\n",
         max_name, max_size);

  char copy_name[NAMELEN];
  snprintf(copy_name, NAMELEN, "%s.copy", max_name);
  printf("Copying %s to %s\n",max_name,copy_name);

  char buf[BUFSIZE];
  int infd = open(max_name, O_RDONLY);
  int flags = O_WRONLY | O_CREAT | O_TRUNC;
  /* mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP; */
  mode_t perms = 0777 & sb.st_mode;

  int outfd = open(copy_name, flags, perms);
  int nbytes;
  while( (nbytes = read(infd, buf, BUFSIZE)) > 0){
    write(outfd, buf, nbytes);
  }
  printf("Copy complete\n");
  close(infd);
  close(outfd);
  return 0;
}
