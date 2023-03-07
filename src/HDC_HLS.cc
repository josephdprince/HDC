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
  FeatType basis_local[FEATURES * PARTITIONS];
  FeatType encoded_local[DIMENSIONS];

  FeatType min = DIMENSIONS;
  FeatType max = DIMENSIONS * -1;

  // Populate encoded_local with 0
  for (int i = 0; i < DIMENSIONS; ++i) {
    #pragma HLS pipeline
    encoded_local[i] = 0;
  }

  // Copy data into sample_local
  for (int i = 0; i < FEATURES; i++) {
    #pragma HLS pipeline
    sample_local[i] = sample[i];
  }

  // Perform matrix multiplication in parts due to limited BRAM on Pynq Z2 board
  // Size of the basis is FEATURES * DIMENSIONS
  // Load data of i-th partition (0 to cycle) into local basis.
  // Size of a local basis partition = FEATURES * PARTITIONS.
  // i * FEATURES * PARTITIONS = i * Size of a local basis partition
  //                           = Starting index of the i-th partition of basis
  // j is from 0 to size of the partition = var to help copy the whole partition
  int cycle = DIMENSIONS / PARTITIONS;
  for (int i = 0; i < cycle; ++i) {
    for (int j = 0; j < FEATURES * PARTITIONS; j++) {
      #pragma HLS pipeline
      basis_local[j] = basis[i * FEATURES * PARTITIONS + j];
    }

    matrixmult(sample_local, basis_local, encoded_local, i, &min, &max);
  }

  mapper(encoded_local, &min, &max);
  for (int i = 0; i < DIMENSIONS; i++) {
    #pragma HLS pipeline
    encoded[i] = encoded_local[i];
  }
}

void matrixmult(FeatType sample[FEATURES],
                FeatType basis[FEATURES * PARTITIONS],
                FeatType encoded[DIMENSIONS], int cycle, FeatType *minR,
                FeatType *maxR) {

  // Start is the index of the first element in current partition
  // End is the index of the last element in current patition
  // Note: In this function, basis is a partition of the original basis
  int start = PARTITIONS * cycle;
  int end = start + PARTITIONS;
  #pragma HLS ARRAY_PARTITION variable=basis type=block factor=PARTITIONS
  for (int i = start; i < end; ++i) {
    #pragma HLS unroll
    for (int j = 0; j < FEATURES; ++j) {
      // i - start is how many columns we have currently processed
      // Each column is of size FEATURES
      encoded[i] += sample[j] * basis[FEATURES * (i - start) + j];
    }
  }

  for (int i = start; i < end; i++) {
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
