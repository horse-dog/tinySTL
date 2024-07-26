#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "unordered_set.h"

using namespace tinySTL;

TEST(unordered_multiset, constructor) {
  /**
   * @test  unordered_multiset()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    unordered_multiset<int> s;
    EXPECT_STRING_EQ(s, []);
  }

  /**
   * @test  unordered_multiset(const unordered_multiset&)
   * @brief copy constructor.
   */
  SUBTEST(constructor) {
    unordered_multiset<int> s1{4, 3, 4, 7, 1};
    EXPECT_STRING_EQ(s1, [1, 3, 4, 4, 7]);
    unordered_multiset<int> s2(s1);
    EXPECT_STRING_EQ(s2, [1, 3, 4, 4, 7]);
  }

  /**
   * @test  unordered_multiset(unordered_multiset&&)
   * @brief move constructor.
   */
  SUBTEST(constructor) {
    unordered_multiset<int> s1{4, 3, 4, 7, 1};
    EXPECT_STRING_EQ(s1, [1, 3, 4, 4, 7]);
    unordered_multiset<int> s2(tinySTL::move(s1));
    EXPECT_EQ(s1.size(), 0);
    EXPECT_EQ(s2.size(), 5);
    EXPECT_STRING_EQ(s1, []);
    EXPECT_STRING_EQ(s2, [1, 3, 4, 4, 7]);
  }

  /**
   * @test  unordered_multiset(initializer_list l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    unordered_multiset<std::string> s = { "Hello", "Standard", "Hello", "Template", "Library" };
    EXPECT_STRING_EQ(s, [Hello, Hello, Library, Template, Standard]);
  }
  
  /**
   * @test  unordered_multiset(Iterator first, Iterator last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    vector<double> l = { 3.14, 6.28, 0.25, 0.618 };
    unordered_multiset<int> s(l.begin(), l.end());
    EXPECT_STRING_EQ(s, [0, 0, 3, 6]);
  }
}

TEST(unordered_multiset, assign_operator) {
  /**
   * @test  unordered_multiset& operator=(initializer_list l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    unordered_multiset<int> s = {1, 2, 3, 4};
    s = {1, 2, 3, 3, 5};
    EXPECT_STRING_EQ(s, [1, 2, 3, 3, 5]);
    EXPECT_EQ(s.size(), 5);
  }
  
  /**
   * @test  unordered_multiset& operator=(const unordered_multiset& x)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    unordered_multiset<int> s0 = {1, 2, 2, 4};
    unordered_multiset<int> s1 = {1, 2, 3, 3, 5};
    s0 = s1;
    EXPECT_STRING_EQ(s0, [1, 2, 3, 3, 5]);
    EXPECT_EQ(s0.size(), 5);
    EXPECT_STRING_EQ(s1, [1, 2, 3, 3, 5]);
    EXPECT_EQ(s1.size(), 5);
  }

  /**
   * @test  unordered_multiset& operator=(unordered_multiset&& x)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    unordered_multiset<int> s = {1, 2, 2, 4};
    s = unordered_multiset<int>({1, 2, 3, 3, 5});
    EXPECT_STRING_EQ(s, [1, 2, 3, 3, 5]);
    EXPECT_EQ(s.size(), 5);
  }
}

TEST(unordered_multiset, begin) {
  unordered_multiset<int> s {5, 4, 0, 5, 0};
  auto it = s.begin();
  EXPECT_EQ(*it, 0);
}

TEST(unordered_multiset, cbegin) {
  unordered_multiset<int> s {5, 4, 0, 5, 0};
  auto it = s.cbegin();
  EXPECT_EQ(*it, 0);
}

TEST(unordered_multiset, end) {
  unordered_multiset<int> s;
  auto end = s.end();
  s.insert(1);
  s.insert(1);
  auto it = s.begin();
  EXPECT_EQ(++++it, end);
}

TEST(unordered_multiset, cend) {
  unordered_multiset<int> s;
  auto end = s.cend();
  s.insert(1);
  s.insert(1);
  auto it = s.cbegin();
  EXPECT_EQ(++++it, end);
}

TEST(unordered_multiset, bucket) {
  unordered_multiset<int> s {1, 4, 2, 5, 1};
#define __Slot_print(x) printf("slot of "#x": %zd\n", s.bucket(x))
  __Slot_print(1);
  __Slot_print(4);
  __Slot_print(2);
  __Slot_print(5);
  __Slot_print(1);
#undef __Slot_print
}

TEST(unordered_multiset, bucket_count) {
  unordered_multiset<int> s {1, 4, 2, 5, 1, 23};
  EXPECT_EQ(s.bucket_count(), 11);
}

TEST(unordered_multiset, bucket_size) {
  unordered_multiset<int> s {1, 4, 2, 5, 1, 23};
  EXPECT_EQ(s.bucket_size(0), 0);
  EXPECT_EQ(s.bucket_size(1), 3);
}

TEST(unordered_multiset, clear) {
  unordered_multiset<int> s {1, 4, 2, 5, 1, 23};
  s.clear();
  EXPECT_STRING_EQ(s, []);
}

TEST(unordered_multiset, contains) {
  unordered_multiset<int> s = {1, 4, 2, 5, 1};
  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(2));
  EXPECT_FALSE(s.contains(3));
}

TEST(unordered_multiset, count) {
  unordered_multiset<int> s = {1, 4, 2, 5, 1};
  EXPECT_EQ(s.count(3), 0);
  EXPECT_EQ(s.count(4), 1);
  EXPECT_EQ(s.count(1), 2);
}

TEST(unordered_multiset, emplace) {
  unordered_multiset<
    pair<int, std::string>, 
    decltype([](const pair<int, std::string>& x) { 
      return std::hash<int>()(x.first); 
    }), 
    decltype([](const pair<int, std::string>& x, const pair<int, std::string>& y) { 
      return x.first == y.first; 
    })> s;
  s.emplace(1, "Hello");
  s.emplace(2, "World");
  s.emplace(1, "Fuck");
  EXPECT_STRING_EQ(s, [{1, Hello}, {1, Fuck}, {2, World}]);
}

TEST(unordered_multiset, emplace_hint) {
  unordered_multiset<
    pair<int, std::string>, 
    decltype([](const pair<int, std::string>& x) { 
      return std::hash<int>()(x.first); 
    }), 
    decltype([](const pair<int, std::string>& x, const pair<int, std::string>& y) { 
      return x.first == y.first; 
    })> s;
  auto it = s.emplace_hint(s.end(), 2, "World");
  it = s.emplace_hint(it, 1, "Hello");
  EXPECT_STRING_EQ(s, [{1, Hello}, {2, World}]);
}

TEST(unordered_multiset, empty) {
  unordered_multiset<int> s;
  EXPECT_TRUE(s.empty());
  s.insert(111);
  EXPECT_FALSE(s.empty());
  s.insert(111);
  EXPECT_EQ(s.size(), 2);
  EXPECT_FALSE(s.empty());
  s.erase(111);
  EXPECT_TRUE(s.empty());
}

TEST(unordered_multiset, equal_range) {
  unordered_multiset<int> s = {1, 0, 2, 5, 0};
  auto [l1, r1] = s.equal_range(3);
  EXPECT_EQ(l1, r1);
  auto [l2, r2] = s.equal_range(0);
  EXPECT_EQ(*l2, 0);
  EXPECT_EQ(*r2, 1);
  EXPECT_EQ(++++l2, r2);
}

TEST(unordered_multiset, erase) {
  /**
   * @test  size_type erase(const key_type& __k) 
   * @brief erase by value.
   */
  SUBTEST(erase) {
    unordered_multiset<int> s = {1, 4, 2, 5, 1};
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
    unordered_multiset<int> s = {1, 4, 2, 5, 1};
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
    unordered_multiset<int> s = {1, 4, 2, 5, 1};
    auto it = s.erase(s.begin(), ++++++s.begin());
    EXPECT_EQ(*it, 4);
    EXPECT_STRING_EQ(s, [4, 5]);
  }
}

TEST(unordered_multiset, find) {
  unordered_multiset<int> s1 = {1, 4, 2, 5, 2};
  unordered_multiset<int> s2;
  for (auto x : s1) {
    if (s2.find(x) == s2.end()) {
      s2.insert(x);
    }
  }
  EXPECT_STRING_EQ(s2, [1, 2, 4, 5]);
}

TEST(unordered_multiset, get_allocator) {
  unordered_multiset<int> s;
  auto alloc = s.get_allocator();
}

TEST(unordered_multiset, hash_function) {
  unordered_multiset<int> s;
  auto hf = s.hash_function();
  auto hashcode = hf(123);
  EXPECT_EQ(hashcode, 123);
}

TEST(unordered_multiset, insert) {
  /**
   * @test  iterator insert(const value_type& __x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    unordered_multiset<int> s;
    auto it = s.insert(1);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(s.size(), 1);
    it = s.insert(1);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(s.size(), 2);
    it = s.insert(5);
    EXPECT_EQ(*it, 5);
    EXPECT_EQ(s.size(), 3);
    EXPECT_STRING_EQ(s, [5, 1, 1]);
  }
  
  /**
   * @test  iterator insert(value_type&& __x)
   * @brief insert an rvalue reference.
   */
  SUBTEST(insert) {
    unordered_multiset<int> s;
    s.insert(1);
    s.insert(3);
    s.insert(1);
    EXPECT_STRING_EQ(s, [1, 1, 3]);
  }

  /**
   * @test  iterator insert(const_iterator __hint, const value_type& __x) 
   * @brief insert an lvalue reference and using hint.
   */
  SUBTEST(insert) {
    unordered_multiset<int> s;
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
    unordered_multiset<int> s;
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
    unordered_multiset<int> s;
    vector<int> vc {1, 2, 2, 5, 2};
    s.insert(vc.begin(), vc.begin()+4);
    EXPECT_STRING_EQ(s, [1, 2, 2, 5]);
  }

  /**
   * @test  void insert(std::initializer_list<value_type> __l)
   * @brief insert initializer_list.
   */
  SUBTEST(insert) {
    unordered_multiset<int> s;
    s.insert({1, 2, 2, 5, 2});
    EXPECT_STRING_EQ(s, [1, 2, 2, 2, 5]);
  }
}

TEST(unordered_multiset, key_eq) {
  unordered_multiset<int> s;
  auto eq = s.key_eq();
  EXPECT_TRUE(eq(123, 123));
  EXPECT_FALSE(eq(123, 1234));
}

TEST(unordered_multiset, load_factor) {
  unordered_multiset<int> s {2, 4, 2, 5, 0};
  EXPECT_EQ(s.bucket_count(), 11);
  EXPECT_FLOAT_EQ(s.load_factor(), 5/11.f);
  EXPECT_FLOAT_EQ(s.max_load_factor(), 0.75F);
  s.max_load_factor(1);
  EXPECT_EQ(s.bucket_count(), 5);
  EXPECT_FLOAT_EQ(s.max_load_factor(), 1.F);
}

TEST(unordered_multiset, max_bucket_count) {
  unordered_multiset<int> s;
  EXPECT_GT(s.max_bucket_count(), 0);
}

TEST(unordered_multiset, max_size) {
  unordered_multiset<int> s;
  EXPECT_GT(s.max_size(), 0);
}

TEST(unordered_multiset, merge) {
  /**
   * @brief merge another unordered_multiset.
   */
  SUBTEST(merge) {
    unordered_multiset<int> s1;
    unordered_multiset<int> s2 = {1, 4, 2, 5, 2};
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

    unordered_multiset<int> s3;
    s2.merge(s3);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_STRING_EQ(s3, []);
    EXPECT_EQ(s2.size(), 0);
    EXPECT_EQ(s3.size(), 0);
  }

  /**
   * @brief merge another unordered_set.
   */
  SUBTEST(merge) {
    unordered_multiset<int> s1;
    unordered_set<int> s2 = {1, 4, 2, 5, 0};
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

    unordered_set<int> s3;
    s2.merge(s3);
    EXPECT_STRING_EQ(s2, []);
    EXPECT_STRING_EQ(s3, []);
    EXPECT_EQ(s2.size(), 0);
    EXPECT_EQ(s3.size(), 0);
  }
}

TEST(unordered_multiset, rehash) {
  unordered_multiset<int> s {1, 4, 2, 5, 2};
  s.rehash(3);
  EXPECT_EQ(s.bucket_count(), 11);
  EXPECT_STRING_EQ(s, [1, 2, 2, 4, 5]);
  s.rehash(5);
  EXPECT_EQ(s.bucket_count(), 11);
  EXPECT_STRING_EQ(s, [1, 2, 2, 4, 5]);
  s.rehash(21);
  EXPECT_EQ(s.bucket_count(), 23);
  EXPECT_STRING_EQ(s, [1, 2, 2, 4, 5]);
}

TEST(unordered_multiset, reserve) {
  unordered_multiset<int> s {1, 4, 2, 5, 2};
  s.reserve(25);
  EXPECT_STRING_EQ(s, [1, 2, 2, 4, 5]);
}

TEST(unordered_multiset, size) {
  unordered_multiset<int> s {1, 4, 2, 5, 2};
  EXPECT_EQ(s.size(), 5);
}

TEST(unordered_multiset, swap) {
  unordered_multiset<int> s1 {1, 4, 2, 5, 2};
  unordered_multiset<int> s2 {2, 4, 2};
  s1.swap(s2);
  EXPECT_STRING_EQ(s1, [2, 2, 4]);
  EXPECT_STRING_EQ(s2, [1, 2, 2, 4, 5]);
}

TEST(unordered_multiset, compare_operator) {
  unordered_multiset<int> s1 {1, 4, 2, 5, 2};
  unordered_multiset<int> s2 {2, 4, 2};
  unordered_multiset<int> s3 {1, 4, 2, 5, 2};
  EXPECT_EQ(s1, s1);
  EXPECT_EQ(s1, s3);
  EXPECT_NE(s1, s2);
}