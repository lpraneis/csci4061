#include "commando.h"
#include "minunit.h"

int tests_run = 0;

static char *test_read_all() {
  int bytes_read = 0;
  char* test= read_all(STDIN_FILENO, &bytes_read);
  printf("Actual bytes_read = %d\n", bytes_read);
  mu_assert("Error, bytes_read != 1026 ", bytes_read==1026);

  return 0;
}

static char *run_tests() { 
  mu_run_test(test_read_all);
  return 0;
}


int main(int argc, char **argv){
  printf("About to run tests: \n");
  char *result = run_tests();
  if (result !=0){
    printf("%s\n", result);
  } else {
    printf("All tests passed\n");
  }
  printf("Tests run: %d\n", tests_run);

  return result != 0;
}

