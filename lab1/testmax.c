/*
 * testmax.c - Test the correctness of an external max function
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>

extern long max(long a, long b);

int main(int argc, char **argv) {
  
  int fail = 0;
  long rv;
  
  if ((rv = max(1, 2)) != 2) {
    printf("max(1, 2) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = max(2, 1)) != 2) {
    printf("max(2, 1) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = max(-1, 2)) != 2) {
    printf("max(-1, 2) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = max(2, -1)) != 2) {
    printf("max(2, -1) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = max(-1, -2)) != -1) {
    printf("max(-1, -2) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = max(-2, -1)) != -1) {
    printf("max(-2, -1) bad: %ld\n", rv);
    fail ++;
  }
  if (fail == 0)
    printf("max works\n");
  return fail;
}
