#pragma once

/* Global variables */
#define DIMENSIONS 10000
#define FEATURES 784
#define CLASSES 10

typedef float FeatType;

struct HDC {
  int dimensions;
  int features;
  int classes;

  FeatType basis[FEATURES][DIMENSIONS];
  FeatType classList[CLASSES][DIMENSIONS];
};

/* Note: HLS requires that pointers are not used, and C does not allow an array
 * return value. Therefore, a struct hdc instance must be passed as a value to
 * below functions with a struct HDC return value. We need this return value
 * because the functions will make a copy of HDC without saving any updates.
 * Updating the copy of HDC in main will fix this issue but will incur a need
 * for more space. In the future, we should look at a workaround to this space
 * issue. */

/* Populates each column in the basis matrix with random values from [-1,1] */
struct HDC populateBasis(struct HDC hdc);

/* Encodes a sample vector by multiplying it with the basis matrix */
struct HDC encode(struct HDC hdc, FeatType sample[]);

/* Encodes a sample vector and adds it to the classList */
struct HDC train(struct HDC hdc, FeatType sample[]);

/* Encodes a sample and compares it to the closest class in classList. Return is
 * a numerical value of the classification */
int similarity(struct HDC hdc, FeatType sample[]);