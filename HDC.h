#pragma once

/* Global variables */
#define DIMENSIONS 10000
#define FEATURES 28
#define CLASSES 10

#define FALSE 0
#define TRUE 1

typedef float FeatType;

struct HDvector {
  float magnitude;
  FeatType vector[FEATURES];
};

struct BasisVectors{
  struct HDvector b_vectors[DIMENSIONS];
};

/* Populates each column in the basis matrix with random values from [-1,1] */
void populateBasis(struct BasisVectors* target);

/* Encodes a sample vector by multiplying it with the basis matrix */
void encode(struct HDvector *hdc, FeatType sample[]);

/* Encodes a sample vector and adds it to the classList */
void train(struct HDvector *hdc, FeatType sample[]);

/* Encodes a sample and compares it to the closest class in classList. Return is
 * a numerical value of the classification */
int similarity(struct HDvector *hdc, FeatType sample[]);

/* Helper functions for HDvector */
float float_rand(float min, float max);
struct HDvector *rng_gen(struct HDvector *target);
void print_full_vector(struct HDvector *target);
void print_partial_vector(struct HDvector *target);
void print_vector(struct HDvector *target, char includeInfo);

/* Helper functions for BasisVectors*/
void print_basis(struct BasisVectors* target);