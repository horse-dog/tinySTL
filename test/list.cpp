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
    list<int> li1 = {1, 2, 3, 4};
    list<int> li2 = {1, 2, 3};
    li1 = li2;
    EXPECT_STRING_EQ(li1, [1, 2, 3]);
    EXPECT_EQ(li1.size(), 3);
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

TEST(list, front) {
  list<int> li;
  EXPECT_THROW(li.front(), std::range_error);
  li = {1, 2, 3};
  EXPECT_EQ(li.front(), 1);
  li.front() = 11;
  EXPECT_EQ(li.front(), 11);
}

TEST(list, get_allocator) {
  list<int> li;
  li.get_allocator();
}

TEST(list, insert) {
  /**
   * @test  iterator insert(const_iterator pos, initializer_list l)
   * @brief insert an initializer_list.
   */
  SUBTEST(insert) {
    list<int> li = {1, 2, 3};
    li.insert(++li.begin(), {11, 22});
    EXPECT_STRING_EQ(li, [1, 11, 22, 2, 3]);
    EXPECT_EQ(li.size(), 5);
  }

  /**
   * @test  iterator insert(const_iterator pos, T&& x)
   * @brief insert an rvalue.
   */
  SUBTEST(insert) {
    list<int> li = {1, 2, 3};
    li.insert(li.begin(), 11);
    EXPECT_STRING_EQ(li, [11, 1, 2, 3]);
    EXPECT_EQ(li.size(), 4);
  }

  /**
   * @test  iterator insert(const_iterator pos, const T& x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    list<int> li = {1, 2, 3};
    int x = 11;
    li.insert(li.begin(), x);
    EXPECT_STRING_EQ(li, [11, 1, 2, 3]);
    EXPECT_EQ(li.size(), 4);
  }

  /**
   * @test  iterator insert(const_iterator pos, size_type n, const T& x)
   * @brief insert an lvalue reference n times.
   */
  SUBTEST(insert) {
    list<int> li = {1, 2};
    li.insert(li.begin(), 3, 11);
    EXPECT_STRING_EQ(li, [11, 11, 11, 1, 2]);
    EXPECT_EQ(li.size(), 5);
  }

  /**
   * @test  iterator insert(const_iterator pos, Iterator first, Iterator last)
   * @brief range insert.
   */
  SUBTEST(insert) {
    std::vector<double> vc = {11, 22, 33};
    list<int> li = {1, 2, 3, 4};
    li.insert(li.begin(), vc.begin(), vc.end());
    EXPECT_STRING_EQ(li, [11, 22, 33, 1, 2, 3, 4]);
    EXPECT_EQ(li.size(), 7);
  }
}

TEST(list, max_size) {
  list<int> li;
  EXPECT_GT(li.max_size(), 0xffff);
}

TEST(list, pop_back) {
  list<int> li;
  li.pop_back();
  EXPECT_STRING_EQ(li, []);
  EXPECT_EQ(li.size(), 0);
  li = {1, 2, 3};
  li.pop_back();
  EXPECT_STRING_EQ(li, [1, 2]);
  EXPECT_EQ(li.size(), 2);
}

TEST(list, push_back) {
  /**
   * @test  void push_back(T&& x)
   * @brief push_back an rvalue.
   */
  SUBTEST(push_back) {
    list<int> li;
    li.push_back(1);
    EXPECT_STRING_EQ(li, [1]);
    EXPECT_EQ(li.size(), 1);
    li.push_back(2);
    li.push_back(3);
    EXPECT_STRING_EQ(li, [1, 2, 3]);
    EXPECT_EQ(li.size(), 3);
  }

  /**
   * @test  void push_back(const T& x)
   * @brief push_back an lvalue reference.
   */
  SUBTEST(push_back) {
    list<int> li;
    int arr[3] = {1, 2, 3};
    li.push_back(arr[0]);
    EXPECT_STRING_EQ(li, [1]);
    EXPECT_EQ(li.size(), 1);
    li.push_back(arr[1]);
    li.push_back(arr[2]);
    EXPECT_STRING_EQ(li, [1, 2, 3]);
    EXPECT_EQ(li.size(), 3);
  }
}

TEST(list, rbegin) {
  list<int> li = {1, 2, 3};
  EXPECT_EQ(*li.rbegin(), 3);
  *li.rbegin() = 4;
  EXPECT_EQ(*li.rbegin(), 4);
}

TEST(list, rend) {
  list<int> li = {1, 2, 3};
  EXPECT_EQ(*--li.rend(), 1);
  *--li.rend() = 11;
  EXPECT_EQ(*--li.rend(), 11);
}

TEST(list, resize) {
  /**
   * @test  void resize(size_type new_size)
   * @brief resize the vector.
   */
  SUBTEST(resize) {
    list<int> li = {1, 2, 3};
    li.resize(5);
    EXPECT_STRING_EQ(li, [1, 2, 3, 0, 0]);
    EXPECT_EQ(li.size(), 5);
    li.resize(2);
    EXPECT_STRING_EQ(li, [1, 2]);
    EXPECT_EQ(li.size(), 2);
  }

  /**
   * @test  void resize(size_type new_size, const T& x)
   * @brief resize the vector.
   */
  SUBTEST(resize) {
    list<int> li = {1, 2, 3};
    li.resize(5, 10);
    EXPECT_STRING_EQ(li, [1, 2, 3, 10, 10]);
    EXPECT_EQ(li.size(), 5);
    li.resize(2, 20);
    EXPECT_STRING_EQ(li, [1, 2]);
    EXPECT_EQ(li.size(), 2);
  }
}

TEST(list, size) {
  list<int> li = {1, 2, 3};
  EXPECT_EQ(li.size(), 3);
}

TEST(list, swap) {
  list<int> li1 = {1, 2, 3};
  list<int> li2 = {11, 22, 33, 44};
  li1.swap(li2);
  EXPECT_STRING_EQ(li1, [11, 22, 33, 44]);
  EXPECT_STRING_EQ(li2, [1, 2, 3]);
}

TEST(list, push_front) {
  /**
   * @test  void push_front(T&& x)
   * @brief push element at front.
   */
  SUBTEST(push_front) {
    list<int> li = {1, 2, 3, 4};
    li.push_front(0);
    EXPECT_STRING_EQ(li, [0, 1, 2, 3, 4]);
    EXPECT_EQ(li.size(), 5);
  }

  /**
   * @test  void push_front(const T& x)
   * @brief push element at front.
   */
  SUBTEST(push_front) {
    list<int> li = {1, 2, 3, 4};
    int x = 0;
    li.push_front(x);
    EXPECT_STRING_EQ(li, [0, 1, 2, 3, 4]);
    EXPECT_EQ(li.size(), 5);
  }
}

TEST(list, pop_front) {
  list<int> li;
  li.pop_front();
  li = {1, 2, 3, 4};
  li.pop_front();
  EXPECT_STRING_EQ(li, [2, 3, 4]);
  EXPECT_EQ(li.size(), 3);
}

TEST(list, splice) {
  /**
   * @test  splice(const_iterator pos, list& x)
   * @test  splice(const_iterator pos, list&& x)
   * @brief splice x to current list at pos.
   */
  SUBTEST(splice) {
    list<int> li1 = {1, 2, 3, 4, 5};
    list<int> li2 = {11, 12, 13};
    li1.splice(++li1.begin(), li2);
    EXPECT_STRING_EQ(li1, [1, 11, 12, 13, 2, 3, 4, 5]);
    EXPECT_STRING_EQ(li2, []);
    EXPECT_EQ(li1.size(), 8);
    EXPECT_EQ(li2.size(), 0);
  }

  /**
   * @test  splice(const_iterator pos, list& x, const_iterator i)
   * @test  splice(const_iterator pos, list&& x, const_iterator i)
   * @brief move i from x to current list at pos.
   */
  SUBTEST(splice) {
    list<int> li1 = {1, 2, 3, 4, 5};
    list<int> li2 = {11, 12, 13};
    li1.splice(li1.begin(), li1, ++li1.begin());
    EXPECT_STRING_EQ(li1, [2, 1, 3, 4, 5]);
    EXPECT_EQ(li1.size(), 5);

    li1.splice(++++li1.begin(), li2, li2.begin());
    EXPECT_STRING_EQ(li1, [2, 1, 11, 3, 4, 5]);
    EXPECT_STRING_EQ(li2, [12, 13]);
    EXPECT_EQ(li1.size(), 6);
    EXPECT_EQ(li2.size(), 2);
  }

  /**
   * @test  splice(const_iterator pos, list& x, 
   *               const_iterator first, const_iterator last)
   * @test  splice(const_iterator pos, list&& x, 
   *               const_iterator first, const_iterator last)
   * @brief move [first, last) from x to current list at pos.
   */
  SUBTEST(splice) {
    list<int> li1 = {1, 2, 3, 4, 5};
    list<int> li2 = {11, 12, 13};
    li1.splice(li1.begin(), li1, ++li1.begin(), --li1.end());
    EXPECT_STRING_EQ(li1, [2, 3, 4, 1, 5]);
    EXPECT_EQ(li1.size(), 5);

    li1.splice(++++li1.begin(), li2, li2.begin(), --li2.end());
    EXPECT_STRING_EQ(li1, [2, 3, 11, 12, 4, 1, 5]);
    EXPECT_STRING_EQ(li2, [13]);
    EXPECT_EQ(li1.size(), 7);
    EXPECT_EQ(li2.size(), 1);
  }
}

TEST(list, remove) {
  list<std::string> li {"Aa", "Bb", "Aa", "Cc", "Dd"};
  li.remove(li.front());
  EXPECT_STRING_EQ(li, [Bb, Cc, Dd]);
  EXPECT_EQ(li.size(), 3);
}

TEST(list, remove_if) {
  list<std::string> li {"Aa", "Bb", "Aa", "Cc", "Dd"};
  li.remove_if([](const std::string& str) {
    return str[0] < 'C';
  });
  EXPECT_STRING_EQ(li, [Cc, Dd]);
  EXPECT_EQ(li.size(), 2);
}

TEST(list, unique) {
  /**
   * @test  size_type unique()
   * @brief unique an ordered list.
   */
  SUBTEST(unique) {
    list<int> li = {1, 1, 4, 5, 5, 5, 8};
    int old = li.size();
    int cnt = li.unique();
    EXPECT_STRING_EQ(li, [1, 4, 5, 8]);
    EXPECT_EQ(cnt, old - li.size());
  }

  /**
   * @test  size_type unique(_BinaryPredicate __pred)
   * @brief unique an ordered list.
   */
  SUBTEST(unique) {
    list<int> li = {1, 1, 4, 20, 50, 53, 81};
    int old = li.size();
    int cnt = li.unique([](int a, int b) {
      return a / 10 == b / 10;
    });
    EXPECT_STRING_EQ(li, [1, 20, 50, 81]);
    EXPECT_EQ(cnt, old - li.size());
  }
}

TEST(list, merge) {
  /**
   * @test  merge(list& __x)
   * @test  merge(list&& __x)
   * @brief merge two list.
   */
  SUBTEST(merge) {
    list<int> li1 = {1, 3, 8, 9};
    list<int> li2 = {2, 3, 4, 5, 6, 11, 12};
    li1.merge(li2);
    EXPECT_STRING_EQ(li1, [1, 2, 3, 3, 4, 5, 6, 8, 9, 11, 12]);
    EXPECT_STRING_EQ(li2, []);
    EXPECT_EQ(li1.size(), 11);
    EXPECT_EQ(li2.size(), 0);
  }

  /**
   * @test  merge(list& __x, _StrictWeakOrdering __comp) 
   * @test  merge(list&& __x, _StrictWeakOrdering __comp) 
   * @brief merge two list.
   */
  SUBTEST(merge) {
    list<int> li1 = {9, 8, 3, 1};
    list<int> li2 = {12, 11, 6, 5, 4, 3, 2};
    li1.merge(li2, [](int a, int b) {
      return a > b;
    });
    EXPECT_STRING_EQ(li1, [12, 11, 9, 8, 6, 5, 4, 3, 3, 2, 1]);
    EXPECT_STRING_EQ(li2, []);
    EXPECT_EQ(li1.size(), 11);
    EXPECT_EQ(li2.size(), 0);
  }
}

TEST(list, reverse) {
  list<int> li;
  li.reverse();
  EXPECT_STRING_EQ(li, []);
  
  li = {1};
  li.reverse();
  EXPECT_STRING_EQ(li, [1]);
  
  li = {1, 2};
  li.reverse();
  EXPECT_STRING_EQ(li, [2, 1]);

  li = {1, 2, 3};
  li.reverse();
  EXPECT_STRING_EQ(li, [3, 2, 1]);
}

TEST(list, sort) {
  /**
   * @test  void sort()
   * @brief sort the list.
   */
  SUBTEST(sort) {
    list<int> li = {1, 3, 2, 7, 8, 6, 9, 5, 0, 4};
    li.sort();
    EXPECT_STRING_EQ(li, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]);
  }

  /**
   * @test  void sort(_StrictWeakOrdering __comp)
   * @brief sort the list by __comp.
   */
  SUBTEST(sort) {
    list<int> li = {1, 3, 2, 7, 8, 6, 9, 5, 0, 4};
    li.sort([](int x, int y) {
      return x > y;
    });
    EXPECT_STRING_EQ(li, [9, 8, 7, 6, 5, 4, 3, 2, 1, 0]);
  }
}

TEST(list, emplace_front) {
  list<int> li;
  li.emplace_front(1);
  EXPECT_STRING_EQ(li, [1]);
  li.emplace_front(11);
  EXPECT_STRING_EQ(li, [11, 1]);
}