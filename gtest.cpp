#include "gtest/gtest.h"

TEST(SampleTest, Test1) { EXPECT_EQ("", ""); }

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}