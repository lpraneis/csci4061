// worm_pthreads.c: demonstrates multiple thread running at different
// rates, sharing global data via shared file, avoiding conflicts
// mutexes. Visualize the runing threads by 'watching' the file
// worm_board.txt via the command
//
// > watch -n 0.1 'cat worm_board.txt'
//
// then start the program running. Must be compiled with the pthread
// library linked as in
//
// > gcc -Wall -g -c worms_pthread.c
// > gcc -Wall -g -o worms_pthread worms_pthread.o -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>

#define MAX_ROW    10                            // size of board in rows
#define MAX_COL    10                            // size of board in cols
#define PRINT_MAX  50                            // max #chars that wroms print out
#define NSTEPS    100                            // number of steps worms take
#define NWORMS      4
  
char *board[MAX_ROW];                            // 2D array of characters
char *print_rows[NWORMS];                        // rows at which to print stats
pthread_mutex_t mutexes[MAX_ROW][MAX_COL];       // array of locks

typedef struct {                                 // type that carries worm parameters
  int id;                                        // id #
  char display_char;                             // character to display
  double delay;                                  // delay between moves in seconds 
  int row, col;                                  // starting row/col
} wormparam_t;

void *worm_func(void *param);                    // function worms run, takes a wormparam_t pointer

int main(){
  int fd = open("worm_board.txt",                // file descriptor for the board file
                O_CREAT|O_RDWR,
                S_IRUSR|S_IWUSR);
  int board_size = (MAX_ROW) * (MAX_COL+1);      // +1 for newlines
  int print_size = 1 + NWORMS * (PRINT_MAX+1);   // newline + 4 worms each printing a line with a newline
  int file_size = board_size + print_size;       // total file size
  ftruncate(fd, file_size);                      // set size of file to specified
  char *file_bytes=                              // pointer to memory mapped file contents
    mmap(NULL, file_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  
  int fidx = 0;                                  // linear index into file
  for(int i=0; i<MAX_ROW; i++){                  // populate the 2D board area with dots
    board[i] = &file_bytes[fidx];                // track row start position for 2D access via board[i][j]
    for(int j=0; j<MAX_COL; j++){
      file_bytes[fidx++] = '.';                  // dots fill board initially
    }
    file_bytes[fidx++] = '\n';
  }

  file_bytes[fidx++] = '\n';                     // newline betwen board and printing area

  for(int i=0; i<NWORMS; i++){                   // populate the printing area with spaces
    print_rows[i] = &file_bytes[fidx];           // track print area row starts for easy printing
    for(int j=0; j<PRINT_MAX; j++){
      file_bytes[fidx++] = ' ';
    }
    file_bytes[fidx++] = '\n';
  }

  for(int i=0; i<MAX_ROW; i++){                  // initialize the mutexes
    for(int j=0; j<MAX_COL; j++){
      pthread_mutex_init(&mutexes[i][j],NULL);
    }
  }

  wormparam_t params[NWORMS] = {                 // parameters for the 4 worms
    {.id=0, .display_char='E', .delay=0.50, .row=0, .col=0,  },
    {.id=1, .display_char='A', .delay=0.25, .row=MAX_ROW-1, .col=0, },
    {.id=2, .display_char='G', .delay=0.60, .row=MAX_ROW-1, .col=MAX_COL-1, },
    {.id=3, .display_char='L', .delay=0.10, .row=0, .col=MAX_COL-1, },
  };

  printf("in separate terminal, use command the \n");
  printf("  > watch -n 0.1 'cat worm_board.txt'\n");
  printf("in this directory to observer the worms\n");
  printf("press 'enter' to start\n");
  getchar();
  printf("Running worm threads...\n");

  pthread_t worm_threads[NWORMS];                // array of thread types for the worm threads

  for(int i=0; i < NWORMS; i++){                 // parent generates child threads
    pthread_create(&worm_threads[i], NULL, worm_func, &params[i]);
  }
  for(int i=0; i < NWORMS; i++){                 // main thread waits on all children to finish
    pthread_join(worm_threads[i], NULL);
  }
  printf("All threads complete.\n");

  for(int i=0; i<MAX_ROW; i++){                  // destroy the mutexes
    for(int j=0; j<MAX_COL; j++){
      pthread_mutex_destroy(&mutexes[i][j]);
    }
  }

  munmap(board, file_size);                      // unmap and close file
  close(fd);
  return 0;
}

// function run by each thread to produce 'worm' output
void *worm_func(void *p){                                      // function which moves a 'worm' around in the file
  wormparam_t *param = (wormparam_t *) p;                      // caste parameter from void
  int id = param->id;                                          // local copies of parameters
  int row=param->row, col=param->col;
  char display_char = param->display_char;
  int udelay = (int) (1e6 * param->delay);                     // calculate microsecond delay as fractional second delay

  board[row][col] = display_char;                              // place the initial worm character on the board
  int territory = 1;                                           // worms start with 1 position

  int moves[4][2] = {                                          // 4 possible moves
    {+1, 0}, {-1, 0}, { 0,-1}, { 0,+1},                            
    // up     down      left     right
  };

  unsigned int state = time(NULL);                             // initialize private state for random number generation

  for(int step=0; step<NSTEPS; step++){                        // loop over # steps
    int movei = rand_r(&state) % 4;                            // select a random move to start with
    for(int i=0; i<4; i++, movei=(movei+1)%4){                 // loop over all possible moves
      int new_row = row + moves[movei][0];                     // calculate new position
      int new_col = col + moves[movei][1];
      if(   new_row < 0 || new_row >= MAX_ROW                  // check if in bounds
         || new_col < 0 || new_col >= MAX_COL)
      {
        continue;                                              // nope: loop back around and try again
      }

      pthread_mutex_lock(&mutexes[new_row][new_col]);          // lock the inbounds position to preven other worms from using it
      if(   board[new_row][new_col] != '.'                     // check if it is available or...
         && board[new_row][new_col] != tolower(display_char))  // part of this worm's territory
      {
        pthread_mutex_unlock(&mutexes[new_row][new_col]);      // nope, unlock position and 
        continue;                                              // loop back around and try another move
      }

      if(board[new_row][new_col] == '.'){                      // got an empty spot, increase territory
        territory++;
      }
       
      board[row][col] = tolower(display_char);                 // found a valid location, lowercase old position
      row = new_row; col = new_col;                            // update current position
      board[row][col] = display_char;                          // mark new position
      pthread_mutex_unlock(&mutexes[new_row][new_col]);        // unlock position
      break;                                                   // made a valid move, bail out
    }

    int nprint =
      sprintf(print_rows[id],                                  // update print area with new psition
              "%c: step %3d (%2d,%2d) territory: %2d",
              display_char,step,row,col,territory);
    print_rows[id][nprint] = ' ';                              // eliminate nulll character sprintf() puts in output

    usleep(udelay);                                            // sleep until trying another move
  }

  return NULL;                                                 // thread returns null
}
