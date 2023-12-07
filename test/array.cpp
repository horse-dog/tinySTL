#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "array.h"

using namespace tinySTL;

TEST(array, constructor) {
  array<int, 3> arr1 = {1, 2, 3};
  array<int, 3> arr2;
  EXPECT_STRING_EQ(arr1, [1, 2, 3]);
  arr2 = arr1;
  arr1[0] = 11;
  EXPECT_STRING_EQ(arr1, [11, 2, 3]);
  EXPECT_STRING_EQ(arr2, [1, 2, 3]);
}

TEST(array, fill) {
  array<int, 3> arr = {1, 2, 3};
  EXPECT_STRING_EQ(arr, [1, 2, 3]);
  arr.fill(1024);
  EXPECT_STRING_EQ(arr, [1024, 1024, 1024]);
}

TEST(array, swap) {
  array<int, 3> arr1 = {1, 2, 3};
  array<int, 3> arr2 = {3, 2, 1};
  arr1.swap(arr2);
  EXPECT_STRING_EQ(arr1, [3, 2, 1]);
  EXPECT_STRING_EQ(arr2, [1, 2, 3]);
}

TEST(array, begin) {
  array<int, 3> arr = {1, 2, 3};
  EXPECT_EQ(*arr.begin(), 1);
  *arr.begin() = 11;
  EXPECT_EQ(*arr.begin(), 11);
}

TEST(array, end) {
  array<int, 3> arr = {1, 2, 3};
  auto it = arr.end();
  EXPECT_EQ(*--it, 3);
}

TEST(array, rbegin) {
  array<int, 3> arr = {1, 2, 3};
  EXPECT_EQ(*arr.rbegin(), 3);
  *arr.rbegin() = 4;
  EXPECT_EQ(*arr.rbegin(), 4);
}

TEST(array, rend) {
  array<int, 3> arr = {1, 2, 3};
  EXPECT_EQ(*--arr.rend(), 1);
  *--arr.rend() = 11;
  EXPECT_EQ(*--arr.rend(), 11);
}

TEST(array, size) {
  array<int, 3> arr = {1, 2, 3};
  EXPECT_EQ(arr.size(), 3);
}

TEST(array, max_size) {
  array<int, 3> arr = {1, 2, 3};
  EXPECT_EQ(arr.max_size(), 3);
}

TEST(array, empty) {
  array<int, 0> arr1;
  array<int, 3> arr2 = {1, 2, 3};
  EXPECT_TRUE(arr1.empty());
  EXPECT_FALSE(arr2.empty());
}

TEST(array, at_operator) {
  array<int, 3> arr = {1, 2, 3};
  EXPECT_EQ(arr[0], 1);
  arr[1] = 22;
  EXPECT_STRING_EQ(arr, [1, 22, 3]);
}

TEST(array, at) {
  array<int, 3> arr = {1, 2, 3};
  for (int i = 0; i < arr.size(); i++) arr.at(i) = i * i;
  EXPECT_STRING_EQ(arr, [0, 1, 4]);
  EXPECT_EQ(arr.at(0), 0);
  EXPECT_EQ(arr.at(1), 1);
  EXPECT_EQ(arr.at(2), 4);
  EXPECT_THROW(arr.at(3), std::range_error);
  EXPECT_THROW((arr.at(3) = 6), std::range_error);
}

TEST(array, front) {
  array<int, 3> arr = {1, 2, 3};
  EXPECT_EQ(arr.front(), 1);
  arr.front() = 11;
  EXPECT_EQ(arr.front(), 11);
}

TEST(array, back) {
  array<int, 3> arr = {1, 2, 3};
  EXPECT_EQ(arr.back(), 3);
  arr.back() = 33;
  EXPECT_EQ(arr.back(), 33);
}

TEST(array, data) {
  array<int, 3> arr = {1, 2, 3};
  int* ptr = arr.data();
  EXPECT_EQ(ptr[0], 1);
  EXPECT_EQ(ptr[1], 2);
  EXPECT_EQ(ptr[2], 3);
}