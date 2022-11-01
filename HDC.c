#include "HDC.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void populateBasis(struct BasisVectors *target) {
  for (int i = 0; i < DIMENSIONS; i++) {
    rng_gen(&(target->b_vectors[i]));
  }
}

void encode(struct HDvector *hdc, struct BasisVectors *basis,
            struct ENvector *encoded) {
  encoded->magnitude = 0;
  for (int i = 0; i < DIMENSIONS; ++i) {
    for (int j = 0; j < FEATURES; ++j) {
      encoded->vector[i] = hdc->vector[j] * basis->b_vectors[i].vector[j];
      encoded->magnitude += powf(encoded->vector[i], 2);
    }
  }
  encoded->magnitude = sqrtf(encoded->magnitude);
}

void train(struct classList *l, int numClass, struct ENvector *sample) {
  // Is a float large enough?
  float magnitude = 0;
  for (int i = 0; i < DIMENSIONS; ++i) {
    l->classes[numClass].vector[i] += sample->vector[i];
    magnitude += powf(l->classes[numClass].vector[i], 2);
  }
  magnitude = sqrtf(magnitude);
  l->classes[numClass].magnitude = magnitude;
}

void normalize(struct classList *l, int numClass) {
  for (int i = 0; i < DIMENSIONS; ++i) {
    l->classes[numClass].vector[i] /= l->classes[numClass].magnitude;
  }
}

float float_rand(float min, float max) {
  // generate random float number
  float scale = (float)rand() / (float)RAND_MAX; /* [0, 1.0] */
  return min + scale * (max - min);              /* [min, max] */
}

void rng_gen(struct HDvector *target) {
  // fill the given vector with randeom float # from -1 to 1
  static char initial_setup = 1;
  if (initial_setup == 1) {
    srand(time(NULL));
    initial_setup = 0;
  }
  float total = 0;
  float new_float = 0;

  for (int i = 0; i < FEATURES; i++) {
    new_float = float_rand(-1.0, 1.0);
    target->vector[i] = new_float;
    total += powf(new_float, 2);
  }
  target->magnitude = sqrtf(total);
}

void print_vector(struct HDvector *target, char includeInfo) {
  // print the full vector if the size is <= 4, otherwise just print partial
  if (includeInfo) {
    printf("Printing a %i-wide vector w/ a magnitude of %f :\n", FEATURES,
           target->magnitude);
  }
  if (FEATURES <= 4) {
    print_full_vector(target);
  } else {
    print_partial_vector(target);
  }
}

void print_full_vector(struct HDvector *target) {
  // This function print all the elements in the vector
  printf("[");
  for (int i = 0; i < FEATURES; i++) {
    printf("%f", target->vector[i]);
    if (i != FEATURES - 1) {
      printf(", ");
    }
  }
  printf("]");
}

void print_partial_vector(struct HDvector *target) {
  // only print the first 2 and last 2
  // assume target.vector size is atleast 2, should be 4
  printf("[%f, %f, ..., %f, %f]", target->vector[0], target->vector[1],
         target->vector[FEATURES - 2], target->vector[FEATURES - 1]);
}

void print_basis(struct BasisVectors *target) {
  int top_bot_num = 2; // decide how many elem at the top and at bottom to print
  printf("Printing a Basis-Vector of %i features by %i dimensions:\n", FEATURES,
         DIMENSIONS);
  printf("[\n");

  // Print [0, top_bot_num] of basis
  for (int i = 0; i < top_bot_num; i++) {
    printf("\t%i: ", i);

    // print_full_vector(&target->b_vectors[i]);
    print_vector(&(target->b_vectors[i]), FALSE);
    printf("\n");
  }
  printf("\t.\n\t.\n\t.\n");

  // Print [DIMENSIONS - top_bot_num, DIMENSIONS] of basis
  for (int i = DIMENSIONS - top_bot_num; i < DIMENSIONS; ++i) {
    printf("\t%i: ", i);

    // print_full_vector(&target->b_vectors[i]);
    print_vector(&(target->b_vectors[i]), FALSE);
    printf("\n");
  }

  printf("\n]\n");
}
