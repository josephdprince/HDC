#pragma once

/* Global variables */
#define DIMENSIONS 10000
#define FEATURES 784
#define CLASSES 10

typedef float FeatType;

struct HDvector {
  FeatType magnitude;
  FeatType vector[FEATURES];
};

struct ENvector {
  FeatType magnitude;
  FeatType vector[DIMENSIONS];
};

struct classList {
  FeatType classes[CLASSES][DIMENSIONS];
};

/* Populates each column in the basis matrix with random values from [-1,1] */
void populateBasis(struct HDvector *hdc);

/* Encodes a sample vector by multiplying it with the basis matrix */
struct ENvector* encode(struct HDvector *hdc);

/* Encodes a sample vector and adds it to the classList */
void train(struct classList *l, int numClass, struct ENvector *sample);

/* Encodes a sample and compares it to the closest class in classList. Return is
 * a numerical value of the classification */
int similarity(struct HDvector *hdc, FeatType sample[]);

/* Helper functuins for HDvector */
float float_rand(float min, float max);
struct HDvector* rng_gen(struct HDvector *target);
void print_full_vector(struct HDvector *target);
void print_partial_vector(struct HDvector *target);
void print_vector(struct HDvector *target);
