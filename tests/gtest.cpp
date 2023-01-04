#include "gtest/gtest.h"

extern "C" {
#include "../src/HDC.h"
}

#include <iostream>
#include <stdio.h>

TEST(EncodingTests, BASIS_RNG_TEST) {
  struct BasisVectors basis;

  populateBasis(&basis);

  bool result = true;
  bool done = false;
  for (int i = 0; i < DIMENSIONS; ++i) {
    for (int j = 0; j < FEATURES; ++j) {
      if (basis.b_vectors[i].vector[j] < -1 ||
          basis.b_vectors[i].vector[j] > 1) {
        result = false;
        done = true;
        break;
      }
    }
    if (done) {
      break;
    }
  }

  EXPECT_TRUE(result);
}

TEST(EncodingTests, MIN_MAX_TEST) {
  struct BasisVectors basis;

  populateBasis(&basis);

  bool result = true;
  for (int i = 0; i < DIMENSIONS; ++i) {
    FeatType min = 1;
    FeatType max = -1;
    for (int j = 0; j < FEATURES; ++j) {
      if (basis.b_vectors[i].vector[j] < min) {
        min = basis.b_vectors[i].vector[j];
      }
      if (basis.b_vectors[i].vector[j] > max) {
        max = basis.b_vectors[i].vector[j];
      }
    }
    if (basis.b_vectors[i].min != min || basis.b_vectors[i].max != max) {
      result = false;
      break;
    }
  }

  EXPECT_TRUE(result);
}

TEST(EncodingTests, ENCODE_TEST) {
  struct BasisVectors basis;
  struct HDvector sample;
  struct ENvector encoded;

  populateBasis(&basis);
  rng_gen(&sample);

  // To check for accuracy of the matrix multiplication, hand calculations will
  // need to be performed (with lower values for global variables).
  encode(&sample, &basis, &encoded);

  // print_encoded(&encoded, TRUE, FULL);

  // Instead, we will check if min and max are successfully updated
  FeatType min =
      FEATURES; // The largest possible value of each position is FEATURES
                // (in the unlikely event that the sample is filled with all
                // values being '1' and if a column of the basis is filled
                // with all values being '1'. Since sample is FEATURES long
                // and basis is FEATURES tall, we would get '1' multiplied
                // with '1' for FEATURES amount of times, hence max value is
                // FEATURES)
  FeatType max = FEATURES * -1; // Similarly, the minimum value of a position in
                                // the encoded vector is -(FEATURES)
  for (int i = 0; i < DIMENSIONS; ++i) {
    if (encoded.vector[i] < min) {
      min = encoded.vector[i];
    }
    if (encoded.vector[i] > max) {
      max = encoded.vector[i];
    }
  }

  EXPECT_EQ(encoded.min, min);
  EXPECT_EQ(encoded.max, max);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}