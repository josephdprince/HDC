#include "HDC.h"
#include <stdio.h>

int main() {
  printf("Start of Program\n");
  struct HDvector test1;
  rng_gen(&test1);
  printf("Print vector:\n");
  print_vector(&test1);
  // printf("Print full vector\n");
  // print_full_vector(test1);
  // printf("%f", test1.magnitude);
  return 0;
}