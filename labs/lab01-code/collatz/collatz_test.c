#include <stdio.h>
#include "collatz.h"

int test = 0;
int failures = 0;

void test_next(int n, int expected){
  test++;
  printf("Test %2d : ",test);
  int actual = collatz_next(n);
  if(expected != actual){
    printf("FAILED\nCall: collatz_next(%d)\nExpect: %d\nActual: %d\n",
	   n,expected,actual);
    failures++; 
  }
  else{
    printf("ok\n");
  }
}

void test_steps(int n, int expected){
  test++;
  printf("Test %2d : ",test);
  int actual = collatz_steps(n,0);
  if(expected != actual){
    printf("FAILED\nCall: collatz_steps(%d)\nExpect: %d\nActual: %d\n",
	   n,expected,actual);
    failures++; 
    return;
  }
  else{
    printf("ok\n");
  }

}

int main(int argc, char **argv){
  test_next(1  , 1   );
  test_next(0  , -1  );
  test_next(-5 , -1  );
  test_next(5  , 16  );
  test_next(16 , 8   );
  test_next(20 , 10  );
  test_next(15 , 46  );

  test_steps(-7, 0 );
  test_steps(1, 0  );
  test_steps(5, 5  );
  test_steps(14, 17);
  test_steps(30, 18);
  test_steps(22, 15);
  test_steps(6, 8  );

  printf("----------------------------\n");
  printf("Overall %d / %d tests passed\n",test-failures,test);

  return 0;
}
