#include "bigint.h"

#include <stdlib.h>
#include <ctype.h>

/* Allocates and initializes a new digit struct. Also updates
   neighbouring digits to point to the new digit.
   
   Parameters:
   - value: digit value of the new digit, must be between 0 and
     base-1.
   - next: digit to the right of the new digit. Will also be updated
     to point to new digit.
   - prev: digit to the left of the new digit. Will also be updated
     to point to new digit.

   Returns: a pointer to the newly allocated digit.
 */
static inline
digit_t *new_digit(baseint_t value, digit_t *next, digit_t *prev) {

  /* TO BE COMPLETED BY THE STUDENT */
  return NULL;
}

/* Allocates and initializes a new big integer struct with a single
   digit.
   
   Parameters:
   - sign: the sign of the new number (positive, negative, zero).
   - base: the base at which the number is represented. Must be
     between MIN_BASE and MAX_BASE.
   - digit_value: the value of the single digit. Must be between
     0 and base-1.
   
   Returns: a pointer to the newly allocated big integer.
 */
static inline
bigint_t *new_bigint(sign_t sign, baseint_t base, baseint_t digit_value) {

  /* TO BE COMPLETED BY THE STUDENT */
  return NULL;
}

/* Ensures that the big integer data structure is clean. Removes (and
   frees) leading zeros (ensuring that the number always has at least
   one digit), and updates the sign to SIGN_ZERO if there is only a
   single digit with the value zero.
   
   Parameter: bigint: the big integer to be cleaned up.

   Returns: the same bigint passed as a parameter.
 */
static inline bigint_t *cleanup_bigint(bigint_t *bigint) {

  /* TO BE COMPLETED BY THE STUDENT */
  return bigint;
}

/* Frees a big integer and all its digits.
   
   Parameter: bigint: the big integer to be freed.
 */
void bigint_free(bigint_t *bigint) {
  
  /* TO BE COMPLETED BY THE STUDENT */
}

/* Converts an unsigned integer to a big integer.
   
   Parameters:
   - value: the unsigned integer to be converted to a big integer.
   - base: the base at which the big integer is to be
     represented. Must be between MIN_BASE and MAX_BASE.

   Returns: a newly allocated big integer representing the same
   numeric value as 'value'.
 */
bigint_t *uint_to_bigint(uintmax_t value, baseint_t base) {

  /* TO BE COMPLETED BY THE STUDENT */
  return NULL;
}

/* Converts a signed integer to a big integer.
   
   Parameters:
   - value: the signed integer to be converted to a big integer.
   - base: the base at which the big integer is to be
     represented. Must be between MIN_BASE and MAX_BASE.

   Returns: a newly allocated big integer representing the same
   numeric value as 'value'.
 */
bigint_t *int_to_bigint(intmax_t value, baseint_t base) {

  if (value >= 0)
    return uint_to_bigint(value, base);
  else {
    bigint_t *bigint = uint_to_bigint(-value, base);
    bigint->sign = SIGN_NEGATIVE;
    return bigint;
  }
}

/* Converts a character to the digit value it represents. */
static inline baseint_t char_to_digit(int c) {

  if ('0' <= c && c <= '9')
    return c - '0';
  else if ('A' <= c && c <= 'Z')
    return c - 'A' + 10;
  else if ('a' <= c && c <= 'z')
    return c - 'a' + 10;
  else
    return INVALID_DIGIT;
}

/* Converts a digit value to the character representing it. */
static inline char digit_to_char(baseint_t num) {
  
  if (num <= 9)
    return num + '0';
  else if (num <= 35)
    return num + 'a' - 10;
  else
    return '?';
}

/* Converts the initial part of the string in str to a big integer
   according to the given base. The string may begin with an arbitrary
   amount of white space (as determined by isspace) followed by a
   single optional '+' or '-' sign.  The remainder of the string is
   converted to a big integer in the regular manner, stopping at the
   first character which is not a valid digit in the given base (as
   determined by the char_to_digit function).
   
   Parameters:
   - str: the string to be converted to a big integer.
   - endptr: if endptr is not NULL, this function stores the address
     of the first invalid character in *endptr. If there were no
     digits at all, it stores the original value of str (or a few
     characters ahead, if spaces and/or a sign are found) in *endptr,
     and returns the big integer representing zero. In particular, if
     **endptr is '\0' on return, the entire string is valid.
   - base: the base at which the number is represented. Must be
     between MIN_BASE and MAX_BASE.

   Returns: a newly allocated big integer representing the same
   numeric value as 'str'.
 */
bigint_t *str_to_bigint(char *str, char **endptr, baseint_t base) {

  /* TO BE COMPLETED BY THE STUDENT */
  return NULL;
}

/* Reads a big integer from a file or terminal in the given base.
   
   An arbitrary amount of white space (as determined by isspace) in
   the beginning is ignored. There may be a single optional '+' or '-'
   sign. The remainder of the input is converted to a big integer in
   the regular manner, stopping at the first character which is not a
   valid digit in the given base (as determined by the char_to_digit
   function). This invalid digit is returned (using ungetc) to the
   stream.
   
   Parameters:
   - file: the file or terminal to read data from. This may be 'stdin'
     to read from the standard input.
   - base: the base at which the number is represented. Must be
     between MIN_BASE and MAX_BASE.

   Returns: a newly allocated big integer representing the value read
   from the file pointer.
 */
bigint_t *read_bigint(FILE *file, baseint_t base) {

  /* TO BE COMPLETED BY THE STUDENT */
  return NULL;
}

/* Prints a big integer to a file or terminal in its original base.
   If the big integer is negative, a '-' is printed. After that, the
   number is printed using one character per digit (as determined by
   digit_to_char), with no leading zeros.
   
   Parameters:
   - bigint: the big integer to be printed.
   - file: the file or terminal to print data to. This may be 'stdout'
     to print to the standard output.
 */
void print_bigint(bigint_t *bigint, FILE *file) {

  /* TO BE COMPLETED BY THE STUDENT */
}

/* Converts a big integer into its string representation in its
   original base If the big integer is negative, the string starts
   with a '-'. After that, the string is filled with one character per
   digit (as determined by digit_to_char), with no leading zeros.
   
   Parameters:
   - bigint: the big integer to be printed.
   - buf: the string buffer to be filled with the number's
     digits. Must be at least bigint_num_digits(bigint)+2 long (to
     account for the sign and termination byte).
 */
void bigint_to_str(bigint_t *bigint, char *buf) {

  /* TO BE COMPLETED BY THE STUDENT */
}

/* Counts the number of digits in a big integer.

   Parameters: bigint: the big integer to be evaluated.
   
   Returns: a positive integer representing the number of digits in
   the big integer. The sign is not counted. A value of zero is
   considered to have one digit.
 */
uintmax_t bigint_num_digits(bigint_t *bigint) {

  /* TO BE COMPLETED BY THE STUDENT */
  return 0;
}

/* Divides a big integer by an unsigned integer divisor. This function
   can be used, among other things, to assist in the convertion of a
   big integer into a new base.

   Parameters:
   - dividend: the big integer corresponding to the dividend.
   - divisor: the number that the dividend will be divided by. Cannot
     be zero.
   - remainder: if not NULL, the remainder of the division will be
     stored into *remainder.
   
   Returns: a newly allocated big integer corresponding to the result
   of the division of dividend by divisor.
 */
static inline
bigint_t *bigint_divide_by_uint(bigint_t *dividend, baseint_t divisor,
				baseint_t *remainder) {

  /* TO BE COMPLETED BY THE STUDENT */
  return NULL;
}

/* Creates a new copy of a big integer. The copy will represent the
   same number in the same base, with new digits.

   Parameters: src: the big integer to be copied.
   
   Returns: a newly allocated big integer representing the same number
   as src in the same base.
 */
bigint_t *bigint_dup(bigint_t *src) {

  /* TO BE COMPLETED BY THE STUDENT */
  return NULL;
}

/* Creates a copy of a big integer, replacing the base with a new
   base. The copy will represent the same number but with a different
   base, performing the appropriate computations as required.

   Parameters:
   - old: the big integer to be copied.
   - base: the base at which the new number is to be represented. Must
     be between MIN_BASE and MAX_BASE.
   
   Returns: a newly allocated big integer representing the same number
   as src in a different base.
 */
bigint_t *bigint_change_base(bigint_t *old, baseint_t new_base) {

  if (new_base == old->base) return bigint_dup(old);

  /* TO BE COMPLETED BY THE STUDENT */
  return NULL;
}

/* Compares the absolute values of two big integers. The two integers
   may not be represented in the same base.
   
   Parameters: a, b: big integers to be compared.
   
   Returns: 0 (zero) if |a| = |b|; some negative number if |a| < |b|;
   or some positive number if |a| > |b|.
 */
static inline
int bigint_mod_cmp(bigint_t *a, bigint_t *b) {

  /* TO BE COMPLETED BY THE STUDENT */
  return 0;
}

/* Compares two big integers. The two integers may not be represented
   in the same base.
   
   Parameters: a, b: big integers to be compared.
   
   Returns: 0 (zero) if a = b; some negative number if a < b; or some
   positive number if a > b.
 */
int bigint_cmp(bigint_t *a, bigint_t *b) {

  if (a->sign != b->sign)
    return a->sign - b->sign;
  else if (a->sign == SIGN_ZERO)
    return 0;
  else
    return a->sign * bigint_mod_cmp(a, b);
}

/* Creates a new big integer representing the sum of two big
   integers.
   
   Parameters: a, b: operands of the add operation. Numbers may not
   have the same base.
   
   Returns: a newly allocated big integer representing the sum of the
   numbers in a and b, in the base of a.
 */
bigint_t *bigint_add(bigint_t *a, bigint_t *b) {

  /* TO BE COMPLETED BY THE STUDENT - BONUS */
  return bigint_dup(a);
}

/* Creates a new big integer representing the subtraction of two big
   integers.
   
   Parameters: a, b: operands of the add operation. Numbers may not
   have the same base.
   
   Returns: a newly allocated big integer representing the value of a
   minus the value of b, in the base of a.
 */
bigint_t *bigint_sub(bigint_t *a, bigint_t *b) {

  b->sign = -b->sign;
  bigint_t *result = bigint_add(a, b);
  b->sign = -b->sign;
  return result;
}

