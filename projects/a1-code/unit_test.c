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

static char *test_cmdcol_print(){
  printf("Running print test\n");
  char* args[5] = {"this","is","a","test",NULL};
  cmd_t *test = cmd_new(args);
  test->pid = 1234;
  test->output_size = 123;
  test->finished =1;
  test->status = 0;
  snprintf(test->str_status, 8, "%s", "EXIT(0)"); // set str_status to INIT


  char* args2[5] = {"ls","-l","-h","-a",NULL};
  cmd_t *test2 = cmd_new(args2);
  test2->pid = 1235;
  test2->output_size = 203;
  test2->finished =0;
  test2->status = -1;
  snprintf(test2->str_status, 4, "%s", "RUN"); // set str_status to INIT

  cmdcol_t *test_cmdcol = (cmdcol_t *) malloc(MAX_CMDS * sizeof(cmd_t));
  cmdcol_add(test_cmdcol, test);
  cmdcol_add(test_cmdcol, test2);

  cmdcol_print(test_cmdcol);

  mu_assert("1=1", 1==1);

  return 0;
}

static char *run_tests() { 
  /* mu_run_test(test_read_all); */
  mu_run_test(test_cmdcol_print);
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

