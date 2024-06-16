#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "unordered_set.h"

using namespace tinySTL;

TEST(unordered_set, constructor) {
  std::hash<int> hf;
  Eq__ __eql;
  _Identity<int> __ext;
  tinySTL::hashtable<int, int, std::hash<int>, _Identity<int>, Eq__>
  x(8, hf, __eql, __ext);
}