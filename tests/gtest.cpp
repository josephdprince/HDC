#include "gtest/gtest.h"

extern "C" {
#include "../src/HDC.h"
}

#include <stdio.h>

TEST(EncodingTests, Test1) {
  struct BasisVectors basis;

  populateBasis(&basis);

  print_basis(&basis);

  EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}