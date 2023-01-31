#include "gtest/gtest.h"

extern "C" {
#include "../src/HDC.h"
}

#include <stdio.h>

TEST(EncodingTests, BASIS_RNG_TEST) {
  struct BasisVectors basis;

  // Generate values in basis matrix
  populateBasis(&basis);

  bool result = true;
  bool done = false;

  // Check if all values are between -1 and 1
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

  // Generate values in basis matrix
  populateBasis(&basis);

  bool result = true;

  // Calculate min/max values of each basis column
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
    // If object's min/max does not match calculated min/max, test case failed
    if (basis.b_vectors[i].min != min || basis.b_vectors[i].max != max) {
      result = false;
      break;
    }
  }

  EXPECT_TRUE(result);
}

TEST(EncodingTests, ENCODE_TEST) {
  struct BasisVectors basis;
  struct FTvector sample;
  struct ENvector encoded;

  // Generate values in basis matrix and sample vector
  populateBasis(&basis);
  rng_gen(&sample);

  // To check for accuracy of the matrix multiplication, hand calculations will
  // need to be performed (with lower values for global variables).
  encode(&sample, &basis, &encoded);

  // Print and check matrix multiplication by hand :/
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

  // Calculate min/max of encoded vector
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

TEST(TrainingTests, TRAIN_STEP) {
  struct BasisVectors basis;
  struct FTvector sample1, sample2;
  struct ENvector encoded1, encoded2;
  struct ClassList classes;

  // Specify which class data should fall into (val does not matter for testing)
  int numClass = 0;

  // Generate values in basis matrix and both sample vectors
  populateBasis(&basis);
  rng_gen(&sample1);
  rng_gen(&sample2);

  // Encode both sample vectors
  encode(&sample1, &basis, &encoded1);
  encode(&sample2, &basis, &encoded2);

  // Train both encoded vectors into class 0
  train(&classes, numClass, &encoded1);
  train(&classes, numClass, &encoded2);

  // Assuming that DIMENSIONS is greater than 0 (there is no reason why it
  // should be <= 0)
  EXPECT_EQ(classes.classes[numClass].vector[DIMENSIONS - 1],
            encoded1.vector[DIMENSIONS - 1] + encoded2.vector[DIMENSIONS - 1]);

  // Now check if min/max were successfully updated
  FeatType min = 5;
  FeatType max = -5;
  for (int i = 0; i < DIMENSIONS; ++i) {
    if (classes.classes[numClass].vector[i] < min) {
      min = classes.classes[numClass].vector[i];
    }
    if (classes.classes[numClass].vector[i] > max) {
      max = classes.classes[numClass].vector[i];
    }
  }

  EXPECT_EQ(classes.classes[numClass].min, min);
  EXPECT_EQ(classes.classes[numClass].max, max);
}

TEST(TrainingTests, NORMALIZE_AFTER_TRAIN) {
  struct BasisVectors basis;
  struct FTvector sample1, sample2;
  struct ENvector encoded1, encoded2;
  struct ClassList classes;

  // Specify which class data should fall into (val does not matter for testing)
  int numClass = 1;

  // Generate values in basis matrix and both sample vectors
  populateBasis(&basis);
  rng_gen(&sample1);
  rng_gen(&sample2);

  // Encode both sample vectors
  encode(&sample1, &basis, &encoded1);
  encode(&sample2, &basis, &encoded2);

  // Train both encoded vectors into class 0
  train(&classes, numClass, &encoded1);
  train(&classes, numClass, &encoded2);

  // Normalize the trained class
  normalize(&classes, numClass);

  // Check if all values are normalized between -1 and 1
  bool result = true;
  for (int i = 0; i < DIMENSIONS; ++i) {
    if (classes.classes[numClass].vector[i] < -1 ||
        classes.classes[numClass].vector[i] > 1) {
      result = false;
      break;
    }
  }

  // Also check min/max
  if (classes.classes[numClass].min != -1 ||
      classes.classes[numClass].max != 1) {
    result = false;
  }

  EXPECT_TRUE(result);
}

// This test is kinda tricky since we don't know whether or not the calculation
// is running correctly with randomly generated data. Instead, this test will
// just check if the code is able to make a decision
TEST(SimilarityTests, COSINE_SIM) {
  struct BasisVectors basis;
  struct FTvector sample, trainData1, trainData2, trainData3;
  struct ENvector encodedSample, encodedData1, encodedData2, encodedData3;
  struct ClassList classes;

  // Populate basis vector and sample data to be checked. Then, encode sample.
  populateBasis(&basis);
  rng_gen(&sample);
  encode(&sample, &basis, &encodedSample);

  // Start generating data and train into 3 separate classes
  int numDataSets = 10;
  for (int i = 0; i < numDataSets; ++i) {
    rng_gen(&trainData1);
    rng_gen(&trainData2);
    rng_gen(&trainData3);

    encode(&trainData1, &basis, &encodedData1);
    encode(&trainData2, &basis, &encodedData2);
    encode(&trainData3, &basis, &encodedData3);

    train(&classes, 0, &encodedData1);
    train(&classes, 1, &encodedData2);
    train(&classes, 2, &encodedData3);
  }

  // Now perform cosine similarity
  bool result;
  int classification = similarity(&encodedSample, &classes);

  result = classification >= CLASSES || classification < 0 ? false : true;

  if (result) {
    std::cout << "Sample data is closest to class " << classification
              << std::endl;
  }

  EXPECT_TRUE(result);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}