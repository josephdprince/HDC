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

/*========== Function to Synthesize ==========*/

/* Encodes a sample vector by multiplying it with the basis matrix */
void encode(FeatType sample[FEATURES], FeatType basis[DIMENSIONS*FEATURES],
            FeatType encoded[DIMENSIONS], float* min, float* max);

/* Does the matrix multiplication for the encode step */
void matrixmult(FeatType sample[FEATURES], FeatType basis[DIMENSIONS*FEATURES],
                FeatType encoded[DIMENSIONS], float* min, float* max);

/* Adds an encoded vector to the ClassList */
void train(FeatType l[CLASSES][DIMENSIONS], int numClass, FeatType encoded[DIMENSIONS], FeatType classMinMax[CLASSES][2]);

/* Map values of ENvectors data in ClassList to [-1,1] */
void normalize(FeatType l[CLASSES][DIMENSIONS], int numClass, float* min, float* max);

/* Helper for normalize that actually does the mapping */
void mapper(FeatType en[DIMENSIONS], float* min, float* max);

/* Compares an encoded sample to the closest class in ClassList using a
 * cosine similarity. Returns a numerical value of the classification. */
int similarity(FeatType encoded[DIMENSIONS], FeatType l[CLASSES][DIMENSIONS]);

/* Performs a cosine similarity between vector a and b. Returns the angle
 * between both vectors in radians. */
float cosinesim(FeatType a[], FeatType b[]);

