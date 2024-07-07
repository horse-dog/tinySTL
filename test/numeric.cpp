#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "vector.h"
#include "numeric.h"

using namespace tinySTL;

TEST(numeric, accumulate) {
  vector<int> vc0 {1, 4, 2, 5, 0};
  EXPECT_EQ(tinySTL::accumulate(vc0.begin(), vc0.end(), 0), 12);

  vector<int> vc1 {1, 4, 2, 5, 2};
  EXPECT_EQ(tinySTL::accumulate(vc1.begin(), vc1.end(), 1, [](const int& acc, const int& cur) { return acc * cur; }), 80);
}

TEST(numeric, partial_sum) {
  vector<int> vc0 {1, 4, 2, 5, 0};
  vector<int> result0(vc0.size());
  tinySTL::partial_sum(vc0.begin(), vc0.end(), result0.begin());
  EXPECT_STRING_EQ(result0, [1, 5, 7, 12, 12]);

  vector<int> vc1 {1, 4, 2, 5, 0};
  vector<int> result1(vc1.size());
  tinySTL::partial_sum(vc1.begin(), vc1.end(), result1.begin(), [](const int& pre, const int& cur) { return pre * cur; });
  EXPECT_STRING_EQ(result1, [1, 4, 8, 40, 0]);
}

TEST(numeric, inner_product) {
  {
    vector<int> vc0 {1, 4, 2, 5, 0};
    vector<int> vc1 {1, 4, 2, 5, 0};
    EXPECT_EQ(tinySTL::inner_product(vc0.begin(), vc0.end(), vc1.begin(), 0), 46);
  }
  {
    vector<int> vc0 {1, 4, 2, 5, 1};
    vector<int> vc1 {1, 4, 2, 5, 1}; // 2 8 4 10 2
    auto sum_op = [](const int& a, const int& b) { return a * b; };
    auto pdt_op = [](const int& a, const int& b) { return a + b; };
    EXPECT_EQ(tinySTL::inner_product(vc0.begin(), vc0.end(), vc1.begin(), 1, sum_op, pdt_op), 1280);
  }
}

TEST(numeric, adjacent_difference) {
  {
    vector<int> vc0 {1, 4, 2, 5, 1};
    vector<int> result0(vc0.size());
    tinySTL::adjacent_difference(vc0.begin(), vc0.end(), result0.begin());
    EXPECT_STRING_EQ(result0, [1, 3, -2, 3, -4]);
  }

  {
    vector<int> vc0 {1, 4, 2, 5, 1};
    vector<int> result0(vc0.size());
    auto op = [](const int& a, const int b) { return 2*a - b; };
    tinySTL::adjacent_difference(vc0.begin(), vc0.end(), result0.begin(), op);
    EXPECT_STRING_EQ(result0, [1, 7, 0, 8, -3]);
  }
}
