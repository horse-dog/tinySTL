#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "unordered_map.h"

using namespace tinySTL;

TEST(unordered_multimap, constructor) {
  /**
   * @test  unordered_multimap()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    unordered_multimap<int, int> m;
    EXPECT_STRING_EQ(m, []);
  }

  /**
   * @test  unordered_multimap(const unordered_multimap&)
   * @brief copy constructor.
   */
  SUBTEST(constructor) {
    unordered_multimap<int, int> m1{{4,4}, {3,3}, {4,4}, {7,7}, {1,1}};
    EXPECT_STRING_EQ(m1, [{1, 1}, {3, 3}, {4, 4}, {4, 4}, {7, 7}]);
    unordered_multimap<int, int> m2(m1);
    EXPECT_STRING_EQ(m2, [{1, 1}, {3, 3}, {4, 4}, {4, 4}, {7, 7}]);
  }

  /**
   * @test  unordered_multimap(unordered_multimap&&)
   * @brief move constructor.
   */
  SUBTEST(constructor) {
    unordered_multimap<int, int> m1{{4,4}, {3,3}, {4,4}, {7,7}, {1,1}};
    EXPECT_STRING_EQ(m1, [{1, 1}, {3, 3}, {4, 4}, {4, 4}, {7, 7}]);
    unordered_multimap<int, int> m2(tinySTL::move(m1));
    EXPECT_EQ(m1.size(), 0);
    EXPECT_EQ(m2.size(), 5);
    EXPECT_STRING_EQ(m1, []);
    EXPECT_STRING_EQ(m2, [{1, 1}, {3, 3}, {4, 4}, {4, 4}, {7, 7}]);
  }

  /**
   * @test  unordered_multimap(std::initializer_list<value_type> __l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    unordered_multimap<int, std::string> m {{1, "Hello"}, {1, "World"}, {2, "STL"}};
    EXPECT_STRING_EQ(m, [{1, Hello}, {1, World}, {2, STL}]);
  }

  /**
   * @test  unordered_multimap(Iterator __first, Iterator __last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    vector<tinySTL::pair<int, int>> l {{1, 1}, {3, 3}, {3, 3}, {0, 0}};
    unordered_multimap<int, int> m(l.begin(), l.end());
    EXPECT_STRING_EQ(m, [{0, 0}, {1, 1}, {3, 3}, {3, 3}]);
  }
}

TEST(unordered_multimap, assign_operator) {
  /**
   * @test  unordered_multimap& operator=(std::initializer_list<value_type> __l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    unordered_multimap<int, int> m {{1,1}, {2,2}, {3,3}, {4,4}};
    m = {{1,1}, {2,2}, {3,3}, {3,3}, {5,5}};
    EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {3, 3}, {3, 3}, {5, 5}]);
    EXPECT_EQ(m.size(), 5);
  }

  /**
   * @test  unordered_multimap& operator=(const unordered_multimap&)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    unordered_multimap<int, int> m0 {{1,1}, {2,2}, {2,2}, {4,4}};
    unordered_multimap<int, int> m1 {{1,1}, {2,2}, {3,3}, {3,3}, {5,5}};
    m0 = m1;
    EXPECT_STRING_EQ(m0, [{1, 1}, {2, 2}, {3, 3}, {3, 3}, {5, 5}]);
    EXPECT_EQ(m0.size(), 5);
    EXPECT_STRING_EQ(m1, [{1, 1}, {2, 2}, {3, 3}, {3, 3}, {5, 5}]);
    EXPECT_EQ(m1.size(), 5);
  }

  /**
   * @test  unordered_multimap& operator=(unordered_multimap&&)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    unordered_multimap<int, int> m0 {{1,1}, {2,2}, {2,2}, {4,4}};
    unordered_multimap<int, int> m1 {{1,1}, {2,2}, {3,3}, {3,3}, {5,5}};
    m0 = tinySTL::move(m1);
    EXPECT_STRING_EQ(m0, [{1, 1}, {2, 2}, {3, 3}, {3, 3}, {5, 5}]);
    EXPECT_EQ(m0.size(), 5);
    EXPECT_STRING_EQ(m1, []);
    EXPECT_EQ(m1.size(), 0);
  }
}

TEST(unordered_multimap, begin) {
  unordered_multimap<int, int> m {{5,5}, {4,4}, {0,0}, {5,5}, {0,0}};
  auto it = m.begin();
  EXPECT_EQ(it->first, 0);
  it->second = 111;
  EXPECT_STRING_EQ(m, [{0, 111}, {0, 0}, {4, 4}, {5, 5}, {5, 5}]);
}

TEST(unordered_multimap, cbegin) {
  unordered_multimap<int, int> m {{5,5}, {4,4}, {0,0}, {5,5}, {0,0}};
  auto it = m.cbegin();
  EXPECT_EQ(it->first, 0);
  EXPECT_STRING_EQ(m, [{0, 0}, {0, 0}, {4, 4}, {5, 5}, {5, 5}]);
}

TEST(unordered_multimap, end) {
  unordered_multimap<int, int> m;
  EXPECT_EQ(m.begin(), m.end());
  m = {{0,0}, {5,5}, {0,0}};
  EXPECT_EQ(++++++m.begin(), m.end());
}

TEST(unordered_multimap, cend) {
  unordered_multimap<int, int> m;
  EXPECT_EQ(m.cbegin(), m.cend());
  m = {{0,0}, {5,5}, {0,0}};
  EXPECT_EQ(++++++m.cbegin(), m.cend());
}

TEST(unordered_multimap, bucket) {
  unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {1,1}};
#define __Slot_print(x) printf("slot of "#x": %zd\n", m.bucket(x))
  __Slot_print(1);
  __Slot_print(4);
  __Slot_print(2);
  __Slot_print(5);
  __Slot_print(1);
#undef __Slot_print
}

TEST(unordered_multimap, bucket_count) {
  unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {1,1}, {23,23}};
  EXPECT_EQ(m.bucket_count(), 11);
}

TEST(unordered_multimap, bucket_size) {
  unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {1,1}, {23,23}};
  EXPECT_EQ(m.bucket_size(0), 0);
  EXPECT_EQ(m.bucket_size(1), 3);
}

TEST(unordered_multimap, clear) {
  unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {4,42}, {0,0}};
  m.clear();
  EXPECT_EQ(m.size(), 0);
  EXPECT_STRING_EQ(m, []);
}

TEST(unordered_multimap, contains) {
  unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {2,21}};
  EXPECT_TRUE(m.contains(2));
  EXPECT_FALSE(m.contains(3));
}

TEST(unordered_multimap, count) {
  unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {2,21}};
  EXPECT_EQ(m.count(3), 0);
  EXPECT_EQ(m.count(2), 2);
}

TEST(unordered_multimap, emplace) {
  unordered_multimap<int, std::string> m;
  m.emplace(1, "Hello");
  m.emplace(2, "World");
  m.emplace(1, "Fuck");
  EXPECT_STRING_EQ(m, [{1, Hello}, {1, Fuck}, {2, World}]);
}

TEST(unordered_multimap, emplace_hint) {
  unordered_multimap<int, std::string> m;
  auto it = m.emplace_hint(m.end(), 2, "World");
  it = m.emplace_hint(it, 1, "Hello");
  it = m.emplace_hint(it, 1, "Fuck");
  EXPECT_STRING_EQ(m, [{1, Fuck}, {1, Hello}, {2, World}]);
}

TEST(unordered_multimap, empty) {
  unordered_multimap<int, int> m;
  EXPECT_TRUE(m.empty());
  m.insert({111, 1});
  m.insert({111, 11});
  m.insert({111, 111});
  EXPECT_FALSE(m.empty());
  m.insert({123, 1});
  EXPECT_FALSE(m.empty());
  m.erase(111);
  EXPECT_FALSE(m.empty());
  m.erase(123);
  EXPECT_TRUE(m.empty());
}

TEST(unordered_multimap, equal_range) {
  unordered_multimap<int, int> m {{1,1}, {0,0}, {2,2}, {5,5}, {0,0}};
  auto [l1, r1] = m.equal_range(3);
  EXPECT_EQ(l1, r1);
  auto [l2, r2] = m.equal_range(0);
  EXPECT_EQ(l2->first, 0);
  EXPECT_EQ(r2->first, 1);
  EXPECT_EQ(++++l2, r2);
}

TEST(unordered_multimap, erase) {
  /**
   * @test  size_type erase(const key_type& __k)
   * @brief erase by value.
   */
  SUBTEST(erase) {
    unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {1,11}};
    EXPECT_EQ(m.erase(4), 1);
    EXPECT_EQ(m.size(), 4);
    EXPECT_EQ(m.erase(1), 2);
    EXPECT_EQ(m.size(), 2);
    EXPECT_EQ(m.erase(1), 0);
    EXPECT_EQ(m.size(), 2);
    EXPECT_STRING_EQ(m, [{2, 2}, {5, 5}]);
  }

  /**
   * @test  iterator erase(const_iterator __position) 
   * @brief erase by position.
   */
  SUBTEST(erase) {
    unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {1,10}};
    auto it = m.erase(m.begin());
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, 10);
    EXPECT_EQ(m.size(), 4);
    EXPECT_THROW(m.erase(m.end()), std::range_error);
  }

  /**
   * @test  iterator erase(const_iterator __first, const_iterator __last)
   * @brief erase by range.
   */
  SUBTEST(erase) {
    unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {1,10}};
    auto it = m.erase(m.begin(), ++++++m.begin());
    EXPECT_EQ(it->first, 4);
    EXPECT_EQ(it->second, 4);
    EXPECT_STRING_EQ(m, [{4, 4}, {5, 5}]);
  }
}

TEST(unordered_multimap, find) {
  unordered_multimap<int, int> m1 {{1,1}, {4,4}, {2,2}, {5,5}, {2,21}};
  unordered_multimap<int, int> m2;
  for (auto&& [k, v] : m1) {
    if (m2.find(k) == m2.end()) {
      m2.emplace(k, v);
    }
  }
  EXPECT_STRING_EQ(m2, [{1, 1}, {2, 2}, {4, 4}, {5, 5}]);
  EXPECT_EQ(m2.bucket_count(), 11);
}

TEST(unordered_multimap, get_allocator) {
  unordered_multimap<int, int> m;
  auto alloc = m.get_allocator();
}

TEST(unordered_multimap, hash_function) {
  unordered_multimap<int, int> m;
  auto hf = m.hash_function();
  auto hashcode = hf(123);
  EXPECT_EQ(hashcode, 123);
}

TEST(unordered_multimap, insert) {
  /**
   * @test  iterator insert(const value_type& __x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    unordered_multimap<int, int> m;
    tinySTL::pair<int, int> value {1, 111};
    auto it = m.insert(value);
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, 111);
    EXPECT_EQ(m.size(), 1);
    value.second = 112;
    it = m.insert(value);
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, 112);
    EXPECT_EQ(m.size(), 2);
    value = {5, 520};
    it = m.insert(value);
    EXPECT_EQ(it->first, 5);
    EXPECT_EQ(it->second, 520);
    EXPECT_EQ(m.size(), 3);
    EXPECT_STRING_EQ(m, [{5, 520}, {1, 111}, {1, 112}]);
  }

  /**
   * @test  iterator insert(value_type&& __x)
   * @brief insert an rvalue reference.
   */
  SUBTEST(insert) {
    unordered_multimap<int, vector<int>> m;
    pair node = {1, vector{1, 10, 100}};
    EXPECT_STRING_EQ(node.second, [1, 10, 100]);
    m.insert(tinySTL::move(node));
    EXPECT_STRING_EQ(node.second, []);
    EXPECT_TRUE(node.second.empty());
    node.second = {2, 20, 200};
    auto it = m.insert(tinySTL::move(node));
    EXPECT_STRING_EQ(it->second, [2, 20, 200]);
    EXPECT_STRING_EQ(node.second, []);
    EXPECT_EQ(m.size(), 2);
    EXPECT_EQ(node.second.size(), 0);
  }

  /**
   * @test  iterator insert(const_iterator __hint, const value_type& __x)
   * @brief insert an lvalue reference and using hint.
   */
  SUBTEST(insert) {
    unordered_multimap<int, int> m;
    pair node {5, 5};
    --node.first;
    m.insert(m.begin(), node);
    --node.first;
    m.insert(m.begin(), node);
    --node.first;
    m.insert(m.begin(), node);
    --node.second;
    m.insert(m.begin(), node);
    EXPECT_STRING_EQ(m, [{2, 4}, {2, 5}, {3, 5}, {4, 5}]);
  }

  /**
   * @test  iterator insert(const_iterator __hint, value_type&& __x)
   * @brief insert an rvalue reference and using hint.
   */
  SUBTEST(insert) {
    unordered_multimap<int, int> m;
    m.insert(m.begin(), {4, 4});
    m.insert(m.begin(), {3, 3});
    m.insert(m.begin(), {2, 2});
    m.insert(m.begin(), {2, 2});
    EXPECT_STRING_EQ(m, [{2, 2}, {2, 2}, {3, 3}, {4, 4}]);
  }

  /**
   * @test  void insert(_InputIterator __first, _InputIterator __last) 
   * @brief range insert.
   */
  SUBTEST(insert) {
    unordered_multimap<int, int> m;
    std::vector<pair<int, int>> vc {{1,1}, {2,2}, {2,2}, {5,5}, {2,2}};
    m.insert(vc.begin(), vc.begin()+4);
    EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {2, 2}, {5, 5}]);
  }

  /**
   * @test  void insert(std::initializer_list<value_type> __l)
   * @brief insert initializer_list.
   */
  SUBTEST(insert) {
    unordered_multimap<int, int> m;
    m.insert({{1,1}, {2,2}, {2,2}, {5,5}, {2,2}});
    EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {2, 2}, {2, 2}, {5, 5}]);
  }
}

TEST(unordered_multimap, key_eq) {
  unordered_multimap<int, int> m;
  auto eq = m.key_eq();
  EXPECT_TRUE(eq(123, 123));
  EXPECT_FALSE(eq(123, 1234));
}

TEST(unordered_multimap, load_factor) {
  unordered_multimap<int, int> m {{2,2}, {4,4}, {2,2}, {5,5}, {0,0}};
  EXPECT_EQ(m.bucket_count(), 11);
  EXPECT_FLOAT_EQ(m.load_factor(), 5/11.f);
  EXPECT_FLOAT_EQ(m.max_load_factor(), 0.75F);
  m.max_load_factor(1);
  EXPECT_EQ(m.bucket_count(), 5);
  EXPECT_FLOAT_EQ(m.max_load_factor(), 1.F);
}

TEST(unordered_multimap, max_bucket_count) {
  unordered_multimap<int, int> m;
  EXPECT_GT(m.max_bucket_count(), 0);
}

TEST(unordered_multimap, max_size) {
  unordered_multimap<int, int> m;
  EXPECT_GT(m.max_size(), 0);
}

TEST(unordered_multimap, merge) {
  /**
   * @brief merge another unordered_multimap.
   */
  SUBTEST(merge) {
    unordered_multimap<int, int> m1;
    unordered_multimap<int, int> m2 = {{1,1}, {4,4}, {2,2}, {5,5}, {2,2}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{1, 1}, {2, 2}, {2, 2}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_EQ(m1.size(), 5);
    EXPECT_EQ(m2.size(), 0);

    m2 = {{3,3}, {4,4}, {3,3}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{1, 1}, {2, 2}, {2, 2}, {3, 3}, {3, 3}, {4, 4}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_EQ(m1.size(), 8);
    EXPECT_EQ(m2.size(), 0);

    unordered_multimap<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 0);
    EXPECT_EQ(m3.size(), 0);
  }

  /**
   * @brief merge another unordered_map.
   */
  SUBTEST(merge) {
    unordered_multimap<int, int> m1;
    unordered_map<int, int> m2 = {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_EQ(m1.size(), 5);
    EXPECT_EQ(m2.size(), 0);

    m2 = {{3,3}, {4,4}, {5,5}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {4, 4}, {5, 5}, {5, 5}]);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_EQ(m1.size(), 8);
    EXPECT_EQ(m2.size(), 0);

    unordered_map<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 0);
    EXPECT_EQ(m3.size(), 0);
  }
}

TEST(unordered_multimap, rehash) {
  unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {2,2}};
  m.rehash(3);
  EXPECT_EQ(m.bucket_count(), 11);
  EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {2, 2}, {4, 4}, {5, 5}]);
  m.rehash(5);
  EXPECT_EQ(m.bucket_count(), 11);
  EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {2, 2}, {4, 4}, {5, 5}]);
  m.rehash(21);
  EXPECT_EQ(m.bucket_count(), 23);
  EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {2, 2}, {4, 4}, {5, 5}]);
}

TEST(unordered_multimap, reserve) {
  unordered_multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {2,2}};
  m.reserve(25);
  EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {2, 2}, {4, 4}, {5, 5}]);
}

TEST(unordered_multimap, size) {
  unordered_multimap<int, int> m;
  EXPECT_EQ(m.size(), 0);
  m.insert({111, 1});
  m.insert({111, 11});
  m.insert({111, 111});
  EXPECT_EQ(m.size(), 3);
  m.insert({123, 1});
  EXPECT_EQ(m.size(), 4);
  m.erase(111);
  EXPECT_EQ(m.size(), 1);
  m.erase(123);
  EXPECT_EQ(m.size(), 0);
}

TEST(unordered_multimap, swap) {
  unordered_multimap<int, int> m1, m2 {{1,1}, {2,2}, {2,2}, {1,1}, {0,0}};
  m1.swap(m2);
  EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {1, 1}, {2, 2}, {2, 2}]);
  EXPECT_STRING_EQ(m2, []);
  m2 = {{111,111}, {111,111}, {333,333}, {111,111}};
  m1.swap(m2);
  EXPECT_STRING_EQ(m1, [{111, 111}, {111, 111}, {111, 111}, {333, 333}]);
  EXPECT_STRING_EQ(m2, [{0, 0}, {1, 1}, {1, 1}, {2, 2}, {2, 2}]);
}

TEST(unordered_multimap, comp_operator) {
  unordered_multimap<int, int> m1 {{1,1}, {4,4}, {2,2}, {5,5}, {2,2}};
  unordered_multimap<int, int> m2 {{2,2}, {4,4}, {2,2}};
  unordered_multimap<int, int> m3 {{1,1}, {4,4}, {2,2}, {5,5}, {2,2}};
  EXPECT_EQ(m1, m1);
  EXPECT_EQ(m1, m3);
  EXPECT_NE(m1, m2);
}
