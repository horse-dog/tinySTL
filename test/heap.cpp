#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "vector.h"
#include "tiny_heap.h"
#include <queue>

using namespace tinySTL;

TEST(heap, make_heap) {
  vector<int> vc {0, 1, 2, 3, 4, 8, 9, 3, 5};
  make_heap(vc.begin(), vc.end());
  EXPECT_STRING_EQ(vc, [9, 5, 8, 3, 4, 0, 2, 1, 3]);
}

TEST(heap, push_heap) {
  vector<int> vc {9, 5, 8, 3, 4, 0, 2, 1, 3};
  vc.push_back(7);
  push_heap(vc.begin(), vc.end());
  EXPECT_STRING_EQ(vc, [9, 7, 8, 3, 5, 0, 2, 1, 3, 4]);
}

TEST(heap, pop_heap) {
  vector<int> vc {9, 7, 8, 3, 5, 0, 2, 1, 3, 4};
  pop_heap(vc.begin(), vc.end());
  EXPECT_EQ(vc.back(), 9);
  vc.pop_back();
  EXPECT_STRING_EQ(vc, [8, 7, 4, 3, 5, 0, 2, 1, 3]);
}

TEST(heap, sort_heap) {
  vector<int> vc {1, 7, 4, 3, 5, 0, 2, 8, 3};
  make_heap(vc.begin(), vc.end());
  sort_heap(vc.begin(), vc.end());
  EXPECT_STRING_EQ(vc, [0, 1, 2, 3, 3, 4, 5, 7, 8]);
}