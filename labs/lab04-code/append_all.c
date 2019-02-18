#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
  int cur_pos = 0;                                           // initial position in input buffer
  int max_size = 4;                                          // initial size of input buffer
  char *input = malloc(max_size*sizeof(char));               // allocate memory

  printf("Enter text. Press Ctrl-d to end input\n");

  while(1){                                                  // loop until end of input is indicated
    write(STDOUT_FILENO,"> ",2);                             // put prompt on screen
    if(cur_pos >= max_size){                                 // input buffer full
      max_size = max_size * 2;                               // double in size
      input = realloc(input, max_size);                      // reallocate buffer to larger size

      if(input == NULL){                                     // check for failure
        printf("could not expand input buffer; bailing out\n");
        exit(1);
      }
    }

    int max_read = max_size - cur_pos;                       // calculate maximum read size
    int nread = read(STDIN_FILENO, input+cur_pos, max_read); // perform read()
    // int nread = read(STDIN_FILENO, input, max_read);         // error using read()
    printf("read %d bytes\n",nread);
    
    if(nread == 0){                                          // 0 bytes read indicates end of file/input
      printf("reached end of input\n");
      break;
    }
    else if(nread == -1){
      perror("read failed");
      exit(1);
    }

    cur_pos += nread;                                        // succesful read, advance input buffer position

    printf("input uses %d of %d allocated bytes\n",
           cur_pos,max_size);
    printf("total input is:\n");
    printf("-------\n");
    int nwrite = write(STDOUT_FILENO, input, cur_pos);       // write to screen
    printf("-------\n");
    printf("wrote %d bytes\n",nwrite);

    if(nwrite == -1){                                        // check return value of write for errors
      perror("write failed");
      exit(1);
    }
    else if(nwrite < cur_pos){
      printf("wrote fewer bytes than expected: something is wrong\n");
      exit(1);
    }      
      
  }

  free(input);                                               // free memory heap allocated memory
  return 0;
}
