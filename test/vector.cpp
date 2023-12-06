#include <list>
#include <memory>
#include "list.h"
#include "vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace tinySTL;

TEST(vector, constructor1) {
  list<int> l;
  l.emplace_back(1);
  l.emplace_back(2);
  l.emplace_back(3);
  EXPECT_EQ(l.size(), 3);
  EXPECT_STRING_EQ(l, [1, 2, 3]);
}

TEST(vector, constructor) {
  /**
   * @test  vector()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    vector<int> vc;
    EXPECT_STRING_EQ(vc, []);
  }
  
  /**
   * @test  vector(size_type n). 
   * @brief size constructor. 
   */
  SUBTEST(constructor) {
    vector<int> vc(5);
    EXPECT_STRING_EQ(vc, [0, 0, 0, 0, 0]);
  }

  /**
   * @test  vector(size_type n, const T& val)
   * @brief size with value constructor. 
   */
  SUBTEST(constructor) {
    vector<std::string> vc(3, "Hello");
    EXPECT_STRING_EQ(vc, [Hello, Hello, Hello]);
  }

  /**
   * @test  vector(const vector& x)
   * @brief copy constructor. 
   */
  SUBTEST(constructor) {
    vector<std::string> vc1(3, "Hello");
    EXPECT_STRING_EQ(vc1, [Hello, Hello, Hello]);
    vector<std::string> vc2(vc1);
    EXPECT_STRING_EQ(vc2, [Hello, Hello, Hello]);
  }

  /**
   * @test  vector(vector&& x)
   * @brief move constructor. 
   */
  SUBTEST(constructor) {
    vector<std::string> vc1(3, "Hello");
    EXPECT_STRING_EQ(vc1, [Hello, Hello, Hello]);
    vector<std::string> vc2(tinySTL::move(vc1));
    EXPECT_STRING_EQ(vc1, []);
    EXPECT_STRING_EQ(vc2, [Hello, Hello, Hello]);
  }

  /**
   * @test  vector(initializer_list l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    vector<std::string> vc = { "Hello", "Standard", "Template", "Library" };
    EXPECT_STRING_EQ(vc, [Hello, Standard, Template, Library]);
  }

  /**
   * @test  vector(Iterator first, Iterator last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    std::list<double> l = { 3.14, 6.28, 0.618 };
    vector<int> vc(l.begin(), l.end());
    EXPECT_STRING_EQ(vc, [3, 6, 0]);
  }
}

TEST(vector, assign) {
  /**
   * @test  void assign(initializer_list l)
   * @brief initializer_list assign. 
   */
  SUBTEST(assign) {
    vector<int> vc { 1, 2, 3, 4 };
    vc.assign({1, 2, 3});
    EXPECT_STRING_EQ(vc, [1, 2, 3]);
    EXPECT_EQ(vc.capacity(), 4);
    EXPECT_EQ(vc.size(), 3);
  }

  /**
   * @test  void assign(size_type n, const T& val)
   * @brief size with value assign. 
   */
  SUBTEST(assign) {
    vector<int> vc { 1, 2, 3, 4 };
    vc.assign(3, 100);
    EXPECT_STRING_EQ(vc, [100, 100, 100]);
    EXPECT_EQ(vc.capacity(), 4);
    EXPECT_EQ(vc.size(), 3);
  }

  /**
   * @test  void assign(Iterator first, Iterator last)
   * @brief range assign. 
   */
  SUBTEST(assign) {
    std::list<double> l = { 3.14, 6.28, 0.618 };
    vector<int> vc { 1, 2, 3, 4 };
    vc.assign(l.begin(), l.end());
    EXPECT_STRING_EQ(vc, [3, 6, 0]);
    EXPECT_EQ(vc.capacity(), 4);
    EXPECT_EQ(vc.size(), 3);
  }
}

TEST(vector, assign_operator) {
  /**
   * @test vector& operator=(initializer_list l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    vector<int> vc = {1, 2, 3, 4};
    vc = {1, 2, 3};
    EXPECT_STRING_EQ(vc, [1, 2, 3]);
    EXPECT_EQ(vc.size(), 3);
    EXPECT_EQ(vc.capacity(), 4);
  }
  
  /**
   * @test vector& operator=(const vector& x)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    vector<int> vc0 = {1, 2, 3, 4};
    vector<int> vc1 = {1, 2, 3};
    vc0 = vc1;
    EXPECT_STRING_EQ(vc0, [1, 2, 3]);
    EXPECT_EQ(vc0.size(), 3);
    EXPECT_EQ(vc0.capacity(), 4);
  }

  /**
   * @test vector& operator=(vector&& x)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    vector<int> vc = {1, 2, 3, 4};
    vc = vector<int>({1, 2, 3});
    EXPECT_STRING_EQ(vc, [1, 2, 3]);
    EXPECT_EQ(vc.size(), 3);
    EXPECT_EQ(vc.capacity(), 3);
  }
}

TEST(vector, at) {
  vector<int> vc = {1, 2, 3};
  for (int i = 0; i < vc.size(); i++) vc.at(i) = i * i;
  EXPECT_STRING_EQ(vc, [0, 1, 4]);
  EXPECT_EQ(vc.at(0), 0);
  EXPECT_EQ(vc.at(1), 1);
  EXPECT_EQ(vc.at(2), 4);
  EXPECT_THROW(vc.at(3), std::range_error);
  EXPECT_THROW((vc.at(3) = 6), std::range_error);
}

TEST(vector, at_operator) {
  vector<int> vc = {1, 2, 3};
  EXPECT_EQ(vc[0], 1);
  vc[1] = 22;
  EXPECT_STRING_EQ(vc, [1, 22, 3]);
}

TEST(vector, back) {
  vector<int> vc;
  EXPECT_THROW(vc.back(), std::range_error);
  vc = {1, 2, 3};
  EXPECT_EQ(vc.back(), 3);
  vc.back() = 33;
  EXPECT_EQ(vc.back(), 33);
}

TEST(vector, begin) {
  vector<int> vc = {1, 2, 3};
  EXPECT_EQ(*vc.begin(), 1);
  *vc.begin() = 11;
  EXPECT_EQ(*vc.begin(), 11);
}

TEST(vector, capacity) {
  vector<int> vc = {1, 2, 3};
  EXPECT_EQ(vc.capacity(), 3);
  vc.push_back(11);
  EXPECT_EQ(vc.capacity(), 6);
}

TEST(vector, clear) {
  vector<int> vc = {1, 2, 3};
  vc.clear();
  vc.clear();
  EXPECT_EQ(vc.size(), 0);
  EXPECT_EQ(vc.capacity(), 3);
}

TEST(vector, data) {
  vector<int> vc = {1, 2, 3};
  int *p = vc.data();
  EXPECT_EQ(p[0], 1);
  EXPECT_EQ(p[1], 2);
  EXPECT_EQ(p[2], 3);
  p[1] = 11;
  EXPECT_EQ(p[1], 11);
}

TEST(vector, emplace) {
  vector<TestClass> vc;
  vc.emplace(vc.begin(), 2, 3);
  EXPECT_STRING_EQ(vc, [[3, 3]]);
  vc.emplace(vc.begin(), 3, 4);
  EXPECT_STRING_EQ(vc, [[4, 4, 4], [3, 3]]);
}

TEST(vector, emplace_back) {
  vector<int> vc = {1, 2, 3};
  vc.emplace_back(4);
  EXPECT_STRING_EQ(vc, [1, 2, 3, 4]);
}

TEST(vector, empty) {
  vector<int> vc = {1, 2, 3};
  EXPECT_EQ(vc.empty(), false);
  vc.clear();
  EXPECT_EQ(vc.empty(), true);
}

TEST(vector, end) {
  vector<int> vc = {1, 2, 3};
  auto it = vc.end();
  EXPECT_EQ(*--it, 3);
}

TEST(vector, erase) {
  /**
   * @test  iterator erase(const_iterator pos)
   * @brief erase an item.
   */
  SUBTEST(erase) {
    vector<int> vc = {1, 2, 3};
    EXPECT_THROW(vc.erase(vc.end()), std::range_error);
    vc.erase(vc.begin());
    EXPECT_STRING_EQ(vc, [2, 3]);
  }

  /**
   * @test  iterator erase(const_iterator first, const_iterator last)
   * @brief range erase.
   */
  SUBTEST(erase) {
    vector<int> vc = {1, 2, 3};
    vc.erase(vc.begin(), vc.begin() + 2);
    EXPECT_STRING_EQ(vc, [3]);
  }
}

TEST(vector, front) {
  vector<int> vc;
  EXPECT_THROW(vc.front(), std::range_error);
  vc = {1, 2, 3};
  EXPECT_EQ(vc.front(), 1);
  vc.front() = 11;
  EXPECT_EQ(vc.front(), 11);
}

TEST(vector, get_allocator) {
  vector<int> vc;
  vc.get_allocator();
}

TEST(vector, insert) {
  /**
   * @test  iterator insert(const_iterator pos, initializer_list l)
   * @brief insert an initializer_list.
   */
  SUBTEST(insert) {
    vector<int> vc = {1, 2, 3};
    vc.insert(vc.begin() + 1, {11, 22});
    EXPECT_STRING_EQ(vc, [1, 11, 22, 2, 3]);
    EXPECT_EQ(vc.size(), 5);
    EXPECT_EQ(vc.capacity(), 6);
  }

  /**
   * @test  iterator insert(const_iterator pos, T&& x)
   * @brief insert an rvalue.
   */
  SUBTEST(insert) {
    vector<int> vc = {1, 2, 3};
    vc.insert(vc.begin(), 11);
    EXPECT_STRING_EQ(vc, [11, 1, 2, 3]);
    EXPECT_EQ(vc.size(), 4);
    EXPECT_EQ(vc.capacity(), 6);
  }

  /**
   * @test  iterator insert(const_iterator pos, const T& x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    vector<int> vc = {1, 2, 3};
    int x = 11;
    vc.insert(vc.begin(), x);
    EXPECT_STRING_EQ(vc, [11, 1, 2, 3]);
    EXPECT_EQ(vc.size(), 4);
    EXPECT_EQ(vc.capacity(), 6);
  }

  /**
   * @test  iterator insert(const_iterator pos, size_type n, const T& x)
   * @brief insert an lvalue reference n times.
   */
  SUBTEST(insert) {
    vector<int> vc = {1, 2};
    vc.insert(vc.begin(), 3, 11);
    EXPECT_STRING_EQ(vc, [11, 11, 11, 1, 2]);
    EXPECT_EQ(vc.size(), 5);
    EXPECT_EQ(vc.capacity(), 5);
  }

  /**
   * @test  iterator insert(const_iterator pos, Iterator first, Iterator last)
   * @brief range insert.
   */
  SUBTEST(insert) {
    std::list<double> l = {11, 22, 33};
    vector<int> vc = {1, 2, 3, 4};
    vc.insert(vc.begin(), l.begin(), l.end());
    EXPECT_STRING_EQ(vc, [11, 22, 33, 1, 2, 3, 4]);
    EXPECT_EQ(vc.size(), 7);
    EXPECT_EQ(vc.capacity(), 8);
  }
}

TEST(vector, max_size) {
  vector<int> vc;
  EXPECT_GT(vc.max_size(), 0xffff);
}

TEST(vector, pop_back) {
  vector<int> vc;
  vc.pop_back();
  EXPECT_STRING_EQ(vc, []);
  EXPECT_EQ(vc.size(), 0);
  EXPECT_EQ(vc.capacity(), 0);
  vc = {1, 2, 3};
  vc.pop_back();
  EXPECT_STRING_EQ(vc, [1, 2]);
  EXPECT_EQ(vc.size(), 2);
  EXPECT_EQ(vc.capacity(), 3);
}

TEST(vector, push_back) {
  /**
   * @test  void push_back(T&& x)
   * @brief push_back an rvalue.
   */
  SUBTEST(push_back) {
    vector<int> vc;
    vc.push_back(1);
    EXPECT_STRING_EQ(vc, [1]);
    EXPECT_EQ(vc.size(), 1);
    EXPECT_EQ(vc.capacity(), 1);
    vc.push_back(2);
    vc.push_back(3);
    EXPECT_STRING_EQ(vc, [1, 2, 3]);
    EXPECT_EQ(vc.size(), 3);
    EXPECT_EQ(vc.capacity(), 4);
  }

  /**
   * @test  void push_back(const T& x)
   * @brief push_back an lvalue reference.
   */
  SUBTEST(push_back) {
    vector<int> vc;
    int arr[3] = {1, 2, 3};
    vc.push_back(arr[0]);
    EXPECT_STRING_EQ(vc, [1]);
    EXPECT_EQ(vc.size(), 1);
    EXPECT_EQ(vc.capacity(), 1);
    vc.push_back(arr[1]);
    vc.push_back(arr[2]);
    EXPECT_STRING_EQ(vc, [1, 2, 3]);
    EXPECT_EQ(vc.size(), 3);
    EXPECT_EQ(vc.capacity(), 4);
  }
}

TEST(vector, rbegin) {
  vector<int> vc = {1, 2, 3};
  EXPECT_EQ(*vc.rbegin(), 3);
  *vc.rbegin() = 4;
  EXPECT_EQ(*vc.rbegin(), 4);
}

TEST(vector, rend) {
  vector<int> vc = {1, 2, 3};
  EXPECT_EQ(*--vc.rend(), 1);
  *--vc.rend() = 11;
  EXPECT_EQ(*--vc.rend(), 11);
}

TEST(vector, reserve) {
  vector<int> vc;
  vc.reserve(11);
  EXPECT_EQ(vc.size(), 0);
  EXPECT_EQ(vc.capacity(), 11);
}

TEST(vector, resize) {
  /**
   * @test  void resize(size_type new_size)
   * @brief resize the vector.
   */
  SUBTEST(resize) {
    vector<int> vc = {1, 2, 3};
    vc.resize(5);
    EXPECT_STRING_EQ(vc, [1, 2, 3, 0, 0]);
    EXPECT_EQ(vc.size(), 5);
    EXPECT_EQ(vc.capacity(), 5);
    vc.resize(2);
    EXPECT_STRING_EQ(vc, [1, 2]);
    EXPECT_EQ(vc.size(), 2);
    EXPECT_EQ(vc.capacity(), 5);
  }

  /**
   * @test  void resize(size_type new_size, const T& x)
   * @brief resize the vector.
   */
  SUBTEST(resize) {
    vector<int> vc = {1, 2, 3};
    vc.resize(5, 10);
    EXPECT_STRING_EQ(vc, [1, 2, 3, 10, 10]);
    EXPECT_EQ(vc.size(), 5);
    EXPECT_EQ(vc.capacity(), 5);
    vc.resize(2, 20);
    EXPECT_STRING_EQ(vc, [1, 2]);
    EXPECT_EQ(vc.size(), 2);
    EXPECT_EQ(vc.capacity(), 5);
  }
}

TEST(vector, shrink_to_fit) {
  vector<int> vc(3);
  vc.push_back(11);
  EXPECT_STRING_EQ(vc, [0, 0, 0, 11]);
  EXPECT_EQ(vc.size(), 4);
  EXPECT_EQ(vc.capacity(), 6);
  vc.shrink_to_fit();
  EXPECT_STRING_EQ(vc, [0, 0, 0, 11]);
  EXPECT_EQ(vc.size(), 4);
  EXPECT_EQ(vc.capacity(), 4);
}

TEST(vector, size) {
  vector<int> vc = {1, 2, 3};
  EXPECT_EQ(vc.size(), 3);
}

TEST(vector, swap) {
  vector<int> vc0 = {1, 2, 3};
  vector<int> vc1 = {11, 22, 33, 44};
  vc0.swap(vc1);
  EXPECT_STRING_EQ(vc0, [11, 22, 33, 44]);
  EXPECT_STRING_EQ(vc1, [1, 2, 3]);
}