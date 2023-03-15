#include "HDC_HLS.h"
#include <math.h>

void wrapper(FeatType samples[NUMTEST * FEATURES],
             FeatType basis[DIMENSIONS * FEATURES],
             FeatType results[NUMTEST * DIMENSIONS]) {

  #pragma HLS INTERFACE m_axi port=samples offset=slave bundle=samples
  #pragma HLS INTERFACE s_axilite register port=samples bundle=ctrl
  #pragma HLS INTERFACE m_axi port=basis offset=slave bundle=basis
  #pragma HLS INTERFACE s_axilite register port=basis bundle=ctrl
  #pragma HLS INTERFACE m_axi port=results offset=slave bundle=results
  #pragma HLS INTERFACE s_axilite register port=results bundle=ctrl
  #pragma HLS INTERFACE s_axilite register port=return bundle=ctrl

  FeatType sample_local[FEATURES];
  FeatType encoded_local[DIMENSIONS];

  // Encode each sample
  for (int i = 0; i < NUMTEST; ++i) {
    int start = FEATURES * i;

    // Populate sample_local
    for (int j = 0; j < FEATURES; ++j) {
      #pragma HLS pipeline
      sample_local[j] = samples[start + j];
    }

    // Zero out encoded_local
    for (int j = 0; j < DIMENSIONS; ++j) {
      #pragma HLS pipeline
      encoded_local[j] = 0;
    }

    // Now encode the sample
    encode(sample_local, basis, encoded_local);

    // Store result
    for (int j = 0; j < DIMENSIONS; ++j) {
      #pragma HLS pipeline
      results[start + j] = encoded_local[j];
    }
  }
}

void encode(FeatType sample_local[FEATURES],
            FeatType basis[DIMENSIONS * FEATURES],
            FeatType encoded_local[DIMENSIONS]) {
  FeatType basis_local[FEATURES * PARTITIONS];

  FeatType min = DIMENSIONS;
  FeatType max = DIMENSIONS * -1;

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

void mapper(FeatType en[DIMENSIONS], FeatType *min, FeatType *max) {
  for (int i = 0; i < DIMENSIONS; ++i) {
    en[i] = -1 + (2 / (*max - *min)) * (en[i] - *min);
  }
  *min = -1;
  *max = 1;
}