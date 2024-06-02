#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "set.h"
#include "list.h"

using namespace tinySTL;

TEST(multiset, constructor) {
  /**
   * @test  multiset()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    multiset<int> s;
    EXPECT_STRING_EQ(s, []);
  }

  /**
   * @test  multiset(const multiset&)
   * @brief copy constructor.
   */
  SUBTEST(constructor) {
    multiset<int> s1{4, 3, 4, 7, 1};
    EXPECT_STRING_EQ(s1, [1, 3, 4, 4, 7]);
    multiset<int> s2(s1);
    EXPECT_STRING_EQ(s2, [1, 3, 4, 4, 7]);
  }

  /**
   * @test  multiset(multiset&&)
   * @brief move constructor.
   */
  SUBTEST(constructor) {
    multiset<int> s1{4, 3, 4, 7, 1};
    EXPECT_STRING_EQ(s1, [1, 3, 4, 4, 7]);
    multiset<int> s2(tinySTL::move(s1));
    EXPECT_EQ(s1.size(), 0);
    EXPECT_EQ(s2.size(), 5);
    EXPECT_STRING_EQ(s1, []);
    EXPECT_STRING_EQ(s2, [1, 3, 4, 4, 7]);
  }

  /**
   * @test  multiset(initializer_list l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    multiset<std::string> s = { "Hello", "Standard", "Hello", "Template", "Library" };
    EXPECT_STRING_EQ(s, [Hello, Hello, Library, Standard, Template]);
  }
  
  /**
   * @test  multiset(Iterator first, Iterator last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    list<double> l = { 3.14, 6.28, 0.25, 0.618 };
    multiset<int> s(l.begin(), l.end());
    EXPECT_STRING_EQ(s, [0, 0, 3, 6]);
  }
}

#include <queue>
std::priority_queue<int> pq;

TEST(multiset, assign_operator) {

}

TEST(multiset, key_compare) {

}

TEST(multiset, value_compare) {

}

TEST(multiset, get_allocator) {

}

TEST(multiset, begin) {

}

TEST(multiset, end) {

}

TEST(multiset, rbegin) {

}

TEST(multiset, rend) {

}

TEST(multiset, empty) {

}

TEST(multiset, size) {

}

TEST(multiset, max_size) {

}

TEST(multiset, swap) {

}

TEST(multiset, emplace) {

}

TEST(multiset, emplace_hint) {

}

TEST(multiset, insert) {

}

TEST(multiset, erase) {

}

TEST(multiset, clear) {

}

TEST(multiset, count) {

}

TEST(multiset, contains) {

}

TEST(multiset, find) {

}

TEST(multiset, lower_bound) {

}

TEST(multiset, upper_bound) {

}

TEST(multiset, equal_range) {

}

TEST(multiset, disp) {

}

TEST(multiset, merge) {
  
}

TEST(multiset, compare_operator) {

}

TEST(multiset, cout_operator) {

}