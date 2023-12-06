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

TEST(list, assign_operator) {
  /**
   * @test list& operator=(initializer_list l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    list<int> li = {1, 2, 3, 4};
    li = {1, 2, 3};
    EXPECT_STRING_EQ(li, [1, 2, 3]);
    EXPECT_EQ(li.size(), 3);
  }
  
  /**
   * @test list& operator=(const list& x)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    list<int> li0 = {1, 2, 3, 4};
    list<int> li1 = {1, 2, 3};
    li0 = li1;
    EXPECT_STRING_EQ(li0, [1, 2, 3]);
    EXPECT_EQ(li0.size(), 3);
  }

  /**
   * @test list& operator=(list&& x)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    list<int> li = {1, 2, 3, 4};
    li = list<int>({1, 2, 3});
    EXPECT_STRING_EQ(li, [1, 2, 3]);
    EXPECT_EQ(li.size(), 3);
  }
}

TEST(list, back) {
  list<int> li;
  EXPECT_THROW(li.back(), std::range_error);
  li = {1, 2, 3};
  EXPECT_EQ(li.back(), 3);
  li.back() = 33;
  EXPECT_EQ(li.back(), 33);
}

TEST(list, begin) {
  list<int> li = {1, 2, 3};
  EXPECT_EQ(*li.begin(), 1);
  *li.begin() = 11;
  EXPECT_EQ(*li.begin(), 11);
}

TEST(list, clear) {
  list<int> li = {1, 2, 3};
  li.clear();
  li.clear();
  EXPECT_EQ(li.size(), 0);
}

TEST(list, emplace) {
  list<TestClass> li;
  li.emplace(li.begin(), 2, 3);
  EXPECT_STRING_EQ(li, [[3, 3]]);
  li.emplace(li.begin(), 3, 4);
  EXPECT_STRING_EQ(li, [[4, 4, 4], [3, 3]]);
}

TEST(list, emplace_back) {
  list<int> li = {1, 2, 3};
  li.emplace_back(4);
  EXPECT_STRING_EQ(li, [1, 2, 3, 4]);
}

TEST(list, empty) {
  list<int> li = {1, 2, 3};
  EXPECT_EQ(li.empty(), false);
  li.clear();
  EXPECT_EQ(li.empty(), true);
}

TEST(list, end) {
  list<int> li = {1, 2, 3};
  auto it = li.end();
  EXPECT_EQ(*--it, 3);
}

TEST(list, erase) {
  /**
   * @test  iterator erase(const_iterator pos)
   * @brief erase an item.
   */
  SUBTEST(erase) {
    list<int> li = {1, 2, 3};
    auto it = li.erase(li.begin());
    EXPECT_STRING_EQ(li, [2, 3]);
    EXPECT_EQ(*it, 2);
  }

  /**
   * @test  iterator erase(const_iterator first, const_iterator last)
   * @brief range erase.
   */
  SUBTEST(erase) {
    list<int> li = {1, 2, 3};
    li.erase(++li.begin(), li.end());
    EXPECT_STRING_EQ(li, [1]);
  }
}