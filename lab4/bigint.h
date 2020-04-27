#ifndef _BIGINT_H_
#define _BIGINT_H_

#include <stdio.h>
#include <stdint.h>

#define INVALID_DIGIT ((baseint_t) -1)
#define MIN_BASE      2
#define MAX_BASE      36

typedef uint_fast8_t baseint_t;

typedef struct digit {

  // Value of the digit
  baseint_t     value;
  
  // Next digit in the big integer. NULL if this is the least
  // significant (last) digit.
  struct digit *next;
  
  // Previous digit in the big integer. NULL if this is the most
  // significant (first) digit.
  struct digit *prev;
} digit_t;

typedef enum {
  SIGN_NEGATIVE = -1,
  SIGN_POSITIVE = +1,
  SIGN_ZERO = 0
} sign_t;

typedef struct bigint {

  // Sign of the big integer
  sign_t    sign;

  // Base of the big integer, between MIN_BASE and MAX_BASE
  baseint_t base;

  // First (most significant) digit of the big integer. Must always
  // exist. Can only have the value zero if sign is zero and it's the
  // only digit in the big integer.
  digit_t * first;

  // Last (least significant) digit of the big integer. Must always
  // exist.
  digit_t * last;
} bigint_t;

void bigint_free(bigint_t *bigint);

bigint_t *uint_to_bigint(uintmax_t value, baseint_t base);
bigint_t *int_to_bigint(intmax_t value, baseint_t base);
bigint_t *str_to_bigint(char *str, char **endptr, baseint_t base);
bigint_t *read_bigint(FILE *file, baseint_t base);

void print_bigint(bigint_t *bigint, FILE *file);
void bigint_to_str(bigint_t *bigint, char *buf);

uintmax_t bigint_num_digits(bigint_t *bigint);

bigint_t *bigint_dup(bigint_t *src);
bigint_t *bigint_change_base(bigint_t *old, baseint_t new_base);

int bigint_cmp(bigint_t *a, bigint_t *b);

bigint_t *bigint_add(bigint_t *a, bigint_t *b);
bigint_t *bigint_sub(bigint_t *a, bigint_t *b);

#endif
