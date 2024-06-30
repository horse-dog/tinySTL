#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "set.h"
#include "list.h"
#include "vector.h"

using namespace tinySTL;

TEST(set, constructor) {
  /**
   * @test  set()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    set<int> s;
    EXPECT_STRING_EQ(s, []);
  }

  /**
   * @test  set(const set&)
   * @brief copy constructor.
   */
  SUBTEST(constructor) {
    set<int> s1{2, 4, 3, 7, 1};
    EXPECT_STRING_EQ(s1, [1, 2, 3, 4, 7]);
    set<int> s2(s1);
    EXPECT_STRING_EQ(s2, [1, 2, 3, 4, 7]);
  }

  /**
   * @test  set(set&&)
   * @brief move constructor.
   */
  SUBTEST(constructor) {
    set<int> s1{2, 4, 3, 7, 1};
    EXPECT_STRING_EQ(s1, [1, 2, 3, 4, 7]);
    set<int> s2(tinySTL::move(s1));
    EXPECT_EQ(s1.size(), 0);
    EXPECT_EQ(s2.size(), 5);
    EXPECT_STRING_EQ(s1, []);
    EXPECT_STRING_EQ(s2, [1, 2, 3, 4, 7]);
  }

  /**
   * @test  set(initializer_list l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    set<std::string> s = { "Hello", "Standard", "Template", "Library" };
    EXPECT_STRING_EQ(s, [Hello, Library, Standard, Template]);
  }

  /**
   * @test  set(Iterator first, Iterator last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    list<double> l = { 3.14, 6.28, 0.618 };
    set<int> s(l.begin(), l.end());
    EXPECT_STRING_EQ(s, [0, 3, 6]);
  }
}

TEST(set, assign_operator) {
  /**
   * @test  set& operator=(initializer_list l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    set<int> s = {1, 2, 3, 4};
    s = {1, 2, 3};
    EXPECT_STRING_EQ(s, [1, 2, 3]);
    EXPECT_EQ(s.size(), 3);
  }
  
  /**
   * @test  set& operator=(const set& x)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    set<int> s0 = {1, 2, 3, 4};
    set<int> s1 = {1, 2, 3};
    s0 = s1;
    EXPECT_STRING_EQ(s0, [1, 2, 3]);
    EXPECT_EQ(s0.size(), 3);
    EXPECT_STRING_EQ(s1, [1, 2, 3]);
    EXPECT_EQ(s1.size(), 3);
  }

  /**
   * @test  set& operator=(set&& x)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    set<int> s = {1, 2, 3, 4};
    s = set<int>({1, 2, 3});
    EXPECT_STRING_EQ(s, [1, 2, 3]);
    EXPECT_EQ(s.size(), 3);
  }
}

TEST(set, key_compare) {
  set<int> s {1, 2, 3, 4};
  auto comp = s.key_comp();
  EXPECT_TRUE(comp(*s.begin(), *++s.begin()));
}

TEST(set, value_compare) {
  set<int> s {1, 2, 3, 4};
  auto comp = s.value_comp();
  EXPECT_TRUE(comp(*s.begin(), *++s.begin()));
}

TEST(set, get_allocator) {
  set<int> s;
  auto alloc = s.get_allocator();
}

TEST(set, begin) {
  set<int> s {1, 4, 2, 5, 0};
  auto it = s.begin();
  EXPECT_EQ(*it, 0);
}

TEST(set, end) {
  set<int> s;
  auto it = --s.end();
  s.insert(1);
  it = --s.end();
  EXPECT_EQ(*it, 1);
}

TEST(set, rbegin) {
  set<int> s {1, 4, 2, 5, 0};
  auto it = s.rbegin();
  EXPECT_EQ(*it, 5);
}

TEST(set, rend) {
  set<int> s {1, 4, 2, 5, 0};
  auto it = --s.rend();
  EXPECT_EQ(*it, 0);
}

TEST(set, empty) {
  set<int> s;
  EXPECT_TRUE(s.empty());
  s.insert(111);
  EXPECT_FALSE(s.empty());
  s.erase(111);
  EXPECT_TRUE(s.empty());
}

TEST(set, size) {
  set<int> s;
  EXPECT_EQ(s.size(), 0);
  s.insert(1);
  s.insert(4);
  s.insert(2);
  EXPECT_EQ(s.size(), 3);
  s.erase(1);
  EXPECT_EQ(s.size(), 2);
  s.erase(5);
  EXPECT_EQ(s.size(), 2);
  s.clear();
  EXPECT_EQ(s.size(), 0);
}

TEST(set, max_size) {
  set<int> s;
  EXPECT_TRUE(s.max_size() > 0);
}

TEST(set, swap) {
  set<int> s1, s2 {1, 4, 2, 5, 0};
  s1.swap(s2);
  EXPECT_STRING_EQ(s1, [0, 1, 2, 4, 5]);
  EXPECT_STRING_EQ(s2, []);
  s2 = {111, 333};
  s1.swap(s2);
  EXPECT_STRING_EQ(s1, [111, 333]);
  EXPECT_STRING_EQ(s2, [0, 1, 2, 4, 5]);
}

TEST(set, emplace) {
  using pair_t = pair<int, std::string>;
  set<pair_t, decltype([](const pair_t& x, const pair_t& y) {
    return x.first < y.first;
  })> s;
  s.emplace(1, "Hello");
  s.emplace(2, "World");
  EXPECT_STRING_EQ(s, [{1, Hello}, {2, World}]);
}

TEST(set, emplace_hint) {
  using pair_t = pair<int, std::string>;
  set<pair_t, decltype([](const pair_t& x, const pair_t& y) {
    return x.first < y.first;
  })> s;
  auto it = s.emplace_hint(s.end(), 2, "World");
  it = s.emplace_hint(it, 1, "Hello");
  EXPECT_STRING_EQ(s, [{1, Hello}, {2, World}]);
}

TEST(set, insert) {
  /**
   * @test  pair<iterator, bool> insert(const value_type& __x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    set<int> s;
    int value = 1;
    auto [it, ok] = s.insert(value);
    EXPECT_EQ(*it, 1);
    EXPECT_TRUE(ok);
    auto pr = s.insert(value);
    EXPECT_EQ(*pr.first, 1);
    EXPECT_FALSE(pr.second);
  }
  
  /**
   * @test  pair<iterator, bool> insert(value_type&& __x)
   * @brief insert an rvalue reference.
   */
  SUBTEST(insert) {
    set<vector<int>> s;
    vector<int> vc {1, 2, 3};
    EXPECT_STRING_EQ(vc, [1, 2, 3]);
    s.insert(tinySTL::move(vc));
    EXPECT_STRING_EQ(vc, []);
    EXPECT_TRUE(vc.empty());
    vc = {1, 2, 3};
    auto [it, ok] = s.insert(tinySTL::move(vc));
    EXPECT_STRING_EQ(*it, [1, 2, 3]);
    EXPECT_FALSE(ok);
  }

  /**
   * @test  iterator insert(const_iterator __hint, const value_type& __x) 
   * @brief insert an lvalue reference and using hint.
   */
  SUBTEST(insert) {
    set<int> s;
    int i = 5;
    s.insert(s.begin(), --i);
    s.insert(s.begin(), --i);
    s.insert(s.begin(), --i);
    EXPECT_STRING_EQ(s, [2, 3, 4]);
  }

  /**
   * @test  iterator insert(const_iterator __hint, value_type&& __x) 
   * @brief insert an rvalue reference and using hint.
   */
  SUBTEST(insert) {
    set<int> s;
    s.insert(s.begin(), 4);
    s.insert(s.begin(), 3);
    s.insert(s.begin(), 2);
    EXPECT_STRING_EQ(s, [2, 3, 4]);
  }

  /**
   * @test  void insert(_InputIterator __first, _InputIterator __last)
   * @brief range insert.
   */
  SUBTEST(insert) {
    set<int> s;
    vector<int> vc {1, 4, 2, 5, 0};
    s.insert(vc.begin(), vc.begin()+4);
    EXPECT_STRING_EQ(s, [1, 2, 4, 5]);
  }

  /**
   * @test  void insert(std::initializer_list<value_type> __l)
   * @brief insert initializer_list.
   */
  SUBTEST(insert) {
    set<int> s;
    s.insert({1, 4, 2, 5, 0});
    EXPECT_STRING_EQ(s, [0, 1, 2, 4, 5]);
  }
}

TEST(set, erase) {
  /**
   * @test  size_type erase(const key_type& __k) 
   * @brief erase by value.
   */
  SUBTEST(erase) {
    set<int> s = {1, 4, 2, 5, 0};
    EXPECT_EQ(s.erase(4), 1);
    EXPECT_EQ(s.erase(1), 1);
    EXPECT_EQ(s.erase(1), 0);
    EXPECT_STRING_EQ(s, [0, 2, 5]);
  }

  /**
   * @test  iterator erase(const_iterator __position) 
   * @brief erase by position.
   */
  SUBTEST(erase) {
    set<int> s = {1, 4, 2, 5, 0};
    auto it = s.erase(s.begin());
    EXPECT_EQ(*it, 1);
    EXPECT_THROW(s.erase(s.end()), std::range_error);
  }

  /**
   * @test  iterator erase(const_iterator __first, const_iterator __last) 
   * @brief erase by range.
   */
  SUBTEST(erase) {
    set<int> s = {1, 4, 2, 5, 0};
    auto it = s.erase(s.begin(), ----s.end());
    EXPECT_EQ(*it, 4);
    EXPECT_STRING_EQ(s, [4, 5]);
  }
}

TEST(set, clear) {
  set<int> s = {1, 4, 2, 5, 0};
  s.clear();
  EXPECT_EQ(s.size(), 0);
  EXPECT_STRING_EQ(s, []);
}

TEST(set, count) {
  set<int> s = {1, 4, 2, 5, 0};
  EXPECT_EQ(s.count(3), 0);
  EXPECT_EQ(s.count(4), 1);
}

TEST(set, contains) {
  set<int> s = {1, 4, 2, 5, 0};
  EXPECT_TRUE(s.contains(2));
  EXPECT_FALSE(s.contains(3));
}

TEST(set, find) {
  set<int> s1 = {1, 4, 2, 5, 0};
  set<int> s2;
  for (auto x : s1) {
    if (s2.find(x) == s2.end()) {
      s2.insert(x);
    }
  }
  EXPECT_EQ(s1, s2);
}

TEST(set, lower_bound) {
  set<int> s;
  EXPECT_EQ(s.lower_bound(0), s.begin());
  s = {1, 4, 2, 5, 0};
  EXPECT_EQ(s.lower_bound(0), s.begin());
  EXPECT_EQ(s.lower_bound(100), s.end());
  EXPECT_EQ(*s.lower_bound(3), 4);
  EXPECT_EQ(*s.lower_bound(4), 4);
  EXPECT_EQ(*s.lower_bound(5), 5);
}

TEST(set, upper_bound) {
  set<int> s;
  EXPECT_EQ(s.upper_bound(0), s.end());
  s = {1, 4, 2, 5, 0};
  EXPECT_EQ(*s.upper_bound(0), 1);
  EXPECT_EQ(s.upper_bound(100), s.end());
  EXPECT_EQ(*s.upper_bound(3), 4);
  EXPECT_EQ(*s.upper_bound(4), 5);
  EXPECT_EQ(s.upper_bound(5), s.end());
}

TEST(set, equal_range) {
  set<int> s = {1, 4, 2, 5, 0};
  auto [l1, r1] = s.equal_range(3);
  EXPECT_EQ(l1, r1);
  auto [l2, r2] = s.equal_range(4);
  EXPECT_EQ(*l2, 4);
  EXPECT_EQ(*r2, 5);
  EXPECT_EQ(++l2, r2);
}

TEST(set, disp) {
  set<int> s;
  s.disp(std::cout);
  s = {1, 4, 2, 5, 0};
  s.disp(std::cout);
}

TEST(set, merge) {
  /**
   * @brief merge another same compare set.
   */
  SUBTEST(merge) {
    set<int> s1;
    set<int> s2 = {1, 4, 2, 5, 0};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 4, 5]);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_EQ(s1.size(), 5);
    EXPECT_EQ(s2.size(), 0);

    s2 = {3, 4, 5};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 3, 4, 5]);
    EXPECT_STRING_EQ(s2, [4, 5]);
    EXPECT_EQ(s1.size(), 6);
    EXPECT_EQ(s2.size(), 2);

    set<int> s3;
    s2.merge(s3);
    EXPECT_STRING_EQ(s2, [4, 5]);
    EXPECT_STRING_EQ(s3, []);
    EXPECT_EQ(s2.size(), 2);
    EXPECT_EQ(s3.size(), 0);
  }

  /**
   * @brief merge another different compare set.
   */
  SUBTEST(merge) {
    using sset = set<int, tinySTL::greater<int>>;
    set<int> s1;
    sset s2 = {1, 4, 2, 5, 0};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 4, 5]);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_EQ(s1.size(), 5);
    EXPECT_EQ(s2.size(), 0);

    s2 = {3, 4, 5};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 3, 4, 5]);
    EXPECT_STRING_EQ(s2, [5, 4]);
    EXPECT_EQ(s1.size(), 6);
    EXPECT_EQ(s2.size(), 2);

    set<int> s3;
    s2.merge(s3);
    EXPECT_STRING_EQ(s2, [5, 4]);
    EXPECT_STRING_EQ(s3, []);
    EXPECT_EQ(s2.size(), 2);
    EXPECT_EQ(s3.size(), 0);
  }

  /**
   * @brief merge another same compare multiset.
   */
  SUBTEST(merge) {
    set<int> s1;
    multiset<int> s2 = {1, 1, 4, 2, 2, 5, 0};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 4, 5]);
    EXPECT_STRING_EQ(s2, [1, 2]);
    EXPECT_EQ(s1.size(), 5);
    EXPECT_EQ(s2.size(), 2);

    s2 = {3, 4, 4, 5};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 3, 4, 5]);
    EXPECT_STRING_EQ(s2, [4, 4, 5]);
    EXPECT_EQ(s1.size(), 6);
    EXPECT_EQ(s2.size(), 3);

    set<int> s3;
    s2.merge(s3);
    EXPECT_STRING_EQ(s2, [4, 4, 5]);
    EXPECT_STRING_EQ(s3, []);
    EXPECT_EQ(s2.size(), 3);
    EXPECT_EQ(s3.size(), 0);
  }

  /**
   * @brief merge another different compare multiset.
   */
  SUBTEST(merge) {
    using smultiset = multiset<int, tinySTL::greater<int>>;
    set<int> s1;
    smultiset s2 = {1, 1, 4, 2, 2, 5, 0};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 4, 5]);
    EXPECT_STRING_EQ(s2, [2, 1]);
    EXPECT_EQ(s1.size(), 5);
    EXPECT_EQ(s2.size(), 2);

    s2 = {3, 4, 4, 5};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 3, 4, 5]);
    EXPECT_STRING_EQ(s2, [5, 4, 4]);
    EXPECT_EQ(s1.size(), 6);
    EXPECT_EQ(s2.size(), 3);

    set<int> s3;
    s2.merge(s3);
    EXPECT_STRING_EQ(s2, [5, 4, 4]);
    EXPECT_STRING_EQ(s3, []);
    EXPECT_EQ(s2.size(), 3);
    EXPECT_EQ(s3.size(), 0);
  }
}

TEST(set, compare_operator) {
  set<int> s1 = {1, 4, 2, 5, 0};
  set<int> s2 = {1, 4, 2, 0};
  set<int> s3 = {4, 2};
  EXPECT_TRUE(s1 == s1);
  EXPECT_FALSE(s1 == s2);
  EXPECT_TRUE(s2 < s1);
  EXPECT_TRUE(s2 < s3);
}

TEST(set, cout_operator) {
  set<int> s1 = {1, 4, 2, 5, 0};
  set<int> s2 = {1, 4, 2, 0};
  set<int> s3 = {4, 2};
  set<set<int>> s = {s1, s2, s3};
  std::cout << s << std::endl;
}
