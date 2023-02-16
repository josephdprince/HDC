#include "HDC_HLS.h"
#include <math.h>

void encode(FeatType sample[FEATURES], FeatType basis[DIMENSIONS * FEATURES],
            FeatType encoded[DIMENSIONS]) {

  #pragma HLS INTERFACE m_axi port = sample offset = slave bundle = sample
  #pragma HLS INTERFACE s_axilite register port = sample bundle = ctrl
  #pragma HLS INTERFACE m_axi port = basis offset = slave bundle = basis
  #pragma HLS INTERFACE s_axilite register port = basis bundle = ctrl
  #pragma HLS INTERFACE m_axi port = encoded offset = slave bundle = encoded
  #pragma HLS INTERFACE s_axilite register port = encoded bundle = ctrl
  #pragma HLS INTERFACE s_axilite register port = return bundle = ctrl

  FeatType sample_local[FEATURES];
  FeatType basis_local[DIMENSIONS * FEATURES];
  FeatType encoded_local[DIMENSIONS];

  FeatType min = DIMENSIONS;
  FeatType max = DIMENSIONS * -1;

  // Partion the basis_local into blocks of size DIMENSIONS since the array can
  // be too big to load in at once
  #pragma HLS ARRAY_PARTITION variable = basis_local type = block factor =       \
    DIMENSIONS

  for (int i = 0; i < FEATURES; i++) {
    #pragma HLS pipeline
    sample_local[i] = sample[i];
  }

  for (int i = 0; i < FEATURES * DIMENSIONS; i++) {
    #pragma HLS pipeline
    basis_local[i] = basis[i];
  }

  matrixmult(sample_local, basis_local, encoded_local, &min, &max);
  mapper(encoded_local, &min, &max);
  for (int i = 0; i < DIMENSIONS; i++) {
    #pragma HLS pipeline
    encoded[i] = encoded_local[i];
  }
}

void matrixmult(FeatType sample[FEATURES],
                FeatType basis[FEATURES * DIMENSIONS],
                FeatType encoded[DIMENSIONS], FeatType *minR, FeatType *maxR) {
  for (int i = 0; i < DIMENSIONS; i++) {
    #pragma HLS pipeline
    encoded[i] = 0;
  }
  for (int i = 0; i < FEATURES * DIMENSIONS; i++) {
    #pragma HLS pipeline II = 2
    encoded[i / FEATURES] += sample[i % FEATURES] * basis[i];
  }

  for (int i = 0; i < DIMENSIONS; i++) {
    #pragma HLS pipeline
    *maxR = (encoded[i] > *maxR) ? encoded[i] : *maxR;
    *minR = (encoded[i] < *minR) ? encoded[i] : *minR;
  }
}

void train(int numClass, FeatType encoded[DIMENSIONS],
           FeatType l[CLASSES][DIMENSIONS], FeatType classMinMax[CLASSES][2]) {

  for (int i = 0; i < DIMENSIONS; ++i) {
    #pragma HLS pipeline
    l[numClass][i] += encoded[i];

    classMinMax[numClass][1] = (l[numClass][i] > classMinMax[numClass][1])
                                   ? l[numClass][i]
                                   : classMinMax[numClass][1];

    classMinMax[numClass][0] = (l[numClass][i] < classMinMax[numClass][0])
                                   ? l[numClass][i]
                                   : classMinMax[numClass][0];
  }
}

void normalize(FeatType l[CLASSES][DIMENSIONS], int numClass, FeatType *min,
               FeatType *max) {
  mapper(l[numClass], min, max);
}

void mapper(FeatType en[DIMENSIONS], FeatType *min, FeatType *max) {
  for (int i = 0; i < DIMENSIONS; ++i) {
    en[i] = -1 + (2 / (*max - *min)) * (en[i] - *min);
  }
  *min = -1;
  *max = 1;
}

int similarity(FeatType encoded[DIMENSIONS], FeatType l[CLASSES][DIMENSIONS]) {
  int closestClass = -1;
  float min = 10; // Just needs to be a value larger than pi
  float curr = 0.0;
  for (int i = 0; i < CLASSES; i++) {
    curr = cosinesim(l[i], encoded);
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
