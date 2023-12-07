#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "forward_list.h"

using namespace tinySTL;

TEST(forward_list, constructor) {
  forward_list<int> fl= {1, 2, 3};
  EXPECT_STRING_EQ(fl, [1, 2, 3]);
}