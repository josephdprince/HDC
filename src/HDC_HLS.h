#pragma once

/* Global variables */
#define DIMENSIONS 1024
#define FEATURES 784
#define PARTITIONS 128
#define CLASSES 10

#define NUMTRAIN 60000
#define NUMTEST 10000

#define FALSE 0
#define TRUE 1

#define PARTIAL 0
#define FULL 1

typedef float FeatType;

/*========== Function to Synthesize ==========*/

/* Wrapper function that performs the entire classification algorithm. */
void wrapper(FeatType samples[(NUMTRAIN + NUMTEST) * FEATURES],
        FeatType basis[DIMENSIONS * FEATURES], int ids[NUMTRAIN + NUMTEST],
        int results[NUMTEST]);

/* Encodes a sample vector by multiplying it with the basis matrix */
void encode(FeatType sample[FEATURES], FeatType basis[DIMENSIONS * FEATURES],
            FeatType encoded_local[DIMENSIONS]);

/* Does the matrix multiplication for the encode step. Input sample and basis.
 * Output encoded, minRm and maxR */
void matrixmult(FeatType sample[FEATURES],
                FeatType basis[FEATURES * DIMENSIONS],
                FeatType encoded[DIMENSIONS], int cycle, FeatType *minR,
                FeatType *maxR);

/* Adds an encoded vector to the ClassList. Input numClass and encoded. Output l
 * and classMinMax */
void train(int numClass, FeatType encoded[DIMENSIONS],
           FeatType classes[CLASSES * DIMENSIONS]);

/* Map values of ENvectors data in ClassList to [-1,1] */
void normalize(FeatType classes[CLASSES * DIMENSIONS], int numClass);

/* Helper for normalize that actually does the mapping */
void mapper(FeatType en[DIMENSIONS], FeatType *min, FeatType *max);

/* Compares an encoded sample to the closest class in ClassList using a
 * cosine similarity. Returns a numerical value of the classification. */
int similarity(FeatType encoded[DIMENSIONS],
               FeatType classes[CLASSES * DIMENSIONS]);

/* Performs a cosine similarity between vector a and b. Returns the angle
 * between both vectors in radians. */
float cosinesim(FeatType classes[CLASSES * DIMENSIONS], FeatType b[],
                int curr_class);
