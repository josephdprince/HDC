#include "src/HDC.h"
#include <stdio.h>

int main() {
  printf("Start of Program\n");
  // Testing HDvector===================================
  struct HDvector test1, test2;
  struct ENvector encoded;
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

  // testing basis=======================
  ///*
  printf("Generate basis vector\n");
  struct BasisVectors basis1;
  populateBasis(&basis1);
  print_basis(&basis1);
  //*/

  // testing encode============================
  encode(&test1, &basis1, &encoded);
  for (int i = 0; i < DIMENSIONS; ++i) {
    printf("%f", encoded.vector[i]);
  }
  printf("\n");

  // testing mapper
  struct ENvector encoded1;
  encoded1.max = 4;
  encoded1.min = 0;
  encoded1.vector[0] = 0;
  encoded1.vector[1] = 1;
  encoded1.vector[2] = 2;
  encoded1.vector[3] = 3;
  encoded1.vector[4] = 4;
  mapper(&encoded1);
  for (int i = 0; i < DIMENSIONS; ++i) {
    printf("%f ", encoded1.vector[i]);
  }
  return 0;
}
