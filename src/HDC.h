#pragma once

/* Global variables */
#define DIMENSIONS 10
#define FEATURES 28
#define CLASSES 10

#define FALSE 0
#define TRUE 1

#define PARTIAL 0
#define FULL 1

typedef float FeatType;

/* ============== Different abstract structs ============== */

/* A vector for all the data for each feature*/
struct HDvector {
  // To represent a single fecture vector of the basis vector
  FeatType min;
  FeatType max;
  FeatType vector[FEATURES];
};

/* An encoded vector */
struct ENvector {
  FeatType min;
  FeatType max;
  FeatType vector[DIMENSIONS];
};

/* Encoded sample of each class */
struct classList {
  struct ENvector classes[CLASSES];
};

/* Basis vector used for encoding */
struct BasisVectors {
  struct HDvector b_vectors[DIMENSIONS];
};

/* ============== Main fucntions for HDC ============== */

/* Populates each column in the basis matrix with random values from [-1,1] */
void populateBasis(struct BasisVectors *target);

/* Encodes a sample vector by multiplying it with the basis matrix */
void encode(struct HDvector *hdv, struct BasisVectors *basis,
            struct ENvector *encoded);

/* Adds an encoded vector to the classList */
void train(struct classList *l, int numClass, struct ENvector *sample);

/* Map values of ENvectors data in classList to [-1,1] */
void normalize(struct classList *l, int numClass);

/* Helper for normalize that actually does the mapping */
void mapper(struct ENvector *en);

/* Compares an encoded sample to the closest class in classList using a
 * cosine similarity. Returns a numerical value of the classification. */
int similarity(struct ENvector *sample, struct classList *l);

/* ============== Helper functions for HDvector ============== */

/* Performs a cosine similarity between vector a and b. Returns the angle
 * between two both vectors in radians */
float cosinesim(FeatType a[], FeatType b[]);

/* Generate random float number [min, max] */
float float_rand(float min, float max);

/* Populate target's vector with random FeatType element */
void rng_gen(struct HDvector *target);

/* Decide to print partial or full vector */
void print_vector(struct HDvector *target, char includeInfo, char printSize);
void print_full_vector(struct HDvector *target);
void print_partial_vector(struct HDvector *target);

void print_encoded(struct ENvector *target, char includeInfo, char printSize);
void print_full_encoded(struct ENvector *target);
void print_partial_encoded(struct ENvector *target);

/* Decide to print partial of the BasisVector */
void print_basis(struct BasisVectors *target);