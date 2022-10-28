#include "HDC.h"
#include <stdio.h>

int main() {
  printf("Start of Program\n");
  //Testing HDvector===================================
  struct HDvector test1, test2;
  rng_gen(&test1);
  rng_gen(&test2);
  printf("Print vector:\n");
  print_vector(&test1, TRUE);
  printf("\n");
  print_vector(&test2, TRUE);
  printf("\n");
  // printf("Print full vector\n");
  // print_full_vector(test1);
  // printf("%f", test1.magnitude);

  //testing basis=======================
  ///*
  printf("Generate basis vector\n");
  struct BasisVectors basis1;
  populateBasis(&basis1);
  print_basis(&basis1);
  //*/
  return 0;
}
