#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "forward_list.h"

using namespace tinySTL;

TEST(forward_list, constructor) {
  /**
   * @test  forward_list()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    forward_list<int> fl;
    EXPECT_STRING_EQ(fl, []);
  }

  /**
   * @test  forward_list(size_type n). 
   * @brief size constructor. 
   */
  SUBTEST(constructor) {
    forward_list<int> fl(5);
    EXPECT_STRING_EQ(fl, [0, 0, 0, 0, 0]);
  }

  /**
   * @test  forward_list(size_type n, const T& val)
   * @brief size with value constructor. 
   */
  SUBTEST(constructor) {
    forward_list<std::string> fl(3, "Hello");
    EXPECT_STRING_EQ(fl, [Hello, Hello, Hello]);
  }

  /**
   * @test  forward_list(const forward_list& x)
   * @brief copy constructor. 
   */
  SUBTEST(constructor) {
    forward_list<std::string> fl1(3, "Hello");
    EXPECT_STRING_EQ(fl1, [Hello, Hello, Hello]);
    forward_list<std::string> fl2(fl1);
    EXPECT_STRING_EQ(fl2, [Hello, Hello, Hello]);
  }

  /**
   * @test  forward_list(forward_list&& x)
   * @brief move constructor. 
   */
  SUBTEST(constructor) {
    forward_list<std::string> fl1(3, "Hello");
    EXPECT_STRING_EQ(fl1, [Hello, Hello, Hello]);
    forward_list<std::string> fl2(tinySTL::move(fl1));
    EXPECT_STRING_EQ(fl1, []);
    EXPECT_STRING_EQ(fl2, [Hello, Hello, Hello]);
  }

  /**
   * @test  forward_list(initializer_list l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    forward_list<std::string> fl = { "Hello", "Standard", "Template", "Library" };
    EXPECT_STRING_EQ(fl, [Hello, Standard, Template, Library]);
  }

  /**
   * @test  forward_list(Iterator first, Iterator last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    std::vector<double> vc = { 3.14, 6.28, 0.618 };
    forward_list<int> fl(vc.begin(), vc.end());
    EXPECT_STRING_EQ(fl, [3, 6, 0]);
  }
}


TEST(forward_list, assign) {
  /**
   * @test  void assign(initializer_list l)
   * @brief initializer_list assign. 
   */
  SUBTEST(assign) {
    forward_list<int> fl { 1, 2, 3, 4 };
    fl.assign({1, 2, 3});
    EXPECT_STRING_EQ(fl, [1, 2, 3]);
  }

  /**
   * @test  void assign(size_type n, const T& val)
   * @brief size with value assign. 
   */
  SUBTEST(assign) {
    forward_list<int> fl { 1, 2, 3, 4 };
    fl.assign(3, 100);
    EXPECT_STRING_EQ(fl, [100, 100, 100]);
  }

  /**
   * @test  void assign(Iterator first, Iterator last)
   * @brief range assign. 
   */
  SUBTEST(assign) {
    std::vector<double> vc = { 3.14, 6.28, 0.618 };
    forward_list<int> fl { 1, 2, 3, 4 };
    fl.assign(vc.begin(), vc.end());
    EXPECT_STRING_EQ(fl, [3, 6, 0]);
  }
}


TEST(forward_list, assign_operator) {
  /**
   * @test forward_list& operator=(initializer_list l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    forward_list<int> fl = {1, 2, 3, 4};
    fl = {1, 2, 3};
    EXPECT_STRING_EQ(fl, [1, 2, 3]);
  }
  
  /**
   * @test forward_list& operator=(const forward_list& x)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    forward_list<int> fl1 = {1, 2, 3, 4};
    forward_list<int> fl2 = {1, 2, 3};
    fl1 = fl2;
    EXPECT_STRING_EQ(fl1, [1, 2, 3]);
  }

  /**
   * @test forward_list& operator=(forward_list&& x)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    forward_list<int> fl = {1, 2, 3, 4};
    fl = forward_list<int>({1, 2, 3});
    EXPECT_STRING_EQ(fl, [1, 2, 3]);
  }
}

TEST(forward_list, begin) {
  forward_list<int> fl = {1, 2, 3};
  EXPECT_EQ(*fl.begin(), 1);
  *fl.begin() = 11;
  EXPECT_EQ(*fl.begin(), 11);
}

TEST(forward_list, before_begin){ 
  forward_list<int> fl = {1, 2, 3};
  EXPECT_EQ(++fl.before_begin(), fl.begin());
  EXPECT_EQ(*++fl.before_begin(), 1);
  *++fl.before_begin() = 11;
  EXPECT_EQ(*++fl.before_begin(), 11);
}

TEST(forward_list, end) {
  forward_list<int> fl = {1, 2, 3};
  auto it = fl.end();
  EXPECT_EQ(it, forward_list<int>::iterator{});
}

TEST(forward_list, front) { 
  forward_list<int> fl = {1, 2, 3};
  EXPECT_EQ(fl.front(), 1);
  fl.front() = 11;
  EXPECT_EQ(fl.front(), 11);
}

TEST(forward_list, empty) {
  forward_list<int> fl;
  EXPECT_TRUE(fl.empty());
  fl = {1, 2, 3};
  EXPECT_FALSE(fl.empty());
}

TEST(forward_list, clear) { 
  forward_list<int> fl;
  fl.clear();
  EXPECT_STRING_EQ(fl, []);
  fl = {1, 2, 3};
  EXPECT_STRING_EQ(fl, [1, 2, 3]);
  fl.clear();
  EXPECT_STRING_EQ(fl, []);
}

TEST(forward_list, max_size) {
  forward_list<int> fl;
  EXPECT_GT(fl.max_size(), 0xffff);
}

TEST(forward_list, get_allocator) { 
  forward_list<int> fl;
  fl.get_allocator();
}

TEST(forward_list, insert_after) {
  /**
   * @test  insert_after(const_iterator pos, initializer_list l)
   * @brief insert an initializer_list.
   */
  SUBTEST(constructor) {
    forward_list<int> fl = {1, 2, 3};
    fl.insert_after(fl.begin(), {11, 12, 13});
    EXPECT_STRING_EQ(fl, [1, 11, 12, 13, 2, 3]);
  }

  /**
   * @test  insert_after(const_iterator pos, T&& val)
   * @brief insert an rvalue.
   */
  SUBTEST(constructor) {
    forward_list<int> fl = {1, 2, 3};
    fl.insert_after(fl.before_begin(), 11);
    EXPECT_STRING_EQ(fl, [11, 1, 2, 3]);
  }

  /**
   * @test  insert_after(const_iterator pos, const T& val)
   * @brief insert an lvalue reference.
   */
  SUBTEST(constructor) {
    forward_list<int> fl = {1, 2, 3};
    int x = 11;
    fl.insert_after(fl.before_begin(), x);
    EXPECT_STRING_EQ(fl, [11, 1, 2, 3]);
  }

  /**
   * @test  insert_after(pos, n, val)
   * @brief insert an lvalue reference n times.
   */
  SUBTEST(constructor) {
    forward_list<int> fl = {1, 2, 3};
    fl.insert_after(fl.before_begin(), 3, 11);
    EXPECT_STRING_EQ(fl, [11, 11, 11, 1, 2, 3]);
  }

  /**
   * @test  insert_after(pos, first, last)
   * @brief range insert.
   */
  SUBTEST(insert) {
    std::vector<double> vc = {11, 22, 33};
    forward_list<int> fl = {1, 2, 3};
    fl.insert_after(fl.before_begin(), vc.begin(), vc.end());
    EXPECT_STRING_EQ(fl, [11, 22, 33, 1, 2, 3]);
  }
}

TEST(forward_list, emplace_after) {
  forward_list<int> fl = {1, 2, 3};
  fl.emplace_after(fl.before_begin(), 11);
  EXPECT_STRING_EQ(fl, [11, 1, 2, 3]);
}

TEST(forward_list, emplace_front) {
  forward_list<int> fl;
  fl.emplace_front(3);
  fl.emplace_front(2);
  fl.emplace_front(1);
  EXPECT_STRING_EQ(fl, [1, 2, 3]);
}

TEST(forward_list, push_front) {
  forward_list<int> fl = {1, 2, 3, 4};
  fl.push_front(33);
  EXPECT_STRING_EQ(fl, [33, 1, 2, 3, 4]);
}

TEST(forward_list, pop_front) { 
  forward_list<int> fl = {1, 2, 3, 4};
  fl.pop_front();
  EXPECT_STRING_EQ(fl, [2, 3, 4]);
}

TEST(forward_list, erase_after) { 
  /**
   * @test  iterator erase_after(const_iterator pos)
   * @brief erase an item.
   */
  SUBTEST(erase_after) {
    forward_list<int> fl = {1, 2, 3};
    fl.erase_after(fl.before_begin());
    EXPECT_STRING_EQ(fl, [2, 3]);
  }

  /**
   * @test  iterator erase_after(first, last)
   * @brief range erase.
   */
  SUBTEST(erase_after) {
    forward_list<int> fl = {1, 2, 3};
    fl.erase_after(fl.begin(), fl.end());
    EXPECT_STRING_EQ(fl, [1]);
  }
}

TEST(forward_list, swap) { 
  forward_list<int> fl1;
  forward_list<int> fl2;

  fl1.swap(fl2);
  EXPECT_STRING_EQ(fl1, []);
  EXPECT_STRING_EQ(fl2, []);

  fl2 = {1, 2, 3};
  fl1.swap(fl2);
  EXPECT_STRING_EQ(fl1, [1, 2, 3]);
  EXPECT_STRING_EQ(fl2, []);

  fl1.swap(fl2);
  EXPECT_STRING_EQ(fl1, []);
  EXPECT_STRING_EQ(fl2, [1, 2, 3]);

  fl1 = {11, 12, 13};
  fl1.swap(fl2);
  EXPECT_STRING_EQ(fl1, [1, 2, 3]);
  EXPECT_STRING_EQ(fl2, [11, 12, 13]);
}

TEST(forward_list, resize) {
  /**
   * @test  void resize(size_type new_size)
   * @brief resize the forward_list.
   */
  SUBTEST(resize) {
    forward_list<int> fl = {1, 2, 3};
    fl.resize(5);
    EXPECT_STRING_EQ(fl, [1, 2, 3, 0, 0]);
    fl.resize(2);
    EXPECT_STRING_EQ(fl, [1, 2]);
  }

  /**
   * @test  void resize(size_type new_size, const T& x)
   * @brief resize the forward_list.
   */
  SUBTEST(resize) {
    forward_list<int> fl = {1, 2, 3};
    fl.resize(5, 10);
    EXPECT_STRING_EQ(fl, [1, 2, 3, 10, 10]);
    fl.resize(2, 20);
    EXPECT_STRING_EQ(fl, [1, 2]);
  }
}

TEST(forward_list, merge) {
  /**
   * @test  merge(forward_list& __x)
   * @test  merge(forward_list&& __x)
   * @brief merge two forward_list.
   */
  SUBTEST(merge) {
    forward_list<int> fl1 = {1, 3, 8, 9};
    forward_list<int> fl2 = {2, 3, 4, 5, 6, 11, 12};
    fl1.merge(fl2);
    EXPECT_STRING_EQ(fl1, [1, 2, 3, 3, 4, 5, 6, 8, 9, 11, 12]);
    EXPECT_STRING_EQ(fl2, []);
  }

  /**
   * @test  merge(forward_list& __x, _StrictWeakOrdering __comp) 
   * @test  merge(forward_list&& __x, _StrictWeakOrdering __comp) 
   * @brief merge two forward_list.
   */
  SUBTEST(merge) {
    forward_list<int> fl1 = {9, 8, 3, 1};
    forward_list<int> fl2 = {12, 11, 6, 5, 4, 3, 2};
    fl1.merge(fl2, [](int a, int b) {
      return a > b;
    });
    EXPECT_STRING_EQ(fl1, [12, 11, 9, 8, 6, 5, 4, 3, 3, 2, 1]);
    EXPECT_STRING_EQ(fl2, []);
  }
}

TEST(forward_list, remove) {
  forward_list<std::string> fl {"Aa", "Bb", "Aa", "Cc", "Dd"};
  fl.remove(fl.front());
  EXPECT_STRING_EQ(fl, [Bb, Cc, Dd]);
}

TEST(forward_list, remove_if) {
  forward_list<std::string> fl {"Aa", "Bb", "Aa", "Cc", "Dd"};
  fl.remove_if([](const std::string& str) {
    return str[0] < 'C';
  });
  EXPECT_STRING_EQ(fl, [Cc, Dd]);
}

TEST(forward_list, reverse) {
  forward_list<int> fl;
  fl.reverse();
  EXPECT_STRING_EQ(fl, []);
  fl.reverse();
  EXPECT_STRING_EQ(fl, []);
  
  fl = {1};
  fl.reverse();
  EXPECT_STRING_EQ(fl, [1]);
  fl.reverse();
  EXPECT_STRING_EQ(fl, [1]);
  
  fl = {1, 2};
  fl.reverse();
  EXPECT_STRING_EQ(fl, [2, 1]);
  fl.reverse();
  EXPECT_STRING_EQ(fl, [1, 2]);

  fl = {1, 2, 3};
  fl.reverse();
  EXPECT_STRING_EQ(fl, [3, 2, 1]);
  fl.reverse();
  EXPECT_STRING_EQ(fl, [1, 2, 3]);
}

TEST(forward_list, sort) {
  /**
   * @test  void sort()
   * @brief sort the forward_list.
   */
  SUBTEST(sort) {
    forward_list<int> fl = {1, 3, 2, 7, 8, 6, 9, 5, 0, 4};
    fl.sort();
    EXPECT_STRING_EQ(fl, [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]);
  }

  /**
   * @test  void sort(_StrictWeakOrdering __comp)
   * @brief sort the forward_list by __comp.
   */
  SUBTEST(sort) {
    forward_list<int> fl = {1, 3, 2, 7, 8, 6, 9, 5, 0, 4};
    fl.sort([](int x, int y) {
      return x > y;
    });
    EXPECT_STRING_EQ(fl, [9, 8, 7, 6, 5, 4, 3, 2, 1, 0]);
  }
}

TEST(forward_list, unique) {
  /**
   * @test  size_type unique()
   * @brief unique an ordered forward_list.
   */
  SUBTEST(unique) {
    forward_list<int> fl = {1, 1, 4, 5, 5, 5, 8};
    fl.unique();
    EXPECT_STRING_EQ(fl, [1, 4, 5, 8]);
  }

  /**
   * @test  size_type unique(_BinaryPredicate __pred)
   * @brief unique an ordered forward_list.
   */
  SUBTEST(unique) {
    forward_list<int> fl = {1, 1, 4, 20, 50, 53, 81};
    int cnt = fl.unique([](int a, int b) {
      return a / 10 == b / 10;
    });
    EXPECT_STRING_EQ(fl, [1, 20, 50, 81]);
  }
}

TEST(forward_list, splice) {
  /**
   * @test  splice(const_iterator pos, forward_list& x)
   * @test  splice(const_iterator pos, forward_list&& x)
   * @brief splice x to current forward_list after pos.
   */
  SUBTEST(splice) {
    forward_list<int> fl1 = {1, 2, 3, 4, 5};
    forward_list<int> fl2 = {11, 12, 13};
    fl1.splice_after(fl1.begin(), fl2);
    EXPECT_STRING_EQ(fl1, [1, 11, 12, 13, 2, 3, 4, 5]);
    EXPECT_STRING_EQ(fl2, []);
  }

  /**
   * @test  splice(const_iterator pos, forward_list& x, const_iterator i)
   * @test  splice(const_iterator pos, forward_list&& x, const_iterator i)
   * @brief move element after i from x to current forward_list after pos.
   */
  SUBTEST(splice) {
    forward_list<int> fl1 = {1, 2, 3, 4, 5};
    forward_list<int> fl2 = {11, 12, 13};
    fl1.splice_after(fl1.before_begin(), fl1, fl1.begin());
    EXPECT_STRING_EQ(fl1, [2, 1, 3, 4, 5]);

    fl1.splice_after(++fl1.begin(), fl2, fl2.before_begin());
    EXPECT_STRING_EQ(fl1, [2, 1, 11, 3, 4, 5]);
    EXPECT_STRING_EQ(fl2, [12, 13]);
  }

  /**
   * @test  splice(const_iterator pos, forward_list& x, 
   *               const_iterator first, const_iterator last)
   * @test  splice(const_iterator pos, forward_list&& x, 
   *               const_iterator first, const_iterator last)
   * @brief move [first+1, last) from x to current list after pos.
   */
  SUBTEST(splice) {
    forward_list<int> fl1 = {1, 2, 3, 4, 5};
    forward_list<int> fl2 = {11, 12, 13};
    fl1.splice_after(fl1.before_begin(), fl1, fl1.begin(), fl1.end());
    EXPECT_STRING_EQ(fl1, [2, 3, 4, 5, 1]);

    fl1.splice_after(++fl1.begin(), fl2, fl2.before_begin(), fl2.end());
    EXPECT_STRING_EQ(fl1, [2, 3, 11, 12, 13, 4, 5, 1]);
    EXPECT_STRING_EQ(fl2, []);
  }
}