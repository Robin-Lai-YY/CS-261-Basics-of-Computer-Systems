/*
 * testarraymax.c - Test the correctness of an external arraymax function
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>

extern long arraymax(long *p, long c);

long vals[] = { -3, 1, 0, 9, 4 };

int main(int argc, char **argv) {

  int fail = 0;
  long rv;

  if ((rv = arraymax(vals, 0)) != 0x8000000000000000) {
    printf("arraymax(vals, 0) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = arraymax(vals, 1)) != -3) {
    printf("arraymax(vals, 1) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = arraymax(vals, 2)) != 1) {
    printf("arraymax(vals, 2) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = arraymax(vals, 3)) != 1) {
    printf("arraymax(vals, 3) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = arraymax(vals, 4)) != 9) {
    printf("arraymax(vals, 4) bad: %ld\n", rv);
    fail ++;
  }
  if ((rv = arraymax(vals, 5)) != 9) {
    printf("arraymax(vals, 5) bad: %ld\n", rv);
    fail ++;
  }
  if (fail == 0)
    printf("arraymax works\n");
  return fail;
}
