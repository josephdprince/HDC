#pragma once

/* Global variables */
#define DIMENSIONS 1000
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
            FeatType encoded[DIMENSIONS], FeatType* min, FeatType* max);

/* Does the matrix multiplication for the encode step */
//void matrixmult(//input
//				FeatType sample[FEATURES],
//				FeatType basis[FEATURES*DIMENSIONS],
//				FeatType minI, FeatType maxI,
//				//output
//                FeatType encoded[DIMENSIONS],
//				FeatType* minR, FeatType* maxR);
void matrixmult(//input
				FeatType sample[FEATURES],
				FeatType basis[FEATURES*DIMENSIONS],
				//output
                FeatType encoded[DIMENSIONS],
				FeatType* minR, FeatType* maxR);

/* Adds an encoded vector to the ClassList */
//void train(
//		//input
//		int numClass,
//		FeatType encoded[DIMENSIONS],
//		FeatType minI,
//		FeatType maxI,
//		//output
//		FeatType l[CLASSES][DIMENSIONS],
//		FeatType classMinMax[CLASSES][2]);
void train(
		//input
		int numClass,
		FeatType encoded[DIMENSIONS],
		//output
		FeatType l[CLASSES][DIMENSIONS],
		FeatType classMinMax[CLASSES][2]);

/* Map values of ENvectors data in ClassList to [-1,1] */
void normalize(FeatType l[CLASSES][DIMENSIONS], int numClass, FeatType* min, FeatType* max);

/* Helper for normalize that actually does the mapping */
void mapper(FeatType en[DIMENSIONS], FeatType* min, FeatType* max);

/* Compares an encoded sample to the closest class in ClassList using a
 * cosine similarity. Returns a numerical value of the classification. */
int similarity(FeatType encoded[DIMENSIONS], FeatType l[CLASSES][DIMENSIONS]);

/* Performs a cosine similarity between vector a and b. Returns the angle
 * between both vectors in radians. */
float cosinesim(FeatType a[], FeatType b[]);

