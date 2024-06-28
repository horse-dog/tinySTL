#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "unordered_set.h"

using namespace tinySTL;


TEST(unordered_set, constructor) {
  /**
   * @test  unordered_set()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    unordered_set<int> s;
    EXPECT_STRING_EQ(s, []);
  }

  /**
   * @test  unordered_set(const unordered_set&)
   * @brief copy constructor.
   */
  SUBTEST(constructor) {
    unordered_set<int> s1{2, 4, 3, 7, 1};
    EXPECT_STRING_EQ(s1, [1, 2, 3, 4, 7]);
    unordered_set<int> s2(s1);
    EXPECT_STRING_EQ(s2, [1, 2, 3, 4, 7]);
  }

  /**
   * @test  unordered_set(unordered_set&&)
   * @brief move constructor.
   */
  SUBTEST(constructor) {
    unordered_set<int> s1{2, 4, 3, 7, 1};
    EXPECT_STRING_EQ(s1, [1, 2, 3, 4, 7]);
    unordered_set<int> s2(tinySTL::move(s1));
    EXPECT_EQ(s1.size(), 0);
    EXPECT_EQ(s2.size(), 5);
    EXPECT_STRING_EQ(s1, []);
    EXPECT_STRING_EQ(s2, [1, 2, 3, 4, 7]);
  }

  /**
   * @test  unordered_set(initializer_list l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    unordered_set<std::string> s = { "Hello", "Standard", "Template", "Library" };
    EXPECT_STRING_EQ(s, [Hello, Library, Template, Standard]);
  }

  /**
   * @test  unordered_set(Iterator first, Iterator last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    vector<double> l = { 3.14, 6.28, 0.618 };
    unordered_set<int> s(l.begin(), l.end());
    EXPECT_STRING_EQ(s, [0, 6, 3]);
  }
}

TEST(unordered_set, assign_operator) {
  /**
   * @test  unordered_set& operator=(initializer_list l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    unordered_set<int> s = {1, 2, 3, 4};
    s = {1, 2, 3};
    EXPECT_STRING_EQ(s, [1, 2, 3]);
    EXPECT_EQ(s.size(), 3);
  }
  
  /**
   * @test  unordered_set& operator=(const unordered_set& x)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    unordered_set<int> s0 = {1, 2, 3, 4};
    unordered_set<int> s1 = {1, 2, 3};
    s0 = s1;
    EXPECT_STRING_EQ(s0, [1, 2, 3]);
    EXPECT_EQ(s0.size(), 3);
    EXPECT_STRING_EQ(s1, [1, 2, 3]);
    EXPECT_EQ(s1.size(), 3);
  }

  /**
   * @test  unordered_set& operator=(unordered_set&& x)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    unordered_set<int> s = {1, 2, 3, 4};
    s = unordered_set<int>({1, 2, 3});
    EXPECT_STRING_EQ(s, [1, 2, 3]);
    EXPECT_EQ(s.size(), 3);
  }
}

TEST(unordered_set, begin) {
  unordered_set<int> s {1, 4, 2, 5, 0};
  auto it = s.begin();
  EXPECT_EQ(*it, 0);
}

TEST(unordered_set, cbegin) {
  unordered_set<int> s {1, 4, 2, 5, 0};
  auto it = s.cbegin();
  EXPECT_EQ(*it, 0);
}

TEST(unordered_set, end) {
  unordered_set<int> s;
  auto end = s.end();
  s.insert(1);
  auto it = s.begin();
  EXPECT_EQ(++it, end);
}

TEST(unordered_set, cend) {
  unordered_set<int> s;
  auto end = s.cend();
  s.insert(1);
  auto it = s.cbegin();
  EXPECT_EQ(++it, end);
}

TEST(unordered_set, bucket) {
  unordered_set<int> s {1, 4, 2, 5, 0, 23};
#define __Slot_print(x) printf("slot of "#x": %zd\n", s.bucket(x))
  __Slot_print(1);
  __Slot_print(4);
  __Slot_print(2);
  __Slot_print(5);
  __Slot_print(0);
  __Slot_print(12);
#undef __Slot_print
}

TEST(unordered_set, bucket_count) {
  unordered_set<int> s {1, 4, 2, 5, 0, 23};
  EXPECT_EQ(s.bucket_count(), 11);
}

TEST(unordered_set, bucket_size) {
  unordered_set<int> s {1, 4, 2, 5, 0, 23};
  EXPECT_EQ(s.bucket_size(0), 1);
  EXPECT_EQ(s.bucket_size(1), 2);
}

TEST(unordered_set, clear) {
  unordered_set<int> s {1, 4, 2, 5, 0, 23};
  s.clear();
  EXPECT_STRING_EQ(s, []);
}

TEST(unordered_set, contains) {
  unordered_set<int> s = {1, 4, 2, 5, 0};
  EXPECT_TRUE(s.contains(2));
  EXPECT_FALSE(s.contains(3));
}

TEST(unordered_set, count) {
  unordered_set<int> s = {1, 4, 2, 5, 0};
  EXPECT_EQ(s.count(3), 0);
  EXPECT_EQ(s.count(4), 1);
}

TEST(unordered_set, emplace) {
  using pair_t = pair<int, std::string>;
  unordered_set<pair_t, decltype([](const pair_t& x) {
    return std::hash<int>()(x.first);
  })> s;
  s.emplace(1, "Hello");
  s.emplace(2, "World");
  EXPECT_STRING_EQ(s, [{2, World}, {1, Hello}]);
}

TEST(unordered_set, emplace_hint) {
  using pair_t = pair<int, std::string>;
  unordered_set<pair_t, decltype([](const pair_t& x) {
    return std::hash<int>()(x.first);
  })> s;
  auto it = s.emplace_hint(s.end(), 2, "World");
  it = s.emplace_hint(it, 1, "Hello");
  EXPECT_STRING_EQ(s, [{2, World}, {1, Hello}]);
}

TEST(unordered_set, empty) {
  unordered_set<int> s;
  EXPECT_TRUE(s.empty());
  s.insert(111);
  EXPECT_FALSE(s.empty());
  s.erase(111);
  EXPECT_TRUE(s.empty());
}

TEST(unordered_set, equal_range) {
  unordered_set<int> s = {1, 4, 2, 5, 0};
  auto [l1, r1] = s.equal_range(3);
  EXPECT_EQ(l1, r1);
  auto [l2, r2] = s.equal_range(4);
  EXPECT_EQ(*l2, 4);
  EXPECT_EQ(*r2, 5);
  EXPECT_EQ(++l2, r2);
}

TEST(unordered_set, erase) {
  /**
   * @test  size_type erase(const key_type& __k) 
   * @brief erase by value.
   */
  SUBTEST(erase) {
    unordered_set<int> s = {1, 4, 2, 5, 0};
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
    unordered_set<int> s = {1, 4, 2, 5, 0};
    auto it = s.erase(s.begin());
    EXPECT_EQ(*it, 1);
    EXPECT_THROW(s.erase(s.end()), std::range_error);
  }

  /**
   * @test  iterator erase(const_iterator __first, const_iterator __last) 
   * @brief erase by range.
   */
  SUBTEST(erase) {
    unordered_set<int> s = {1, 4, 2, 5, 0};
    auto it = s.erase(s.begin(), ++++++s.begin());
    EXPECT_EQ(*it, 4);
    EXPECT_STRING_EQ(s, [4, 5]);
  }
}

TEST(unordered_set, find) {
  unordered_set<int> s1 = {1, 4, 2, 5, 0};
  unordered_set<int> s2;
  for (auto x : s1) {
    if (s2.find(x) == s2.end()) {
      s2.insert(x);
    }
  }
  EXPECT_EQ(s1, s2);
}

TEST(unordered_set, get_allocator) {
  unordered_set<int> s;
  auto alloc = s.get_allocator();
}

TEST(unordered_set, hash_function) {
  unordered_set<int> s;
  auto hf = s.hash_function();
  auto hashcode = hf(123);
  EXPECT_EQ(hashcode, 123);
}

TEST(unordered_set, insert) {
  /**
   * @test  pair<iterator, bool> insert(const value_type& __x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    unordered_set<int> s;
    auto [it, ok] = s.insert(1);
    EXPECT_EQ(*it, 1);
    EXPECT_TRUE(ok);
    auto pr = s.insert(1);
    EXPECT_EQ(*pr.first, 1);
    EXPECT_FALSE(pr.second);
  }
  
  /**
   * @test  pair<iterator, bool> insert(value_type&& __x)
   * @brief insert an rvalue reference.
   */
  SUBTEST(insert) {
    unordered_set<int> s;
    s.insert(1);
    s.insert(3);
    auto [it, ok] = s.insert(1);
    EXPECT_STRING_EQ(s, [1, 3]);
    EXPECT_FALSE(ok);
  }

  /**
   * @test  iterator insert(const_iterator __hint, const value_type& __x) 
   * @brief insert an lvalue reference and using hint.
   */
  SUBTEST(insert) {
    unordered_set<int> s;
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
    unordered_set<int> s;
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
    unordered_set<int> s;
    vector<int> vc {1, 4, 2, 5, 0};
    s.insert(vc.begin(), vc.begin()+4);
    EXPECT_STRING_EQ(s, [5, 1, 2, 4]);
  }

  /**
   * @test  void insert(std::initializer_list<value_type> __l)
   * @brief insert initializer_list.
   */
  SUBTEST(insert) {
    unordered_set<int> s;
    s.insert({1, 4, 2, 5, 0});
    EXPECT_STRING_EQ(s, [0, 1, 2, 4, 5]);
  }
}

TEST(unordered_set, key_eq) {
  unordered_set<int> s;
  auto eq = s.key_eq();
  EXPECT_TRUE(eq(123, 123));
  EXPECT_FALSE(eq(123, 1234));
}

TEST(unordered_set, load_factor) {
  unordered_set<int> s {1, 4, 2, 5, 0};
  EXPECT_EQ(s.bucket_count(), 11);
  EXPECT_FLOAT_EQ(s.load_factor(), 5/11.f);
  EXPECT_FLOAT_EQ(s.max_load_factor(), 0.75F);
  s.max_load_factor(1);
  EXPECT_EQ(s.bucket_count(), 5);
  EXPECT_FLOAT_EQ(s.max_load_factor(), 1.F);
}

TEST(unordered_set, max_bucket_count) {
  unordered_set<int> s;
  EXPECT_GT(s.max_bucket_count(), 0);
}

TEST(unordered_set, max_size) {
  unordered_set<int> s;
  EXPECT_GT(s.max_size(), 0);
}

TEST(unordered_set, merge) {
  // TODO.
}

TEST(unordered_set, rehash) {
  unordered_set<int> s {1, 4, 2, 5, 0};
  s.rehash(5);
  EXPECT_EQ(s.bucket_count(), 11);
  s.rehash(21);
  EXPECT_EQ(s.bucket_count(), 23);
}

TEST(unordered_set, reserve) {
  unordered_set<int> s {1, 4, 2, 5, 0};
  s.reserve(25);
}

TEST(unordered_set, size) {
  unordered_set<int> s {1, 4, 2, 5, 0};
  EXPECT_EQ(s.size(), 5);
}

TEST(unordered_set, swap) {
  unordered_set<int> s1 {1, 4, 2, 5, 0};
  unordered_set<int> s2 {1, 4, 2};
  s1.swap(s2);
  EXPECT_STRING_EQ(s1, [1, 2, 4]);
  EXPECT_STRING_EQ(s2, [0, 1, 2, 4, 5]);
}

TEST(unordered_set, compare_operator) {
  unordered_set<int> s1 {1, 4, 2, 5, 0};
  unordered_set<int> s2 {1, 4, 2};
  unordered_set<int> s3 {1, 4, 2, 5, 0};
  EXPECT_EQ(s1, s1);
  EXPECT_EQ(s1, s3);
  EXPECT_NE(s1, s2);
}
