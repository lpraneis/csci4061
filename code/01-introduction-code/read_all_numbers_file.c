// Simple demonstration of reading user input using a linked list with
// dynamic memory allocation.
#include <stdio.h>
#include <stdlib.h>

typedef struct int_node_struct {                   // Definition of a node
  int data;                                        // integer data
  struct int_node_struct *next;                    // link to another node
} int_node;

int_node* head = NULL;                             // global variable, front of list

int main(int argc, char **argv){
  int x;
  FILE *input = fopen(argv[1], "r");               // open input file named on command line
  while(fscanf(input,"%d",&x) != EOF){             // read a number, check for end of input
    int_node *new = malloc(sizeof(int_node));      // allocate space for a node
    new->data = x;                                 // set data, -> derefernces and sets
    new->next = head;                              // point at previous front of list
    head = new;                                    // make this node the new front
  }
  int_node *ptr = head;                            // prepare to iterate through list
  int i=0;
  printf("\nEntire list\n");
  while(ptr != NULL){                              // iterate until out of nodes
    printf("list(%d) = %d\n",i,ptr->data);         // print data for one node
    ptr = ptr->next;                               // move pointer forward one node
    i++;
  }
  fclose(input);                                   // close the input file
  return 0;                                        // Should free list but program is ending
}                                                  // so memory will automatically return to system
