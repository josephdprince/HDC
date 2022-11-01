#pragma once

/* Global variables */
#define DIMENSIONS 10000
#define FEATURES 28
#define CLASSES 10

#define FALSE 0
#define TRUE 1

typedef float FeatType;

/* Different abstract structs====================================== */

struct HDvector {
  //To represent a single fecture vector of the basis vector
  FeatType magnitude;
  FeatType vector[FEATURES];
};

struct ENvector {
  /*
  To represent an encoded vector after the dot product of the sample 
  vector with the basis vectors
  */
  FeatType magnitude;
  FeatType vector[DIMENSIONS];
};

struct classList {
  //The classes we try to classify 
  struct ENvector classes[CLASSES];
};

struct BasisVectors{
  //Representation of a basis vector for encoding 
  struct HDvector b_vectors[DIMENSIONS];
};

/* Main important fucntion for HDC============================================= */

/* Populates each column in the basis matrix with random values from [-1,1] */
void populateBasis(struct BasisVectors* target);

/* Encodes a sample vector by multiplying it with the basis matrix */
void encode(struct HDvector *hdc, FeatType sample[]);

/* Encodes a sample vector and adds it to the classList */
void train(struct HDvector *hdc, FeatType sample[]);

/* Encodes a sample and compares it to the closest class in classList. Return is
 * a numerical value of the classification */
int similarity(struct HDvector *hdc, FeatType sample[]);

/* Helper functions for HDvector=============================================== */

/* Generate random float number [min, max] */
float float_rand(float min, float max);

/* Generate Vector of size FEATURES with random FeatType element */
struct HDvector *rng_gen(struct HDvector *target);

void print_full_vector(struct HDvector *target);
void print_partial_vector(struct HDvector *target);

/* Decide to print partial or full vector */
void print_vector(struct HDvector *target, char includeInfo);

/* Decide to print partial of the BasisVector */
void print_basis(struct BasisVectors* target);