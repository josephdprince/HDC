#include "src/HDC.h"
#include <stdio.h>
#include <unistd.h>

#define TRAIN 1
#define TEST 0

#define PIXEL_WIDTH 28
#define PIXEL_HEIGHT 28

int parse_file_and_classify(int mode, struct FTvector *sample,
                            struct BasisVectors *basis,
                            struct ENvector *encoded,
                            struct ClassList *classes);

void classify_train(struct FTvector *sample, struct BasisVectors *basis,
                    struct ENvector *encoded, struct ClassList *classes,
                    int classIdentity);

void classify_test(struct FTvector *sample, struct BasisVectors *basis,
                   struct ENvector *encoded, struct ClassList *classes,
                   int classIdentity, int *correct, int *total);

int main() {
  struct FTvector sample;
  struct BasisVectors basis;
  struct ENvector encoded;
  struct ClassList classes;

  populateBasis(&basis);

  // Get data from train dataset and populate into class list
  printf("Starting Train\n");
  int status =
      parse_file_and_classify(TRAIN, &sample, &basis, &encoded, &classes);
  if (status == -1) {
    return status;
  }

  // Now all the classes are filled, but they need to be normalized.
  for (int i = 0; i < CLASSES; ++i) {
    normalize(&classes, i);
  }

  // Get data from test dataset and run a similarity
  printf("Starting Test\n");
  status = parse_file_and_classify(TEST, &sample, &basis, &encoded, &classes);
  if (status == -1) {
    return status;
  }

  return 0;
}

int parse_file_and_classify(int mode, struct FTvector *sample,
                            struct BasisVectors *basis,
                            struct ENvector *encoded,
                            struct ClassList *classes) {
  FILE *fp;
  if (mode) {
    fp = fopen("mnist_train.csv", "r");
  } else {
    fp = fopen("mnist_test.csv", "r");
  }
  if (fp == NULL) {
    perror("Error in opening file");
    return (-1);
  }

  // Each file has metadata on the first line. Iterate through the first line to
  // start data parsing after the metadata
  int curr = fgetc(fp);
  while (curr != '\n') {
    curr = fgetc(fp);
  }
  curr = 0;

  int correct = 0;
  int total = 0;

  // Parse data and store in a sample FTvector
  while (!feof(fp)) {
    // Default values of sample min and max
    sample->max = 0;
    sample->min = 255;

    // At this point, curr is the identity of the data
    curr = fgetc(fp);
    int c = 0;
    while (curr < 32 && !feof(fp)) {
      curr = fgetc(fp);
      c++;
      if (c > 50) {
        printf("HELP I AM STUCK! ");
      }
    }
    int classIdentity = curr - 48;

    int pixelVal = 0;
    int loc = 0;

    // Get the first value into curr
    curr = fgetc(fp);
    curr = fgetc(fp);

    // Iterate through one line at a time
    while (curr >= 32 && !feof(fp)) {
      if (curr != ',') {
        pixelVal *= 10;
        pixelVal += curr - 48;
      } else {
        sample->vector[loc] = pixelVal;

        sample->max = pixelVal > sample->max ? pixelVal : sample->max;
        sample->min = pixelVal < sample->min ? pixelVal : sample->min;

        pixelVal = 0;
        ++loc;
      }

      // Get next value
      curr = fgetc(fp);

      // Handle last pixel
      if (curr <= 32) {
        sample->vector[loc] = pixelVal;
      }
    }

    // At this point, sample is filled with data. Now run the train portion of
    // the classification algorithm if mode == 1 else run test portion
    if (mode) {
      classify_train(sample, basis, encoded, classes, classIdentity);
    } else {
      if (!feof(fp)) {
        classify_test(sample, basis, encoded, classes, classIdentity, &correct,
                      &total);
      }
    }
  }

  if (!mode) {
    printf("\nThe classification algorithm passed %i/%i tests for a %.2f%% "
           "accuracy.\n",
           correct, total, (double)(correct * 100) / (double)total);
  }

  fclose(fp);

  return 0;
}

void classify_train(struct FTvector *sample, struct BasisVectors *basis,
                    struct ENvector *encoded, struct ClassList *classes,
                    int classIdentity) {
  // Encode data using basis vector
  encode(sample, basis, encoded);

  // Train data into each class
  train(classes, classIdentity, encoded);
}

void classify_test(struct FTvector *sample, struct BasisVectors *basis,
                   struct ENvector *encoded, struct ClassList *classes,
                   int classIdentity, int *correct, int *total) {
  encode(sample, basis, encoded);

  int classification = similarity(encoded, classes);
  printf("Identity: %i   Classification: %i   ", classIdentity, classification);
  if (classification == classIdentity) {
    printf("PASSED\n");
    ++(*correct);
  } else {
    printf("FAILED\n");
  }
  ++(*total);
}