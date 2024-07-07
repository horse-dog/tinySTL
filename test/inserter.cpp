#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "list.h"
#include "vector.h"
#include "iterator.h"
#include "algorithm.h"

using namespace tinySTL;

TEST(inserter, back_inserter) {
  list<int> li1 {1, 2, 2, 3}, li2 {1, 2, 5};
  list<int> result;
  tinySTL::merge(li1.begin(), li1.end(), li2.begin(), li2.end(), tinySTL::back_inserter(result));
  EXPECT_STRING_EQ(result, [1, 1, 2, 2, 2, 3, 5]);
}

TEST(inserter, front_inserter) {
  list<int> li1 {1, 2, 2, 3}, li2 {1, 2, 5};
  list<int> result;
  tinySTL::merge(li1.begin(), li1.end(), li2.begin(), li2.end(), tinySTL::front_inserter(result));
  EXPECT_STRING_EQ(result, [5, 3, 2, 2, 2, 1, 1]);
}

TEST(inserter, inserter) {
  {
    list<int> li1 {1, 2, 2, 3}, li2 {1, 2, 5};
    list<int> result {11, 22};
    tinySTL::merge(li1.begin(), li1.end(), li2.begin(), li2.end(), tinySTL::inserter(result, ++result.begin()));
    EXPECT_STRING_EQ(result, [11, 1, 1, 2, 2, 2, 3, 5, 22]);
  }
  {
    vector<int> vc1 {1, 2, 2, 3}, vc2 {1, 2, 5};
    vector<int> result {11, 22};
    tinySTL::merge(vc1.begin(), vc1.end(), vc2.begin(), vc2.end(), tinySTL::inserter(result, ++result.begin()));
    EXPECT_STRING_EQ(result, [11, 1, 1, 2, 2, 2, 3, 5, 22]);
  }
}
