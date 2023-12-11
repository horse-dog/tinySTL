#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "deque.h"

using namespace tinySTL;

TEST(deque, constructor) {
  /**
   * @test  deque()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    deque<int> dq;
    EXPECT_STRING_EQ(dq, []);
  }
  
  /**
   * @test  deque(size_type n). 
   * @brief size constructor. 
   */
  SUBTEST(constructor) {
    deque<int> dq(5);
    EXPECT_STRING_EQ(dq, [0, 0, 0, 0, 0]);
  }

  /**
   * @test  deque(size_type n, const T& val)
   * @brief size with value constructor. 
   */
  SUBTEST(constructor) {
    deque<std::string> dq(3, "Hello");
    EXPECT_STRING_EQ(dq, [Hello, Hello, Hello]);
  }

  /**
   * @test  deque(const deque& x)
   * @brief copy constructor. 
   */
  SUBTEST(constructor) {
    deque<std::string> dq1(3, "Hello");
    EXPECT_STRING_EQ(dq1, [Hello, Hello, Hello]);
    deque<std::string> dq2(dq1);
    EXPECT_STRING_EQ(dq2, [Hello, Hello, Hello]);
  }

  /**
   * @test  deque(deque&& x)
   * @brief move constructor. 
   */
  SUBTEST(constructor) {
    deque<std::string> dq1(3, "Hello");
    EXPECT_STRING_EQ(dq1, [Hello, Hello, Hello]);
    deque<std::string> dq2(tinySTL::move(dq1));
    EXPECT_STRING_EQ(dq1, []);
    EXPECT_STRING_EQ(dq2, [Hello, Hello, Hello]);
  }

  /**
   * @test  deque(initializer_list l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    deque<std::string> dq = { "Hello", "Standard", "Template", "Library" };
    EXPECT_STRING_EQ(dq, [Hello, Standard, Template, Library]);
  }

  /**
   * @test  deque(Iterator first, Iterator last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    std::vector<double> l = { 3.14, 6.28, 0.618 };
    deque<int> dq(l.begin(), l.end());
    EXPECT_STRING_EQ(dq, [3, 6, 0]);
  }
}
#include <deque>