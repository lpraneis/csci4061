// check_spelling_busy.c: simple spell checking program that uses one
// thread to read inputs from stdin and another to look for words in a
// dictionary.  Uses a bounded buffer (queue) to store pending words
// for consideration. Uses a loop to repeatedly check for empty/full
// queues.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>

#define WORD_QUEUE_CAPACITY 5                                  // max capacity for the queue

char *word_queue[WORD_QUEUE_CAPACITY];                         // fixed size queue 
int word_queue_begin = 0;                                      // front of the queue
int word_queue_end   = 0;                                      // back of the queue
int word_queue_size  = 0;                                      // # elements in queue

pthread_mutex_t word_queue_lock;                               // lock controlling access to the queue

int quit_now = 0;                                              // indicates whether to shut down

void *read_words(void *param);
void *check_words(void *param);

int main(){
  setvbuf(stdout, NULL, _IONBF, 0);                            // turn off output buffering
  pthread_mutex_init(&word_queue_lock,NULL);                   // init mutex

  pthread_t read_thread, check_thread;                         
  pthread_create(&read_thread,  NULL, read_words,  NULL);      // start children
  pthread_create(&check_thread, NULL, check_words, NULL);

  pthread_join(read_thread,  NULL);                            // wait for children
  pthread_join(check_thread, NULL);

  return 0;
}

void *read_words(void *param){                                 // function for reading words form user
  printf("Enter words to check spelling, Ctrl-d to end input\n");
  while(1){
    char check_word[256];
    int ret = scanf("%s",check_word);
    if(ret == EOF){
      break;
    }
    char *check_word_copy = strdup(check_word);

    while(1){                                                  // loop to check for space in queue
      pthread_mutex_lock(&word_queue_lock);                    // acquire lock
      if(word_queue_size != WORD_QUEUE_CAPACITY){              // check for space
        break;                                                 // yes, break out
      }
      pthread_mutex_unlock(&word_queue_lock);                  // no, loop back and try again
    }

    // at this point that the queue has space for a word in it at
    // word_queue_end

    word_queue[word_queue_end] = check_word_copy;
    word_queue_end = (word_queue_end + 1 ) % WORD_QUEUE_CAPACITY;
    word_queue_size++;                                         // unlock queue as enqueue is finished
    pthread_mutex_unlock(&word_queue_lock);
  }
  quit_now = 1;
  return NULL;
}


void *check_words(void *param){                                // function which checks words in queue
  FILE *dict_file = fopen("english-dict.txt","r");             // open dictionary file

  while(1){                                                    // loop continuously

    while(1){                                                  // loop to check if queue is not empty, bail if quitting
      pthread_mutex_lock(&word_queue_lock);                    // acquire lock on queue
      if(word_queue_size != 0 || quit_now == 1){               // check if queue is empty / time to quit
        break;                                                 // yes break
      }
      pthread_mutex_unlock(&word_queue_lock);                  // nope, unlock and try again
    }
    if(quit_now == 1 && word_queue_size == 0){                 // loop until reader thread indicates we are done
      break;
    }

    // at this point that the queue has a word in it at
    // word_queue_begin

    char *check_word = word_queue[word_queue_begin];           // pull off an element from the queue
    word_queue_begin = (word_queue_begin + 1 ) % WORD_QUEUE_CAPACITY;
    word_queue_size--;
    pthread_mutex_unlock(&word_queue_lock);                    // unlock queue as dequeue is finished
      
    rewind(dict_file);                                         // check the word
    while(1){
      char dict_word[256];
      int ret = fscanf(dict_file,"%s",dict_word);
      if(ret == EOF){                                          // end of file, word not in dictionary
        printf("'%s' INCORRECT\n",check_word);
        break;
      }
      if( strcmp(check_word, dict_word)==0 ){                  // check for correct spelling
        printf("'%s' ok\n",check_word);
        break;
      }
    }
    free(check_word);                                          // free duplicated word
  }                                                            // loop back to look for another word in the queue

  fclose(dict_file);                                           // close dictionary file
  return NULL;
}
