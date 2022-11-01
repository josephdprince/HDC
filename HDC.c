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

struct HDvector* rng_gen(struct HDvector* target) { 
  // fill the given vector with randeom float # from -1 to 1
  static char initial_setup = 1;
  if (initial_setup == 1){
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
  return target;
}

void print_full_vector(struct HDvector *target) {
  //This function print all the elements in the vector
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

void print_vector(struct HDvector *target, char includeInfo) {
  //print the full vector if the size is <= 4, otherwise just print partial
  if (includeInfo){
    printf("Printing a %i-wide vector w/ a magnitude of %f :\n", FEATURES,target->magnitude);
  }
  if (FEATURES <= 4) {
    print_full_vector(target);
  } else {
    print_partial_vector(target);
  }
}

void populateBasis(struct BasisVectors* target){
  for (int i = 0; i < DIMENSIONS; i++){
    rng_gen(&(target->b_vectors[i]));
  }
}

void print_basis(struct BasisVectors* target){
  int top_bot_num = 2; // decide how many elem at the top and at bottom to print
  printf("Printing a Basis-Vector of %i features by %i dimensions:\n", 
    FEATURES, DIMENSIONS);
  printf("[\n");
  for(int i = 0; i < DIMENSIONS; i++){
    if(i < top_bot_num || i >= DIMENSIONS-top_bot_num){
      printf("\t");
      printf("%i: ", i);

      //print_full_vector(&target->b_vectors[i]);
      print_vector(&(target->b_vectors[i]), FALSE);
      if (i != DIMENSIONS - 1) {
        printf(",\n");
      }
    }
    if (i == 2 && !(i >= DIMENSIONS-top_bot_num)){
      printf("\t.\n\t.\n\t.\n");
    }
  }
  printf("\n]\n");
}
