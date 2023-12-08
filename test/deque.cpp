#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "deque.h"

using namespace tinySTL;

TEST(deque, constructor) {
  deque<int> dq(5, 3);
  EXPECT_STRING_EQ(dq, [3, 3, 3, 3, 3]);
}