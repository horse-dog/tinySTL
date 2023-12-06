#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "list.h"

using namespace tinySTL;

TEST(list, constructor) {
  /**
   * @test  list()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    list<int> li;
    EXPECT_STRING_EQ(li, []);
  }

  /**
   * @test  list(size_type n). 
   * @brief size constructor. 
   */
  SUBTEST(constructor) {
    list<int> li(5);
    EXPECT_STRING_EQ(li, [0, 0, 0, 0, 0]);
  }

  /**
   * @test  list(size_type n, const T& val)
   * @brief size with value constructor. 
   */
  SUBTEST(constructor) {
    list<std::string> li(3, "Hello");
    EXPECT_STRING_EQ(li, [Hello, Hello, Hello]);
  }

  /**
   * @test  list(const list& x)
   * @brief copy constructor. 
   */
  SUBTEST(constructor) {
    list<std::string> li1(3, "Hello");
    EXPECT_STRING_EQ(li1, [Hello, Hello, Hello]);
    list<std::string> li2(li1);
    EXPECT_STRING_EQ(li2, [Hello, Hello, Hello]);
  }

  /**
   * @test  list(list&& x)
   * @brief move constructor. 
   */
  SUBTEST(constructor) {
    list<std::string> li1(3, "Hello");
    EXPECT_STRING_EQ(li1, [Hello, Hello, Hello]);
    list<std::string> li2(tinySTL::move(li1));
    EXPECT_STRING_EQ(li1, []);
    EXPECT_STRING_EQ(li2, [Hello, Hello, Hello]);
  }

  /**
   * @test  list(initializer_list l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    list<std::string> li = { "Hello", "Standard", "Template", "Library" };
    EXPECT_STRING_EQ(li, [Hello, Standard, Template, Library]);
  }

  /**
   * @test  list(Iterator first, Iterator last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    std::vector<double> vc = { 3.14, 6.28, 0.618 };
    list<int> li(vc.begin(), vc.end());
    EXPECT_STRING_EQ(li, [3, 6, 0]);
  }
}

TEST(list, assign) {
  /**
   * @test  void assign(initializer_list l)
   * @brief initializer_list assign. 
   */
  SUBTEST(assign) {
    list<int> li { 1, 2, 3, 4 };
    li.assign({1, 2, 3});
    EXPECT_STRING_EQ(li, [1, 2, 3]);
    EXPECT_EQ(li.size(), 3);
  }

  /**
   * @test  void assign(size_type n, const T& val)
   * @brief size with value assign. 
   */
  SUBTEST(assign) {
    list<int> li { 1, 2, 3, 4 };
    li.assign(3, 100);
    EXPECT_STRING_EQ(li, [100, 100, 100]);
    EXPECT_EQ(li.size(), 3);
  }

  /**
   * @test  void assign(Iterator first, Iterator last)
   * @brief range assign. 
   */
  SUBTEST(assign) {
    std::vector<double> vc = { 3.14, 6.28, 0.618 };
    list<int> li { 1, 2, 3, 4 };
    li.assign(vc.begin(), vc.end());
    EXPECT_STRING_EQ(li, [3, 6, 0]);
    EXPECT_EQ(li.size(), 3);
  }
}