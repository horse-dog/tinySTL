#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "unordered_set.h"

using namespace tinySTL;

TEST(unordered_set, constructor) {
  hashtable<int, std::hash<int>, Eq__> x;
}