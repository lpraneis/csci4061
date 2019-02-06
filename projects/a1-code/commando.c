#include "commando.h"

int main(){
  char *argv_test[5] = {"0", "1", "2", "3", NULL};
  cmd_t *test = cmd_new(argv_test);
  return 0;
};
