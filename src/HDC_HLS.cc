#include "HDC_HLS.h"
#include <math.h>

void wrapper(FeatType samples[(NUMTRAIN + NUMTEST) * FEATURES],
             FeatType basis[DIMENSIONS * FEATURES], int ids[NUMTRAIN + NUMTEST],
             int results[NUMTEST]) {

  #pragma HLS INTERFACE m_axi port=samples offset=slave bundle=samples
  #pragma HLS INTERFACE s_axilite register port=samples bundle=ctrl
  #pragma HLS INTERFACE m_axi port=basis offset=slave bundle=basis
  #pragma HLS INTERFACE s_axilite register port=basis bundle=ctrl
  #pragma HLS INTERFACE m_axi port=ids offset=slave bundle=ids
  #pragma HLS INTERFACE s_axilite register port=ids bundle=ctrl
  #pragma HLS INTERFACE m_axi port=results offset=slave bundle=results
  #pragma HLS INTERFACE s_axilite register port=results bundle=ctrl
  #pragma HLS INTERFACE s_axilite register port=return bundle=ctrl

  FeatType sample_local[FEATURES];
  FeatType encoded_local[DIMENSIONS];
  FeatType classes[CLASSES * DIMENSIONS];

  // Handle train data
  for (int i = 0; i < NUMTRAIN; ++i) {
    #pragma HLS pipeline
    // Populate local sample
    for (int j = 0; j < FEATURES; ++j) {
      #pragma HLS unroll
      sample_local[j] = samples[FEATURES * i + j];
    }

    // Encode the local sample
    encode(sample_local, basis, encoded_local);

    // Train data into classes
    train(ids[i], encoded_local, classes);
  }

  // Now normalize all the classes
  for (int i = 0; i < CLASSES; ++i) {
    normalize(classes, i);
  }

  // Handle test data
  for (int i = NUMTRAIN; i < NUMTRAIN + NUMTEST; ++i) {
    #pragma HLS pipeline
    // Populate local sample
    for (int j = 0; j < FEATURES; ++j) {
      #pragma HLS unroll
      sample_local[j] = samples[FEATURES * i + j];
    }

    // Encode the local sample
    encode(sample_local, basis, encoded_local);

    // Get result
    results[i - NUMTRAIN] = similarity(encoded_local, classes);
  }
}

void encode(FeatType sample[FEATURES], FeatType basis[DIMENSIONS * FEATURES],
            FeatType encoded_local[DIMENSIONS]) {
  FeatType basis_local[FEATURES * PARTITIONS];

  FeatType min = DIMENSIONS;
  FeatType max = DIMENSIONS * -1;

  // Populate encoded_local with 0
  for (int i = 0; i < DIMENSIONS; ++i) {
    #pragma HLS pipeline
    encoded_local[i] = 0;
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

    matrixmult(sample, basis_local, encoded_local, i, &min, &max);
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
  #pragma HLS ARRAY_PARTITION variable = basis type = block factor = 128
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
           FeatType classes[CLASSES * DIMENSIONS]) {

  int start = numClass * DIMENSIONS;
  for (int i = 0; i < DIMENSIONS; ++i) {
    #pragma HLS pipeline
    classes[start + i] += encoded[i];
  }
}

void normalize(FeatType classes[CLASSES * DIMENSIONS], int numClass) {
  FeatType max = DIMENSIONS * -1;
  FeatType min = DIMENSIONS;

  int start = numClass * DIMENSIONS;

  // Get max and min
  for (int i = 0; i < DIMENSIONS; ++i) {
    #pragma HLS pipeline
    max = classes[start + i] > max ? classes[start + i] : max;
    min = classes[start + i] < min ? classes[start + i] : min;
  }

  // Linear mapping
  for (int i = 0; i < DIMENSIONS; ++i) {
    #pragma HLS pipeline
    classes[start + i] = -1 + (2 / (max - min)) * (classes[start + i] - min);
  }
}

void mapper(FeatType en[DIMENSIONS], FeatType *min, FeatType *max) {
  for (int i = 0; i < DIMENSIONS; ++i) {
    #pragma HLS pipeline
    en[i] = -1 + (2 / (*max - *min)) * (en[i] - *min);
  }
  *min = -1;
  *max = 1;
}

int similarity(FeatType encoded[DIMENSIONS],
               FeatType classes[CLASSES * DIMENSIONS]) {
  int closestClass = -1;
  float min = 10; // Just needs to be a value larger than pi
  float curr = 0.0;
  for (int i = 0; i < CLASSES; i++) {
    #pragma HLS pipeline
    curr = cosinesim(classes, encoded, i);
    if (curr < min) {
      min = curr;
      closestClass = i;
    }
  }
  return closestClass;
}

float cosinesim(FeatType classes[CLASSES * DIMENSIONS], FeatType b[],
                int curr_class) {
  // Return arccos((a*b) / (|a|*|b|))
  FeatType magA = 0.0;
  FeatType magB = 0.0;
  FeatType dot = 0.0;
  int start = curr_class * DIMENSIONS;
  for (int i = 0; i < DIMENSIONS; ++i) {
    #pragma HLS pipeline
    magA += powf(classes[start + i], 2);
    magB += powf(b[i], 2);

    dot += classes[i] * b[i];
  }

  return acosf(dot / (sqrtf(magA) * sqrtf(magB)));
}
