/*
 * testcollatz.c - Test the correctness of an external collatz routine
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>

extern long collatz(long a);

int main(int argc, char **argv) {
  
  int fail = 0;
  long rv;
  
  if ((rv = collatz(1)) != 0) {
    printf("collatz(1) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = collatz(6)) != 8) {
    printf("collatz(6) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = collatz(19)) != 20) {
    printf("collatz(19) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = collatz(27)) != 111) {
    printf("collatz(27) bad: %ld\n", rv);
    fail ++;
  }
  if (fail == 0)
    printf("collatz works\n");
  return fail;
}

