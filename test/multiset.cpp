#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "set.h"
#include "list.h"
#include "vector.h"

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

TEST(multiset, assign_operator) {
  /**
   * @test  multiset& operator=(initializer_list l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    multiset<int> s = {1, 2, 3, 4};
    s = {1, 2, 3, 3, 5};
    EXPECT_STRING_EQ(s, [1, 2, 3, 3, 5]);
    EXPECT_EQ(s.size(), 5);
  }
  
  /**
   * @test  multiset& operator=(const multiset& x)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    multiset<int> s0 = {1, 2, 2, 4};
    multiset<int> s1 = {1, 2, 3, 3, 5};
    s0 = s1;
    EXPECT_STRING_EQ(s0, [1, 2, 3, 3, 5]);
    EXPECT_EQ(s0.size(), 5);
    EXPECT_STRING_EQ(s1, [1, 2, 3, 3, 5]);
    EXPECT_EQ(s1.size(), 5);
  }

  /**
   * @test  multiset& operator=(multiset&& x)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    multiset<int> s = {1, 2, 2, 4};
    s = multiset<int>({1, 2, 3, 3, 5});
    EXPECT_STRING_EQ(s, [1, 2, 3, 3, 5]);
    EXPECT_EQ(s.size(), 5);
  }
}

TEST(multiset, key_compare) {
  multiset<int> s {1, 2, 3, 4};
  auto comp = s.key_comp();
  EXPECT_TRUE(comp(*s.begin(), *++s.begin()));
}

TEST(multiset, value_compare) {
  multiset<int> s {1, 2, 3, 4};
  auto comp = s.value_comp();
  EXPECT_TRUE(comp(*s.begin(), *++s.begin()));
}

TEST(multiset, get_allocator) {
  multiset<int> s;
  auto alloc = s.get_allocator();
}

TEST(multiset, begin) {
  multiset<int> s {5, 4, 0, 5, 0};
  auto it = s.begin();
  EXPECT_EQ(*it, 0);
}

TEST(multiset, end) {
  multiset<int> s;
  auto it = --s.end();
  s = {5, 4, 0, 5, 0};
  it = --s.end();
  EXPECT_EQ(*it, 5);
}

TEST(multiset, rbegin) {
  multiset<int> s {2, 4, 2, 5, 2};
  auto it = s.rbegin();
  EXPECT_EQ(*it, 5);
}

TEST(multiset, rend) {
  multiset<int> s {2, 4, 2, 5, 2};
  auto it = --s.rend();
  EXPECT_EQ(*it, 2);
  EXPECT_EQ(s.size(), 5);
}

TEST(multiset, empty) {
  multiset<int> s;
  EXPECT_TRUE(s.empty());
  s.insert(111);
  s.insert(111);
  s.insert(111);
  EXPECT_FALSE(s.empty());
  s.insert(123);
  EXPECT_FALSE(s.empty());
  s.erase(111);
  EXPECT_FALSE(s.empty());
  s.erase(123);
  EXPECT_TRUE(s.empty());
}

TEST(multiset, size) {
  multiset<int> s;
  EXPECT_EQ(s.size(), 0);
  s.insert(111);
  s.insert(111);
  s.insert(111);
  EXPECT_EQ(s.size(), 3);
  s.insert(123);
  EXPECT_EQ(s.size(), 4);
  s.erase(111);
  EXPECT_EQ(s.size(), 1);
  s.erase(123);
  EXPECT_EQ(s.size(), 0);
}

TEST(multiset, max_size) {
  multiset<int> s;
  EXPECT_TRUE(s.max_size() > 0);
}

TEST(multiset, swap) {
  multiset<int> s1, s2 {1, 2, 2, 1, 0};
  s1.swap(s2);
  EXPECT_STRING_EQ(s1, [0, 1, 1, 2, 2]);
  EXPECT_STRING_EQ(s2, []);
  s2 = {111, 111, 333, 111};
  s1.swap(s2);
  EXPECT_STRING_EQ(s1, [111, 111, 111, 333]);
  EXPECT_STRING_EQ(s2, [0, 1, 1, 2, 2]);
}

TEST(multiset, emplace) {
  using pair_t = pair<int, std::string>;
  multiset<pair_t, decltype([](const pair_t& x, const pair_t& y) {
    return x.first < y.first;
  })> s;
  s.emplace(1, "Hello");
  s.emplace(2, "World");
  s.emplace(1, "Fuck");
  EXPECT_STRING_EQ(s, [{1, Hello}, {1, Fuck}, {2, World}]);
}

TEST(multiset, emplace_hint) {
  using pair_t = pair<int, std::string>;
  multiset<pair_t, decltype([](const pair_t& x, const pair_t& y) {
    return x.first < y.first;
  })> s;
  auto it = s.emplace_hint(s.end(), 2, "World");
  it = s.emplace_hint(it, 1, "Hello");
  it = s.emplace_hint(it, 1, "Fuck");
  EXPECT_STRING_EQ(s, [{1, Fuck}, {1, Hello}, {2, World}]);
}

TEST(multiset, insert) {
  /**
   * @test  iterator insert(const value_type& __x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    multiset<int> s;
    int value = 1;
    auto it = s.insert(value);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(s.size(), 1);
    it = s.insert(value);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(s.size(), 2);
    value = 5;
    it = s.insert(value);
    EXPECT_EQ(*it, 5);
    EXPECT_EQ(s.size(), 3);
    EXPECT_STRING_EQ(s, [1, 1, 5]);
  }
  
  /**
   * @test  iterator insert(value_type&& __x)
   * @brief insert an rvalue reference.
   */
  SUBTEST(insert) {
    multiset<vector<int>> s;
    vector<int> vc {1, 2, 3};
    EXPECT_STRING_EQ(vc, [1, 2, 3]);
    s.insert(tinySTL::move(vc));
    EXPECT_STRING_EQ(vc, []);
    EXPECT_TRUE(vc.empty());
    vc = {1, 2, 3};
    auto it = s.insert(tinySTL::move(vc));
    EXPECT_STRING_EQ(*it, [1, 2, 3]);
    EXPECT_STRING_EQ(vc, []);
    EXPECT_EQ(s.size(), 2);
    EXPECT_EQ(vc.size(), 0);
  }

  /**
   * @test  iterator insert(const_iterator __hint, const value_type& __x) 
   * @brief insert an lvalue reference and using hint.
   */
  SUBTEST(insert) {
    multiset<int> s;
    int i = 5;
    s.insert(s.begin(), --i);
    s.insert(s.begin(), --i);
    s.insert(s.begin(), --i);
    s.insert(s.begin(), i);
    EXPECT_STRING_EQ(s, [2, 2, 3, 4]);
  }

  /**
   * @test  iterator insert(const_iterator __hint, value_type&& __x) 
   * @brief insert an rvalue reference and using hint.
   */
  SUBTEST(insert) {
    multiset<int> s;
    s.insert(s.begin(), 4);
    s.insert(s.begin(), 3);
    s.insert(s.begin(), 2);
    s.insert(s.begin(), 2);
    EXPECT_STRING_EQ(s, [2, 2, 3, 4]);
  }

  /**
   * @test  void insert(_InputIterator __first, _InputIterator __last)
   * @brief range insert.
   */
  SUBTEST(insert) {
    multiset<int> s;
    vector<int> vc {1, 2, 2, 5, 2};
    s.insert(vc.begin(), vc.begin()+4);
    EXPECT_STRING_EQ(s, [1, 2, 2, 5]);
  }

  /**
   * @test  void insert(std::initializer_list<value_type> __l)
   * @brief insert initializer_list.
   */
  SUBTEST(insert) {
    multiset<int> s;
    s.insert({1, 2, 2, 5, 2});
    EXPECT_STRING_EQ(s, [1, 2, 2, 2, 5]);
  }
}

TEST(multiset, erase) {
  /**
   * @test  size_type erase(const key_type& __k) 
   * @brief erase by value.
   */
  SUBTEST(erase) {
    multiset<int> s = {1, 4, 2, 5, 1};
    EXPECT_EQ(s.erase(4), 1);
    EXPECT_EQ(s.size(), 4);
    EXPECT_EQ(s.erase(1), 2);
    EXPECT_EQ(s.size(), 2);
    EXPECT_EQ(s.erase(1), 0);
    EXPECT_EQ(s.size(), 2);
    EXPECT_STRING_EQ(s, [2, 5]);
  }

  /**
   * @test  iterator erase(const_iterator __position) 
   * @brief erase by position.
   */
  SUBTEST(erase) {
    multiset<int> s = {1, 4, 2, 5, 1};
    auto it = s.erase(s.begin());
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(s.size(), 4);
    EXPECT_THROW(s.erase(s.end()), std::range_error);
  }

  /**
   * @test  iterator erase(const_iterator __first, const_iterator __last) 
   * @brief erase by range.
   */
  SUBTEST(erase) {
    multiset<int> s = {1, 4, 2, 5, 1};
    auto it = s.erase(s.begin(), ----s.end());
    EXPECT_EQ(*it, 4);
    EXPECT_STRING_EQ(s, [4, 5]);
  }
}

TEST(multiset, clear) {
  multiset<int> s = {1, 4, 2, 4, 0};
  s.clear();
  EXPECT_EQ(s.size(), 0);
  EXPECT_STRING_EQ(s, []);
}

TEST(multiset, count) {
  multiset<int> s = {1, 4, 2, 5, 2};
  EXPECT_EQ(s.count(3), 0);
  EXPECT_EQ(s.count(2), 2);
}

TEST(multiset, contains) {
  multiset<int> s = {1, 4, 2, 5, 2};
  EXPECT_TRUE(s.contains(2));
  EXPECT_FALSE(s.contains(3));
}

TEST(multiset, find) {
  multiset<int> s1 = {1, 4, 2, 5, 2};
  multiset<int> s2;
  for (auto x : s1) {
    if (s2.find(x) == s2.end()) {
      s2.insert(x);
    }
  }
  EXPECT_STRING_EQ(s2, [1, 2, 4, 5]);
}

TEST(multiset, lower_bound) {
  multiset<int> s;
  EXPECT_EQ(s.lower_bound(0), s.begin());
  s = {1, 0, 2, 5, 0};
  EXPECT_EQ(s.lower_bound(0), s.begin());
  EXPECT_EQ(s.lower_bound(100), s.end());
  EXPECT_EQ(*s.lower_bound(2), 2);
  EXPECT_EQ(*s.lower_bound(4), 5);
  EXPECT_EQ(*s.lower_bound(5), 5);
}

TEST(multiset, upper_bound) {
  multiset<int> s;
  EXPECT_EQ(s.upper_bound(0), s.end());
  s = {1, 0, 2, 5, 0};
  EXPECT_EQ(*s.upper_bound(0), 1);
  EXPECT_EQ(s.upper_bound(100), s.end());
  EXPECT_EQ(*s.upper_bound(1), 2);
  EXPECT_EQ(*s.upper_bound(2), 5);
  EXPECT_EQ(*s.upper_bound(4), 5);
  EXPECT_EQ(s.upper_bound(5), s.end());
}

TEST(multiset, equal_range) {
  multiset<int> s = {1, 0, 2, 5, 0};
  auto [l1, r1] = s.equal_range(3);
  EXPECT_EQ(l1, r1);
  auto [l2, r2] = s.equal_range(0);
  EXPECT_EQ(*l2, 0);
  EXPECT_EQ(*r2, 1);
  EXPECT_EQ(++++l2, r2);
}

TEST(multiset, disp) {
  multiset<int> s;
  s.disp(std::cout);
  s = {1, 4, 2, 5, 2};
  s.disp(std::cout);
}

TEST(multiset, merge) {
  /**
   * @brief merge another same compare multiset.
   */
  SUBTEST(merge) {
    multiset<int> s1;
    multiset<int> s2 = {1, 4, 2, 5, 2};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [1, 2, 2, 4, 5]);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_EQ(s1.size(), 5);
    EXPECT_EQ(s2.size(), 0);

    s2 = {3, 4, 3};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [1, 2, 2, 3, 3, 4, 4, 5]);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_EQ(s1.size(), 8);
    EXPECT_EQ(s2.size(), 0);

    multiset<int> s3;
    s2.merge(s3);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_STRING_EQ(s3, []);
    EXPECT_EQ(s2.size(), 0);
    EXPECT_EQ(s3.size(), 0);
  }

  /**
   * @brief merge another different compare multiset.
   */
  SUBTEST(merge) {
    using smset = multiset<int, tinySTL::greater<int>>;
    multiset<int> s1;
    smset s2 = {1, 4, 2, 5, 2};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [1, 2, 2, 4, 5]);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_EQ(s1.size(), 5);
    EXPECT_EQ(s2.size(), 0);

    s2 = {3, 3, 4};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [1, 2, 2, 3, 3, 4, 4, 5]);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_EQ(s1.size(), 8);
    EXPECT_EQ(s2.size(), 0);

    set<int> s3;
    s2.merge(s3);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_STRING_EQ(s3, []);
    EXPECT_EQ(s2.size(), 0);
    EXPECT_EQ(s3.size(), 0);
  }

  /**
   * @brief merge another same compare set.
   */
  SUBTEST(merge) {
    multiset<int> s1;
    set<int> s2 = {1, 4, 2, 5, 0};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 4, 5]);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_EQ(s1.size(), 5);
    EXPECT_EQ(s2.size(), 0);

    s2 = {3, 4, 5};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 3, 4, 4, 5, 5]);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_EQ(s1.size(), 8);
    EXPECT_EQ(s2.size(), 0);

    set<int> s3;
    s2.merge(s3);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_STRING_EQ(s3, []);
    EXPECT_EQ(s2.size(), 0);
    EXPECT_EQ(s3.size(), 0);
  }

  /**
   * @brief merge another different compare set.
   */
  SUBTEST(merge) {
    using sset = set<int, tinySTL::greater<int>>;
    multiset<int> s1;
    sset s2 = {1, 4, 2, 5, 0};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 4, 5]);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_EQ(s1.size(), 5);
    EXPECT_EQ(s2.size(), 0);

    s2 = {3, 4, 5};
    s1.merge(s2);
    EXPECT_STRING_EQ(s1, [0, 1, 2, 3, 4, 4, 5, 5]);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_EQ(s1.size(), 8);
    EXPECT_EQ(s2.size(), 0);

    set<int> s3;
    s2.merge(s3);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_STRING_EQ(s3, []);
    EXPECT_EQ(s2.size(), 0);
    EXPECT_EQ(s3.size(), 0);
  }
}

TEST(multiset, compare_operator) {
  multiset<int> s1 = {1, 2, 2, 5, 0};
  multiset<int> s2 = {1, 2, 2, 0};
  multiset<int> s3 = {4, 2};
  EXPECT_TRUE(s1 == s1);
  EXPECT_FALSE(s1 == s2);
  EXPECT_TRUE(s2 < s1);
  EXPECT_TRUE(s2 < s3);
}

TEST(multiset, cout_operator) {
  multiset<int> s = {1, 4, 2, 5, 2};
  std::cout << s << std::endl;
}