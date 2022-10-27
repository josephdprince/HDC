#include "HDC.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


float float_rand(float min, float max) {
  // generate random float number
  float scale = (float)rand() / (float)RAND_MAX; /* [0, 1.0] */
  return min + scale * (max - min);              /* [min, max] */
}

struct HDvector *rng_gen(struct HDvector *target) {
  // fill the given vector with randeom float # from -1 to 1
  srand(time(NULL));
  int length = FEATURES;
  float total = 0;
  float new_float = 0;

  for (int i = 0; i < length; i++) {
    new_float = float_rand(-1.0, 1.0);
    target->vector[i] = new_float;
    total += powf(new_float, 2);
    // printf ("new float ^2 = %f|| new total = %f\n" , pow(new_float,2),
    // total);
  }
  // printf("total^1/2 = %f\n", sqrtf(total));
  target->magnitude = sqrtf(total);
  return target;
}

void print_full_vector(struct HDvector *target) {
  printf("Printing a %i dimension vector w/ a magnitude of %f :\n", FEATURES,
         target->magnitude);
  int length = FEATURES;
  printf("[");
  for (int i = 0; i < length; i++) {
    printf("%f", target->vector[i]);
    if (i != length - 1) {
      printf(", ");
    }
  }
  printf("]\n");
}

void print_partial_vector(struct HDvector *target) {
  // only print the first 2 and last 2
  // assume target.vector size is atleast 2, should be 4
  printf("Printing a %i Dimension vector w/ a magnitude of %f :\n", FEATURES,
         target->magnitude);
  printf("[%f, %f, ..., %f, %f]\n", target->vector[0], target->vector[1],
         target->vector[FEATURES - 2], target->vector[FEATURES - 1]);
}

void print_vector(struct HDvector *target) {
  if (FEATURES <= 4) {
    print_full_vector(target);
  } else {
    print_partial_vector(target);
  }
}
