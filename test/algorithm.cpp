#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "algorithm.h"
#include "list.h"
#include "deque.h"
#include "vector.h"

#include <random>

using namespace tinySTL;

TEST(algorithm, adjacent_find) {
  vector<int> vc {0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8};
  list<int> li(vc.begin(), vc.end());
  EXPECT_EQ(*adjacent_find(vc.begin(), vc.end()), 6);
  EXPECT_EQ(*adjacent_find(li.begin(), li.end()), 6);
}

TEST(algorithm, count) {
  vector<int> vc {0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8};
  list<int> li(vc.begin(), vc.end());
  EXPECT_EQ(count(vc.begin(), vc.end(), 6), 3);
  EXPECT_EQ(count(li.begin(), li.end(), 6), 3);
}

TEST(algorithm, count_if) {
  vector<int> vc {0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8};
  list<int> li(vc.begin(), vc.end());
  EXPECT_EQ(count_if(vc.begin(), vc.end(), [](const int& x) { return x < 7; }), 9);
  EXPECT_EQ(count_if(li.begin(), li.end(), [](const int& x) { return x < 7; }), 9);
}

TEST(algorithm, lower_bound) {
  vector<int> vc {0, 1, 2, 3, 5, 6, 6, 6, 7, 8};
  list<int> li(vc.begin(), vc.end());
  EXPECT_EQ(*lower_bound(vc.begin(), vc.end(), 4), 5);
  EXPECT_EQ(*lower_bound(vc.begin(), vc.end(), 6), 6);
  EXPECT_EQ(lower_bound(vc.begin(), vc.end(), 10), vc.end());
  EXPECT_EQ(*lower_bound(li.begin(), li.end(), 4), 5);
  EXPECT_EQ(*lower_bound(li.begin(), li.end(), 6), 6);
  EXPECT_EQ(lower_bound(li.begin(), li.end(), 10), li.end());
}

TEST(algorithm, upper_bound) {
  vector<int> vc {0, 1, 2, 3, 5, 6, 6, 6, 7, 8};
  list<int> li(vc.begin(), vc.end());
  EXPECT_EQ(*upper_bound(vc.begin(), vc.end(), 4), 5);
  EXPECT_EQ(*upper_bound(vc.begin(), vc.end(), 6), 7);
  EXPECT_EQ(upper_bound(vc.begin(), vc.end(), 10), vc.end());
  EXPECT_EQ(*upper_bound(li.begin(), li.end(), 4), 5);
  EXPECT_EQ(*upper_bound(li.begin(), li.end(), 6), 7);
  EXPECT_EQ(upper_bound(li.begin(), li.end(), 10), li.end());
}

TEST(algorithm, binary_search) {
  vector<int> vc {0, 1, 2, 3, 5, 6, 6, 6, 7, 8};
  list<int> li(vc.begin(), vc.end());
  EXPECT_TRUE(binary_search(vc.begin(), vc.end(), 3));
  EXPECT_TRUE(binary_search(vc.begin(), vc.end(), 6));
  EXPECT_FALSE(binary_search(vc.begin(), vc.end(), 4));
  EXPECT_FALSE(binary_search(vc.begin(), vc.end(), 40));

  EXPECT_TRUE(binary_search(li.begin(), li.end(), 3));
  EXPECT_TRUE(binary_search(li.begin(), li.end(), 6));
  EXPECT_FALSE(binary_search(li.begin(), li.end(), 4));
  EXPECT_FALSE(binary_search(li.begin(), li.end(), 40));
}

TEST(algorithm, equal_range) {
  vector<int> vc { 0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8 };
  pair<vector<int>::iterator, vector<int>::iterator> pairIte;  
	pairIte = equal_range(vc.begin(), vc.end(), 6);
	EXPECT_EQ(*pairIte.first, 6);
	EXPECT_EQ(*pairIte.second, 7);
}

TEST(algorithm, find) {
  vector<int> vc { 0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8 };
  EXPECT_EQ(*find(vc.begin(), vc.end(), 4), 4);
  EXPECT_EQ(find(vc.begin(), vc.end(), 44), vc.end());
}

TEST(algorithm, find_if) {
  vector<int> vc { 0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8 };
  EXPECT_EQ(*find_if(vc.begin(), vc.end(), [](const int& x) {return x == 4; }), 4);
  EXPECT_EQ(find_if(vc.begin(), vc.end(), [](const int& x) {return x == 44; }), vc.end());
}

TEST(algorithm, search) {
  vector<int> vc {0, 1, 2, 3, 5, 6, 6, 6, 7, 8};
  list<int> li {5, 6, 6};
  deque<int> dq1 {5, 6, 6}, dq2 {11, 12, 13};
  EXPECT_EQ(*search(vc.begin(), vc.end(), li.begin(), li.end()), 5);
  EXPECT_EQ(*search(vc.begin(), vc.end(), dq1.begin(), dq1.end()), 5);
  EXPECT_EQ(search(li.begin(), li.end(), dq1.begin(), dq1.end()), li.begin());
  EXPECT_EQ(search(dq1.begin(), dq1.end(), dq2.begin(), dq2.end()), dq1.end());
}

TEST(algorithm, search_n) {
  vector<int> vc {0, 1, 2, 3, 5, 6, 6, 6, 7, 8};
  list<int> li {5, 6, 6};
  deque<int> dq {5, 6, 6};
  EXPECT_EQ(*search_n(vc.begin(), vc.end(), 3, 6), 6);
  EXPECT_EQ(search_n(li.begin(), li.end(), 3, 6), li.end());
  EXPECT_EQ(*search_n(li.begin(), li.end(), 2, 6), 6);
  EXPECT_EQ(search_n(dq.begin(), dq.end(), 3, 6), dq.end());
  EXPECT_EQ(*search_n(dq.begin(), dq.end(), 2, 6), 6);
}

TEST(algorithm, merge) {
  vector<int> vc {1, 1, 2, 3, 3};
  list<int> li {2, 4, 4, 5};
  vector<int> result(vc.size() + li.size());
  merge(vc.begin(), vc.end(), li.begin(), li.end(), result.begin());
  EXPECT_STRING_EQ(result, [1, 1, 2, 2, 3, 3, 4, 4, 5]);
}

TEST(algorithm, iter_swap) {
  list<int> li {2, 4, 4, 5};
  tinySTL::iter_swap(li.begin(), --li.end());
  EXPECT_STRING_EQ(li, [5, 4, 4, 2]);
}

TEST(algorithm, reverse) {
  vector<int> vc {0, 1, 2, 3};
  list<int> li {5, 6, 6};
  deque<int> dq {5, 6, 6};
  reverse(vc.begin(), vc.end());
  reverse(li.begin(), li.end());
  reverse(dq.begin(), dq.end());
  EXPECT_STRING_EQ(vc, [3, 2, 1, 0]);
  EXPECT_STRING_EQ(li, [6, 6, 5]);
  EXPECT_STRING_EQ(dq, [6, 6, 5]);
}

TEST(algorithm, sort) {
  static std::random_device rd;
  static std::mt19937 generator(rd());
  static std::uniform_int_distribution<int> distribution(-999, 999);
  tinySTL::vector<int> vc;
  #define __INNER_TEST(n) \
  vc.resize(n); \
  tinySTL::generate(vc.begin(), vc.end(), [] { \
    return distribution(generator); \
  }); \
  tinySTL::sort(vc.begin(), vc.end()); \
  for (int i = 1; i < vc.size(); i++) { \
    if (vc[i-1] <= vc[i]) continue; \
  } \
  if (n > 0 && n <= 10) { \
    printf("[%d", vc[0]); \
    for (int i = 1; i < vc.size(); i++) { \
      printf(", %d", vc[i]); \
    } \
    printf("]\n"); \
  } \

  __INNER_TEST(0);
  __INNER_TEST(1);
  __INNER_TEST(2);
  __INNER_TEST(3);
  __INNER_TEST(4);
  __INNER_TEST(5);
  __INNER_TEST(6);
  __INNER_TEST(10);
  __INNER_TEST(100);
  __INNER_TEST(10'000);

  #undef __INNER_TEST
}

TEST(algorithm, copy) {
  list<int> li {1, 4, 2, 5, 0};
  vector<int> vc(li.size());
  tinySTL::copy(li.begin(), li.end(), vc.begin());
  EXPECT_STRING_EQ(vc, [1, 4, 2, 5, 0]);
}

TEST(algorithm, copy_backward) {
  list<int> li {1, 4, 2, 5, 0};
  int tmp = *--li.end();
  tinySTL::copy_backward(li.begin(), --li.end(), li.end());
  *li.begin() = tmp;
  EXPECT_STRING_EQ(li, [0, 1, 4, 2, 5]);
}

TEST(algorithm, remove) {
  vector<int> vc {1, 2, 2, 3, 5, 2};
  tinySTL::remove(vc.begin(), vc.end(), 2);
  EXPECT_STRING_EQ(vc, [1, 3, 5, 3, 5, 2]);
}

TEST(algorithm, remove_copy) {
  vector<int> vc {1, 2, 2, 3, 5, 2};
  vector<int> result(3);
  tinySTL::remove_copy(vc.begin(), vc.end(), result.begin(), 2);
  EXPECT_STRING_EQ(result, [1, 3, 5]);
}

TEST(algorithm, remove_if) {
  vector<int> vc {1, 2, 2, 3, 5, 2};
  tinySTL::remove_if(vc.begin(), vc.end(), [](const int& x) { return x == 2; });
  EXPECT_STRING_EQ(vc, [1, 3, 5, 3, 5, 2]);
}

TEST(algorithm, remove_copy_if) {
  vector<int> vc {1, 2, 2, 3, 5, 2};
  vector<int> result(3);
  tinySTL::remove_copy_if(vc.begin(), vc.end(), result.begin(), [](const int& x) { return x == 2; });
  EXPECT_STRING_EQ(result, [1, 3, 5]);
}

TEST(algorithm, replace) {
  vector<int> vc {1, 2, 2, 3, 5, 2};
  tinySTL::replace(vc.begin(), vc.end(), 2, 0);
  EXPECT_STRING_EQ(vc, [1, 0, 0, 3, 5, 0]);
}

TEST(algorithm, replace_copy) {
  vector<int> vc {1, 2, 2, 3, 5, 2};
  vector<int> result(6);
  tinySTL::replace_copy(vc.begin(), vc.end(), result.begin(), 2, 0);
  EXPECT_STRING_EQ(result, [1, 0, 0, 3, 5, 0]);
}

TEST(algorithm, replace_if) {
  vector<int> vc {1, 2, 2, 3, 5, 2};
  tinySTL::replace_if(vc.begin(), vc.end(), [](const int& x) { return x == 2; }, 0);
  EXPECT_STRING_EQ(vc, [1, 0, 0, 3, 5, 0]);
}

TEST(algorithm, replace_copy_if) {
  vector<int> vc {1, 2, 2, 3, 5, 2};
  vector<int> result(6);
  tinySTL::replace_copy_if(vc.begin(), vc.end(), result.begin(), [](const int& x) { return x == 2; }, 0);
  EXPECT_STRING_EQ(result, [1, 0, 0, 3, 5, 0]);
}

TEST(algorithm, swap) {
  int a = 1, b = 2;
  tinySTL::swap(a, b);
  EXPECT_EQ(a, 2);
  EXPECT_EQ(b, 1);
}

TEST(algorithm, unique) {
  vector<int> vc {1, 2, 2, 4, 5, 5, 6};
  tinySTL::unique(vc.begin(), vc.end());
  EXPECT_STRING_EQ(vc, [1, 2, 4, 5, 6, 5, 6]);
}

TEST(algorithm, unique_copy) {
  vector<int> vc {1, 2, 2, 4, 5, 5, 6};
  vector<int> result(5);
  tinySTL::unique_copy(vc.begin(), vc.end(), result.begin());
  EXPECT_STRING_EQ(result, [1, 2, 4, 5, 6]);
}

TEST(algorithm, fill) {
  vector<int> vc {1, 2, 2};
  tinySTL::fill(vc.begin(), vc.end(), 3);
  EXPECT_STRING_EQ(vc, [3, 3, 3]);
}

TEST(algorithm, fill_n) {
  vector<int> vc {1, 2, 2, 4, 4};
  tinySTL::fill_n(vc.begin(), 3, 3);
  EXPECT_STRING_EQ(vc, [3, 3, 3, 4, 4]);
}

TEST(algorithm, for_each) {
  vector<int> vc {1, 2, 2, 4, 4};
  tinySTL::for_each(vc.begin(), vc.end(), [](int& x) { x = x * x; });
  EXPECT_STRING_EQ(vc, [1, 4, 4, 16, 16]);
}

TEST(algorithm, generate) {
  vector<int> vc {1, 2, 2, 4, 4};
  tinySTL::generate(vc.begin(), vc.end(), [] { static int i = 0; return ++i; });
  EXPECT_STRING_EQ(vc, [1, 2, 3, 4, 5]);
}

TEST(algorithm, generate_n) {
  vector<int> vc {1, 2, 2, 4, 4};
  tinySTL::generate_n(vc.begin(), 5, [] { static int i = 0; return ++i; });
  EXPECT_STRING_EQ(vc, [1, 2, 3, 4, 5]);
}

TEST(algorithm, transform) {
  vector<int> vc {1, 2, 2, 4, 4};
  list<int> li {1, 2, 2, 4, 2};
  tinySTL::transform(vc.begin(), vc.end(), vc.begin(), [](const int& x) { return x * 10; });
  tinySTL::transform(vc.begin(), vc.end(), li.begin(), li.begin(), [](const int& x, const int& y) {
    return x * y;
  });
  EXPECT_STRING_EQ(vc, [10, 20, 20, 40, 40]);
  EXPECT_STRING_EQ(li, [10, 40, 40, 160, 80]);
}

TEST(algorithm, equal) {
  vector<int> vc1 {1, 4, 2, 5, 0};
  vector<int> vc0(vc1.begin(), vc1.begin()+3);
  EXPECT_TRUE(tinySTL::equal(vc0.begin(), vc0.end(), vc1.begin()));
}

TEST(algorithm, includes) {
  vector<int> vc1 {1, 4, 2, 5, 0};
  vector<int> vc0(vc1.begin(), vc1.begin()+3);
  EXPECT_TRUE(tinySTL::includes(vc1.begin(), vc1.end(), vc0.begin(), vc0.end()));
}

TEST(algorithm, max) {
  EXPECT_EQ(tinySTL::max(2, 3), 3);
}

TEST(algorithm, min) {
  EXPECT_EQ(tinySTL::min(2, 3), 2);
}

TEST(algorithm, max_element) {
  vector<int> vc1 {1, 4, 2, 5, 0};
  EXPECT_EQ(*tinySTL::max_element(vc1.begin(), vc1.end()), 5);
}

TEST(algorithm, min_element) {
  vector<int> vc1 {1, 4, 2, 5, 0};
  EXPECT_EQ(*tinySTL::min_element(vc1.begin(), vc1.end()), 0);
}

TEST(algorithm, set_union) {
  vector<int> vc1 {1, 2, 4};
  vector<int> vc2 {0, 1, 4, 5};
  vector<int> result(5);
  auto it = tinySTL::set_union(vc1.begin(), vc1.end(), vc2.begin(), vc2.end(), result.begin());
  EXPECT_STRING_EQ(result, [0, 1, 2, 4, 5]);
  EXPECT_EQ(it - result.begin(), 5);
}

TEST(algorithm, set_intersection) {
  vector<int> vc1 {1, 2, 4};
  vector<int> vc2 {0, 1, 4, 5};
  vector<int> result(2);
  auto it = tinySTL::set_intersection(vc1.begin(), vc1.end(), vc2.begin(), vc2.end(), result.begin());
  EXPECT_STRING_EQ(result, [1, 4]);
  EXPECT_EQ(it - result.begin(), 2);
}

TEST(algorithm, set_difference) {
  vector<int> vc1 {1, 2, 8};
  vector<int> vc2 {0, 1, 4, 5};
  vector<int> result(2);
  auto it = tinySTL::set_difference(vc1.begin(), vc1.end(), vc2.begin(), vc2.end(), result.begin());
  EXPECT_STRING_EQ(result, [2, 8]);
  EXPECT_EQ(it - result.begin(), 2);
}

TEST(algorithm, set_symmetric_difference) {
  vector<int> vc1 {1, 2, 4};
  vector<int> vc2 {0, 1, 4, 5};
  vector<int> result(3);
  auto it = tinySTL::set_symmetric_difference(vc1.begin(), vc1.end(), vc2.begin(), vc2.end(), result.begin());
  EXPECT_STRING_EQ(result, [0, 2, 5]);
  EXPECT_EQ(it - result.begin(), 3);
}
