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

void encode(struct HDvector *hdv, struct BasisVectors *basis,
            struct ENvector *encoded) {
  encoded->min = DIMENSIONS;
  encoded->max = DIMENSIONS * -1;

  matrixmult(hdv, basis, encoded);

  mapper(encoded);
}

void matrixmult(struct HDvector *hdv, struct BasisVectors *basis,
                struct ENvector *encoded) {
  for (int i = 0; i < DIMENSIONS; ++i) {
    encoded->vector[i] = 0;
    for (int j = 0; j < FEATURES; ++j) {
      #pragma HLS loop_flatten
      #pragma HLS pipeline
      #pragma HLS unroll
      encoded->vector[i] += hdv->vector[j] * basis->b_vectors[i].vector[j];
    }
    encoded->max =
        encoded->vector[i] > encoded->max ? encoded->vector[i] : encoded->max;
    encoded->min =
        encoded->vector[i] < encoded->min ? encoded->vector[i] : encoded->min;
  }
}

void train(struct ClassList *l, int numClass, struct ENvector *sample) {
  for (int i = 0; i < DIMENSIONS; ++i) {
    l->classes[numClass].vector[i] += sample->vector[i];

    l->classes[numClass].max =
        l->classes[numClass].vector[i] > l->classes[numClass].max
            ? l->classes[numClass].vector[i]
            : l->classes[numClass].max;
    l->classes[numClass].min =
        l->classes[numClass].vector[i] < l->classes[numClass].min
            ? l->classes[numClass].vector[i]
            : l->classes[numClass].min;
  }
}

void normalize(struct ClassList *l, int numClass) {
  mapper(&l->classes[numClass]);
}

// output = output_start + ((output_end - output_start) / (input_end -
// input_start)) * (input - input_start)
void mapper(struct ENvector *en) {
  for (int i = 0; i < DIMENSIONS; ++i) {
    en->vector[i] = -1 + (2 / (en->max - en->min)) * (en->vector[i] - en->min);
  }
  en->min = -1;
  en->max = 1;
}

int similarity(struct ENvector *encoded, struct ClassList *l) {
  int closestClass = -1;
  float min = 10; // Just needs to be a value larger than pi
  float curr = 0.0;
  for (int i = 0; i < CLASSES; i++) {
    curr = cosinesim(l->classes[i].vector, encoded->vector);
    if (curr < min) {
      min = curr;
      closestClass = i;
    }
  }
  return closestClass;
}

float cosinesim(FeatType a[], FeatType b[]) {
  // Return arccos((a*b) / (|a|*|b|))
  FeatType magA = 0.0;
  FeatType magB = 0.0;
  FeatType dot = 0.0;
  for (int i = 0; i < DIMENSIONS; ++i) {
    magA += powf(a[i], 2); // FIXME: Overflow??
    magB += powf(b[i], 2); // FIXME: Overflow??

    dot += a[i] * b[i];
  }

  return acosf(dot / (sqrtf(magA) * sqrtf(magB)));
}

float float_rand(float min, float max) {
  // Generate random float number
  float scale = (float)rand() / (float)RAND_MAX; /* [0, 1.0] */
  return min + scale * (max - min);              /* [min, max] */
}

void rng_gen(struct HDvector *target) {
  // Fill the given vector with random float # from -1 to 1
  static char initial_setup = 1;
  if (initial_setup == 1) {
    srand(time(NULL));
    initial_setup = 0;
  }
  float total = 0;
  float new_float = 0;

  target->min = 1;
  target->max = -1;
  for (int i = 0; i < FEATURES; i++) {
    new_float = float_rand(-1.0, 1.0);
    target->vector[i] = new_float;
    target->max =
        target->vector[i] > target->max ? target->vector[i] : target->max;
    target->min =
        target->vector[i] < target->min ? target->vector[i] : target->min;
  }
}

void print_vector(struct HDvector *target, char includeInfo, char printSize) {
  // Print the full vector if the size is <= 4, otherwise just print partial
  if (includeInfo) {
    printf("Printing a %i-wide vector w/ a min of %f and a max of %f :\n",
           FEATURES, target->min, target->max);
  }
  if (printSize) {
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
  // Only print the first 2 and last 2
  // Assume target.vector size is at least 2, should be 4
  printf("[%f, %f, ..., %f, %f]", target->vector[0], target->vector[1],
         target->vector[FEATURES - 2], target->vector[FEATURES - 1]);
}

void print_encoded(struct ENvector *target, char includeInfo, char printSize) {
  // Print the full vector if the size is <= 4, otherwise just print partial
  if (includeInfo) {
    printf("Printing a %i-wide vector w/ a min of %f and a max of %f :\n",
           DIMENSIONS, target->min, target->max);
  }
  if (printSize) {
    print_full_encoded(target);
  } else {
    print_partial_encoded(target);
  }
}

void print_full_encoded(struct ENvector *target) {
  // This function print all the elements in the vector
  printf("[");
  for (int i = 0; i < DIMENSIONS; i++) {
    printf("%f", target->vector[i]);
    if (i != DIMENSIONS - 1) {
      printf(", ");
    }
  }
  printf("]");
}

void print_partial_encoded(struct ENvector *target) {
  // Only print the first 2 and last 2
  // Assume target.vector size is atleast 2, should be 4
  printf("[%f, %f, ..., %f, %f]", target->vector[0], target->vector[1],
         target->vector[DIMENSIONS - 2], target->vector[DIMENSIONS - 1]);
}

void print_basis(struct BasisVectors *target) {
  int top_bot_num = 2; // Decide how many elem at the top and at bottom to print
  printf("Printing a Basis-Vector of %i features by %i dimensions:\n", FEATURES,
         DIMENSIONS);
  printf("[\n");

  // Print [0, top_bot_num] of basis
  for (int i = 0; i < top_bot_num; i++) {
    printf("\t%i: ", i);

    print_vector(&(target->b_vectors[i]), FALSE, PARTIAL);
    printf("\n");
  }
  printf("\t.\n\t.\n\t.\n");

  // Print [DIMENSIONS - top_bot_num, DIMENSIONS] of basis
  for (int i = DIMENSIONS - top_bot_num; i < DIMENSIONS; ++i) {
    printf("\t%i: ", i);

    print_vector(&(target->b_vectors[i]), FALSE, PARTIAL);
    printf("\n");
  }

  printf("\n]\n");
}
