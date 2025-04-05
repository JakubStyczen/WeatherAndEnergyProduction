#include <gtest/gtest.h>

// Test prostego dodawania
TEST(MathTest, Addition) { EXPECT_EQ(2 + 2, 4); }

// Punkt startowy dla Google Test
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}