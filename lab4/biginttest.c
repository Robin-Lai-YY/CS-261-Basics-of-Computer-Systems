#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main(int argc, char *argv[]) {

  if (argc < 3) {
    fprintf(stderr, "%s: Invalid arguments. Arguments must be a sequence\n"
	    " of integers, each preceded by a base.\n", argv[0]);
    return 1;
  }

  char *endpos;
  unsigned int base_x = strtoul(argv[1], &endpos, 10);
  if (*endpos || base_x < MIN_BASE || base_x > MAX_BASE) {
    fprintf(stderr, "%s: Base '%s' is not a valid base.\n",
	    argv[0], argv[1]);
    return 2;
  }
  
  bigint_t *x = str_to_bigint(argv[2], &endpos, base_x);
  if (*endpos) {
    fprintf(stderr, "%s: '%s' is not a valid base-%u number.\n",
	    argv[0], argv[2], base_x);
    bigint_free(x);
    return 3;
  }

  printf("X (base %u) = ", base_x);
  print_bigint(x, stdout);
  printf("\n");

  for (int arg = 3; arg < argc; arg += 2) {

    printf("\n-- Processing arguments: '%s' '%s'\n",
	   argv[arg], argv[arg+1]);
    
    unsigned int base_y = strtoul(argv[arg], &endpos, 10);
    if (*endpos || base_y < MIN_BASE || base_y > MAX_BASE) {
      printf("   Base '%s' is not a valid base. Skipping...\n",
	     argv[1]);
      continue;
    }

    bigint_t *x_base_y = bigint_change_base(x, base_y);
    printf("   X (base %u) = ", base_y);
    print_bigint(x_base_y, stdout);
    printf("\n");
    bigint_free(x_base_y);

    if (argc > arg + 1) {

      bigint_t *y = str_to_bigint(argv[arg + 1], &endpos, base_y);
      if (*endpos) {
	printf("   '%s' is not a valid base-%u number. Skipping...\n",
	       argv[arg + 1], base_y);
	bigint_free(y);
	continue;
      }

      printf("   Y (base %u) = ", base_y);
      print_bigint(y, stdout);
      printf("\n");

      int cmp = bigint_cmp(x, y);
      printf("   X %c Y\n", cmp < 0 ? '<' : cmp > 0 ? '>' : '=');

      bigint_t *result = bigint_add(x, y);
      printf("   X+Y (base %u) = ", result->base);
      print_bigint(result, stdout);
      printf("\n");
      bigint_free(result);
      
      result = bigint_sub(x, y);
      printf("   X-Y (base %u) = ", result->base);
      print_bigint(result, stdout);
      printf("\n");
      bigint_free(result);
      
      bigint_free(y);
    }
  }

  bigint_free(x);

  return 0;
}
