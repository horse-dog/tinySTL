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

TEST(deque, at_operator) {
  deque<int> dq = {1, 2, 3};
  EXPECT_EQ(dq[0], 1);
  dq[1] = 22;
  EXPECT_STRING_EQ(dq, [1, 22, 3]);
}

TEST(deque, get_allocator) {
  deque<int> dq;
  dq.get_allocator();
}

TEST(deque, begin) {
  deque<int> dq = {1, 2, 3};
  EXPECT_EQ(*dq.begin(), 1);
  *dq.begin() = 11;
  EXPECT_EQ(*dq.begin(), 11);
}

TEST(deque, end) {
  deque<int> dq = {1, 2, 3};
  auto it = dq.end();
  EXPECT_EQ(*--dq.end(), 3);
}

TEST(deque, rbegin) {
  deque<int> dq = {1, 2, 3};
  EXPECT_EQ(*dq.rbegin(), 3);
  *dq.rbegin() = 4;
  EXPECT_EQ(*dq.rbegin(), 4);
}

TEST(deque, rend) {
  deque<int> dq = {1, 2, 3};
  EXPECT_EQ(*--dq.rend(), 1);
  *--dq.rend() = 11;
  EXPECT_EQ(*--dq.rend(), 11);
}

TEST(deque, empty) {
  deque<int> dq = {1, 2, 3};
  EXPECT_EQ(dq.empty(), false);
  dq.clear();
  EXPECT_EQ(dq.empty(), true);
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

TEST(deque, size) {
  deque<int> dq = {1, 2, 3};
  EXPECT_EQ(dq.size(), 3);
}

TEST(deque, max_size) {
  deque<int> dq;
  EXPECT_GT(dq.max_size(), 0xffff);
}

TEST(deque, front) {
  deque<int> dq;
  EXPECT_THROW(dq.front(), std::range_error);
  dq = {1, 2, 3};
  EXPECT_EQ(dq.front(), 1);
  dq.front() = 11;
  EXPECT_EQ(dq.front(), 11);
}

TEST(deque, back) {
  deque<int> dq;
  EXPECT_THROW(dq.back(), std::range_error);
  dq = {1, 2, 3};
  EXPECT_EQ(dq.back(), 3);
  dq.back() = 33;
  EXPECT_EQ(dq.back(), 33);
}

TEST(deque, swap) {
  deque<int> dq0 = {1, 2, 3};
  deque<int> dq1 = {11, 22, 33, 44};
  dq0.swap(dq1);
  EXPECT_STRING_EQ(dq0, [11, 22, 33, 44]);
  EXPECT_STRING_EQ(dq1, [1, 2, 3]);
}

TEST(deque, insert) {
  /**
   * @test  iterator insert(const_iterator pos, initializer_list l)
   * @brief insert an initializer_list.
   */
  SUBTEST(insert) {
    deque<int> dq = {1, 2, 3};
    dq.insert(dq.begin() + 1, {11, 22});
    EXPECT_STRING_EQ(dq, [1, 11, 22, 2, 3]);
    EXPECT_EQ(dq.size(), 5);
  }

  /**
   * @test  iterator insert(const_iterator pos, T&& x)
   * @brief insert an rvalue.
   */
  SUBTEST(insert) {
    deque<int> dq = {1, 2, 3};
    dq.insert(dq.begin(), 11);
    EXPECT_STRING_EQ(dq, [11, 1, 2, 3]);
    EXPECT_EQ(dq.size(), 4);
  }

  /**
   * @test  iterator insert(const_iterator pos, const T& x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    deque<int> dq = {1, 2, 3};
    int x = 11;
    dq.insert(dq.begin(), x);
    EXPECT_STRING_EQ(dq, [11, 1, 2, 3]);
    EXPECT_EQ(dq.size(), 4);
  }

  /**
   * @test  iterator insert(const_iterator pos, size_type n, const T& x)
   * @brief insert an lvalue reference n times.
   */
  SUBTEST(insert) {
    deque<int> dq = {1, 2};
    dq.insert(dq.begin(), 3, 11);
    EXPECT_STRING_EQ(dq, [11, 11, 11, 1, 2]);
    EXPECT_EQ(dq.size(), 5);
  }

  /**
   * @test  iterator insert(const_iterator pos, Iterator first, Iterator last)
   * @brief range insert.
   */
  SUBTEST(insert) {
    std::vector<double> l = {11, 22, 33};
    deque<int> dq = {1, 2, 3, 4};
    dq.insert(dq.begin(), l.begin(), l.end());
    EXPECT_STRING_EQ(dq, [11, 22, 33, 1, 2, 3, 4]);
    EXPECT_EQ(dq.size(), 7);
  }
}

TEST(deque, erase) {
  /**
   * @test  iterator erase(const_iterator pos)
   * @brief erase an item.
   */
  SUBTEST(erase) {
    deque<int> dq = {1, 2, 3};
    EXPECT_THROW(dq.erase(dq.end()), std::range_error);
    dq.erase(dq.begin());
    EXPECT_STRING_EQ(dq, [2, 3]);
  }

  /**
   * @test  iterator erase(const_iterator first, const_iterator last)
   * @brief range erase.
   */
  SUBTEST(erase) {
    deque<int> dq = {1, 2, 3};
    dq.erase(dq.begin(), dq.begin() + 2);
    EXPECT_STRING_EQ(dq, [3]);
  }
}

TEST(deque, push_front) {
  /**
   * @test  void push_front(T&& x)
   * @brief push element at front.
   */
  SUBTEST(push_front) {
    deque<int> dq = {1, 2, 3, 4};
    dq.push_front(0);
    EXPECT_STRING_EQ(dq, [0, 1, 2, 3, 4]);
    EXPECT_EQ(dq.size(), 5);
  }

  /**
   * @test  void push_front(const T& x)
   * @brief push element at front.
   */
  SUBTEST(push_front) {
    deque<int> dq = {1, 2, 3, 4};
    int x = 0;
    dq.push_front(x);
    EXPECT_STRING_EQ(dq, [0, 1, 2, 3, 4]);
    EXPECT_EQ(dq.size(), 5);
  }
}

TEST(deque, push_back) {
  /**
   * @test  void push_back(T&& x)
   * @brief push_back an rvalue.
   */
  SUBTEST(push_back) {
    deque<int> dq;
    dq.push_back(1);
    EXPECT_STRING_EQ(dq, [1]);
    EXPECT_EQ(dq.size(), 1);
    dq.push_back(2);
    dq.push_back(3);
    EXPECT_STRING_EQ(dq, [1, 2, 3]);
    EXPECT_EQ(dq.size(), 3);
  }

  /**
   * @test  void push_back(const T& x)
   * @brief push_back an lvalue reference.
   */
  SUBTEST(push_back) {
    deque<int> dq;
    int arr[3] = {1, 2, 3};
    dq.push_back(arr[0]);
    EXPECT_STRING_EQ(dq, [1]);
    EXPECT_EQ(dq.size(), 1);
    dq.push_back(arr[1]);
    dq.push_back(arr[2]);
    EXPECT_STRING_EQ(dq, [1, 2, 3]);
    EXPECT_EQ(dq.size(), 3);
  }
}

TEST(deque, clear) {
  deque<int> dq = {1, 2, 3};
  dq.clear();
  dq.clear();
  EXPECT_EQ(dq.size(), 0);
}

TEST(deque, resize) {
  /**
   * @test  void resize(size_type new_size)
   * @brief resize the deque.
   */
  SUBTEST(resize) {
    deque<int> dq = {1, 2, 3};
    dq.resize(5);
    EXPECT_STRING_EQ(dq, [1, 2, 3, 0, 0]);
    EXPECT_EQ(dq.size(), 5);
    dq.resize(2);
    EXPECT_STRING_EQ(dq, [1, 2]);
    EXPECT_EQ(dq.size(), 2);
  }

  /**
   * @test  void resize(size_type new_size, const T& x)
   * @brief resize the deque.
   */
  SUBTEST(resize) {
    deque<int> dq = {1, 2, 3};
    dq.resize(5, 10);
    EXPECT_STRING_EQ(dq, [1, 2, 3, 10, 10]);
    EXPECT_EQ(dq.size(), 5);
    dq.resize(2, 20);
    EXPECT_STRING_EQ(dq, [1, 2]);
    EXPECT_EQ(dq.size(), 2);
  }
}

TEST(deque, pop_front) {
  deque<int> dq;
  dq.pop_front();
  dq = {1, 2, 3, 4};
  dq.pop_front();
  EXPECT_STRING_EQ(dq, [2, 3, 4]);
  EXPECT_EQ(dq.size(), 3);
}

TEST(deque, pop_back) {
  deque<int> dq;
  dq.pop_back();
  EXPECT_STRING_EQ(dq, []);
  EXPECT_EQ(dq.size(), 0);
  dq = {1, 2, 3};
  dq.pop_back();
  EXPECT_STRING_EQ(dq, [1, 2]);
  EXPECT_EQ(dq.size(), 2);
}

TEST(deque, emplace) {
  deque<TestClass> dq;
  dq.emplace(dq.begin(), 2, 3);
  EXPECT_STRING_EQ(dq, [[3, 3]]);
  dq.emplace(dq.begin(), 3, 4);
  EXPECT_STRING_EQ(dq, [[4, 4, 4], [3, 3]]);
}

TEST(deque, emplace_front) {
  deque<int> dq;
  dq.emplace_front(1);
  EXPECT_STRING_EQ(dq, [1]);
  dq.emplace_front(11);
  EXPECT_STRING_EQ(dq, [11, 1]);
}

TEST(deque, emplace_back) {
  deque<int> dq = {1, 2, 3};
  dq.emplace_back(4);
  EXPECT_STRING_EQ(dq, [1, 2, 3, 4]);
}