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

TEST(deque, assign) {
  /**
   * @test  void assign(initializer_list l)
   * @brief initializer_list assign. 
   */
  SUBTEST(assign) {
    deque<int> dq {1, 2, 3, 4};
    dq.assign({1, 2, 3});
    EXPECT_STRING_EQ(dq, [1, 2, 3]);
    EXPECT_EQ(dq.size(), 3);
  }

  /**
   * @test  void assign(size_type n, const T& val)
   * @brief size with value assign. 
   */
  SUBTEST(assign) {
    deque<int> dq { 1, 2, 3, 4 };
    dq.assign(3, 100);
    EXPECT_STRING_EQ(dq, [100, 100, 100]);
    EXPECT_EQ(dq.size(), 3);
  }

  /**
   * @test  void assign(Iterator first, Iterator last)
   * @brief range assign. 
   */
  SUBTEST(assign) {
    std::vector<double> l = { 3.14, 6.28, 0.618 };
    deque<int> dq { 1, 2, 3, 4 };
    dq.assign(l.begin(), l.end());
    EXPECT_STRING_EQ(dq, [3, 6, 0]);
    EXPECT_EQ(dq.size(), 3);
  }
}

TEST(deque, assign_operator) {
  /**
   * @test deque& operator=(initializer_list l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    deque<int> dq = {1, 2, 3, 4};
    dq = {1, 2, 3};
    EXPECT_STRING_EQ(dq, [1, 2, 3]);
    EXPECT_EQ(dq.size(), 3);
  }
  
  /**
   * @test deque& operator=(const deque& x)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    deque<int> dq0 = {1, 2, 3, 4};
    deque<int> dq1 = {1, 2, 3};
    dq0 = dq1;
    EXPECT_STRING_EQ(dq0, [1, 2, 3]);
    EXPECT_EQ(dq0.size(), 3);
  }

  /**
   * @test deque& operator=(deque&& x)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    deque<int> dq = {1, 2, 3, 4};
    dq = deque<int>({1, 2, 3});
    EXPECT_STRING_EQ(dq, [1, 2, 3]);
    EXPECT_EQ(dq.size(), 3);
  }
}

TEST(deque, at) {
  deque<int> dq = {1, 2, 3};
  for (int i = 0; i < dq.size(); i++) dq.at(i) = i * i;
  EXPECT_STRING_EQ(dq, [0, 1, 4]);
  EXPECT_EQ(dq.at(0), 0);
  EXPECT_EQ(dq.at(1), 1);
  EXPECT_EQ(dq.at(2), 4);
  EXPECT_THROW(dq.at(3), std::range_error);
  EXPECT_THROW((dq.at(3) = 6), std::range_error);
}

TEST(deque, at_operator) {
  deque<int> dq = {1, 2, 3};
  EXPECT_EQ(dq[0], 1);
  dq[1] = 22;
  EXPECT_STRING_EQ(dq, [1, 22, 3]);
}