#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "unordered_map.h"

using namespace tinySTL;

TEST(unordered_map, constructor) {
  /**
   * @test  unordered_map()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    unordered_map<int, std::string> m;
    EXPECT_STRING_EQ(m, []);
  }

  /**
   * @test  unordered_map(const unordered_map&)
   * @brief copy constructor.
   */
  SUBTEST(constructor) {
    unordered_map<int, std::string> m1 {{1, "hello"}, {2, "world"}};
    EXPECT_STRING_EQ(m1, [{2, world}, {1, hello}]);
    unordered_map<int, std::string> m2(m1);
    EXPECT_STRING_EQ(m2, [{2, world}, {1, hello}]);
  }

  /**
   * @test  unordered_map(unordered_map&&)
   * @brief move constructor.
   */
  SUBTEST(constructor) {
    unordered_map<int, std::string> m1 {{1, "hello"}, {2, "world"}};
    EXPECT_STRING_EQ(m1, [{2, world}, {1, hello}]);
    unordered_map<int, std::string> m2(tinySTL::move(m1));
    EXPECT_EQ(m1.size(), 0);
    EXPECT_EQ(m2.size(), 2);
    EXPECT_STRING_EQ(m1, []);
    EXPECT_STRING_EQ(m2, [{2, world}, {1, hello}]);
  }

  /**
   * @test  unordered_map(std::initializer_list __l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    unordered_map<int, std::string> m = { {1, "Hello"}, {2, "Standard"}, {3, "Template"}, {4, "Library"} };
    EXPECT_STRING_EQ(m, [{1, Hello}, {2, Standard}, {3, Template}, {4, Library}]);
  }

  /**
   * @test  unordered_map(Iterator __first, Iterator __last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    vector<tinySTL::pair<int, std::string>>
    l = {{1, "Hello"}, {2, "Standard"}, {3, "Template"}, {4, "Library"}};
    unordered_map<int, std::string> m(l.begin(), l.end());
    EXPECT_STRING_EQ(m, [{1, Hello}, {2, Standard}, {3, Template}, {4, Library}]);
  }
}

TEST(unordered_map, assign_operator) {
  /**
   * @test  unordered_map& operator=(initializer_list l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    unordered_map<int, int> m = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    m = {{1, 11}, {2, 22}, {3, 33}};
    EXPECT_STRING_EQ(m, [{1, 11}, {2, 22}, {3, 33}]);
    EXPECT_EQ(m.size(), 3);
  }

  /**
   * @test  unordered_map& operator=(const unordered_map& x)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    unordered_map<int, int> m0 = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    unordered_map<int, int> m1 = {{1, 11}, {2, 22}, {3, 33}};
    m0 = m1;
    EXPECT_STRING_EQ(m0, [{1, 11}, {2, 22}, {3, 33}]);
    EXPECT_EQ(m0.size(), 3);
    EXPECT_STRING_EQ(m1, [{1, 11}, {2, 22}, {3, 33}]);
    EXPECT_EQ(m1.size(), 3);
  }

  /**
   * @test  unordered_map& operator=(unordered_map&& x)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    unordered_map<int, int> m = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    m = unordered_map<int, int>({{1, 11}, {2, 22}, {3, 33}});
    EXPECT_STRING_EQ(m, [{1, 11}, {2, 22}, {3, 33}]);
    EXPECT_EQ(m.size(), 3);
  }
}

TEST(unordered_map, begin) {
  unordered_map<int, std::string> 
  m {{1, "Hello"}, {2, "STL"}, {3, "World"}};
  auto it = m.begin();
  EXPECT_EQ(it->first, 1);
  EXPECT_EQ(it->second, "Hello");
  it->second = "Fuck";
  EXPECT_STRING_EQ(m, [{1, Fuck}, {2, STL}, {3, World}]);
}

TEST(unordered_map, cbegin) {
  unordered_map<int, std::string> 
  m {{1, "Hello"}, {2, "STL"}, {3, "World"}};
  auto it = m.cbegin();
  EXPECT_EQ(it->first, 1);
  EXPECT_EQ(it->second, "Hello");
}

TEST(unordered_map, end) {
  unordered_map<int, std::string> m;
  EXPECT_EQ(m.begin(), m.end());
  m.insert({0, "Hello"});
  EXPECT_EQ(++m.begin(), m.end());
}

TEST(unordered_map, cend) {
  unordered_map<int, std::string> m;
  EXPECT_EQ(m.cbegin(), m.cend());
  m.insert({0, "Hello"});
  EXPECT_EQ(++m.cbegin(), m.cend());
}

TEST(unordered_map, bucket) {
  unordered_map<int, int> 
  m {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}, {23,1}};
#define __Slot_print(x) printf("slot of "#x": %zd\n", m.bucket(x))
  __Slot_print(1);
  __Slot_print(4);
  __Slot_print(2);
  __Slot_print(5);
  __Slot_print(0);
  __Slot_print(12);
#undef __Slot_print
}

TEST(unordered_map, bucket_count) {
  unordered_map<int, int> 
  m {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}, {23,1}};
  EXPECT_EQ(m.bucket_count(), 11);
}

TEST(unordered_map, bucket_size) {
  unordered_map<int, int> 
  m {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}, {23,1}};
  EXPECT_EQ(m.bucket_size(0), 1);
  EXPECT_EQ(m.bucket_size(1), 2);
}

TEST(unordered_map, clear) {
  unordered_map<int, int> 
  m {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}, {23,1}};
  m.clear();
  EXPECT_STRING_EQ(m, []);
}

TEST(unordered_map, contains) {
  unordered_map<int, int> 
  m {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}, {23,1}};
  EXPECT_TRUE(m.contains(2));
  EXPECT_FALSE(m.contains(3));
}

TEST(unordered_map, count) {
  unordered_map<int, int> 
  m {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}, {23,1}};
  EXPECT_EQ(m.count(3), 0);
  EXPECT_EQ(m.count(4), 1);
}

TEST(unordered_map, emplace) {
  unordered_map<int, std::string> m;
  m.emplace(1, "Hello");
  m.emplace(2, "World");
  EXPECT_STRING_EQ(m, [{2, World}, {1, Hello}]);
}

TEST(unordered_map, emplace_hint) {
  unordered_map<int, std::string> m;
  auto it = m.emplace_hint(m.end(), 2, "World");
  it = m.emplace_hint(it, 1, "Hello");
  EXPECT_STRING_EQ(m, [{2, World}, {1, Hello}]);
}

TEST(unordered_map, empty) {
  unordered_map<int, std::string> m;
  EXPECT_TRUE(m.empty());
  m.insert({1, "hello"});
  EXPECT_FALSE(m.empty());
  m.erase(1);
  EXPECT_TRUE(m.empty());
}

TEST(unordered_map, equal_range) {
  unordered_map<int, int> m {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
  auto [l1, r1] = m.equal_range(3);
  EXPECT_EQ(l1, r1);
  auto [l2, r2] = m.equal_range(4);
  EXPECT_EQ(l2->first, 4);
  EXPECT_EQ(r2->first, 5);
  EXPECT_EQ(++l2, r2);
}

TEST(unordered_map, erase) {
  /**
   * @test  size_type erase(const key_type& __k) 
   * @brief erase by key.
   */
  SUBTEST(erase) {
    unordered_map<int, std::string> 
    m = {{1, "Hello"}, {4, "World"}, {2, "STL"}, {5, "Project"}, {0, "Fuck"}};
    EXPECT_EQ(m.erase(4), 1);
    EXPECT_EQ(m.erase(1), 1);
    EXPECT_EQ(m.erase(1), 0);
    EXPECT_STRING_EQ(m, [{0, Fuck}, {2, STL}, {5, Project}]);
  }

  /**
   * @test  iterator erase(const_iterator __position) 
   * @brief erase by position.
   */
  SUBTEST(erase) {
    unordered_map<int, std::string> m;
    EXPECT_THROW(m.erase(m.begin()), std::range_error);
    EXPECT_THROW(m.erase(m.end()), std::range_error);
    m = {{1, "Hello"}, {4, "World"}, {2, "STL"}, {5, "Project"}, {0, "Fuck"}};
    auto it = m.erase(m.begin());
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, "Hello");
    EXPECT_THROW(m.erase(m.end()), std::range_error);
  }

  /**
   * @test  iterator erase(const_iterator __first, const_iterator __last) 
   * @brief erase by range.
   */
  SUBTEST(erase) {
    unordered_map<int, std::string> m;
    m = {{1, "Hello"}, {4, "World"}, {2, "STL"}, {5, "Project"}, {0, "Fuck"}};
    auto it = m.erase(m.begin(), ++++++m.begin());
    EXPECT_EQ(it->first, 4);
    EXPECT_STRING_EQ(m, [{4, World}, {5, Project}]);
  }
}

TEST(unordered_map, find) {
  unordered_map<int, int> m1 {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
  unordered_map<int, int> m2;
  for (auto&& pr : m1) {
    if (m2.find(pr.first) == m2.end()) {
      m2.insert(pr);
    }
  }
  EXPECT_EQ(m1, m2);
}

TEST(unordered_map, get_allocator) {
  unordered_map<int, std::string> m;
  auto alloc = m.get_allocator();
  static_assert(
    tinySTL::is_same_v<decltype(alloc.allocate(1)), tinySTL::pair<int, std::string>*>, 
    "alloc doesn't match pair<int, std::string>*"
  );
}

TEST(unordered_map, hash_function) {
  unordered_map<int, int> s;
  auto hf = s.hash_function();
  auto hashcode = hf(123);
  EXPECT_EQ(hashcode, 123);
}

TEST(unordered_map, insert) {
  /**
   * @test  tinySTL::pair<iterator, bool> insert(const value_type& __x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    unordered_map<int, std::string> m;
    tinySTL::pair<int, std::string> value {1, "Hello"};
    auto [it, ok] = m.insert(value);
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, "Hello");
    EXPECT_TRUE(ok);
    value.second = "World";
    auto pr = m.insert(value);
    EXPECT_EQ(pr.first->first, 1);
    EXPECT_EQ(pr.first->second, "Hello");
    EXPECT_FALSE(pr.second);
  }

  /**
   * @test  tinySTL::pair<iterator, bool> insert(value_type&& __x)
   * @brief insert an rvalue reference.
   */
  SUBTEST(insert) {
    unordered_map<int, std::string> m;
    pair<int, std::string> node {1, "Hello"};
    EXPECT_EQ(node.second, "Hello");
    m.insert(tinySTL::move(node));
    EXPECT_EQ(node.second, "");
    EXPECT_STRING_EQ(m, [{1, Hello}]);
    auto [it, ok] = m.insert({1, "xx"});
    EXPECT_FALSE(ok);
    EXPECT_STRING_EQ(m, [{1, Hello}]);
  }

  /**
   * @test  iterator insert(const_iterator __hint, const value_type& __x) 
   * @brief insert an lvalue reference and using hint.
   */
  SUBTEST(insert) {
    unordered_map<int, int> m;
    pair<int, int> p {5, 5};
    --p.first;
    m.insert(m.begin(), p);
    --p.first;
    m.insert(m.begin(), p);
    --p.first;
    m.insert(m.begin(), p);
    EXPECT_STRING_EQ(m, [{2, 5}, {3, 5}, {4, 5}]);
  }

  /**
   * @test  iterator insert(const_iterator __hint, value_type&& __x)
   * @brief insert an rvalue reference and using hint.
   */
  SUBTEST(insert) {
    unordered_map<int, int> m;
    int x = 5;
    m.insert(m.begin(), {--x, 5});
    m.insert(m.begin(), {--x, 5});
    m.insert(m.begin(), {--x, 5});
    EXPECT_STRING_EQ(m, [{2, 5}, {3, 5}, {4, 5}]);
  }

  /**
   * @test  void insert(_InputIterator __first, _InputIterator __last)
   * @brief range insert.
   */
  SUBTEST(insert) {
    unordered_map<int, int> m;
    vector<pair<int, int>> l {{1, 1}, {2, 2}, {3, 3}};
    m.insert(l.begin(), l.end());
    EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {3, 3}]);
  }

  /**
   * @test  void insert(std::initializer_list<value_type> __l)
   * @brief insert initializer_list.
   */
  SUBTEST(insert) {
    unordered_map<int, int> m {{11, 1}, {111, 2}};
    m.insert({{1, 1}, {4, 4}, {2, 2}});
    EXPECT_STRING_EQ(m, [{11, 1}, {111, 2}, {1, 1}, {2, 2}, {4, 4}]);
  }
}

TEST(unordered_map, key_eq) {
  unordered_map<int, int> m;
  auto eq = m.key_eq();
  EXPECT_TRUE(eq(123, 123));
  EXPECT_FALSE(eq(123, 1234));
}

TEST(unordered_map, load_factor) {
  unordered_map<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
  EXPECT_EQ(m.bucket_count(), 11);
  EXPECT_FLOAT_EQ(m.load_factor(), 5/11.f);
  EXPECT_FLOAT_EQ(m.max_load_factor(), 0.75F);
  m.max_load_factor(1);
  EXPECT_EQ(m.bucket_count(), 5);
  EXPECT_FLOAT_EQ(m.max_load_factor(), 1.F);
}

TEST(unordered_map, max_bucket_count) {
  unordered_map<int, int> m;
  EXPECT_GT(m.max_bucket_count(), 0);
}

TEST(unordered_map, max_size) {
  unordered_map<int, int> m;
  EXPECT_GT(m.max_size(), 0);
}

TEST(unordered_map, merge) {
  /**
   * @brief merge another unordered_map.
   */
  SUBTEST(merge) {
    unordered_map<int, int> m1;
    unordered_map<int, int> m2 = {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_EQ(m1.size(), 5);
    EXPECT_EQ(m2.size(), 0);

    m2 = {{3,3}, {4,4}, {5,5}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, [{4, 4}, {5, 5}]);
    EXPECT_EQ(m1.size(), 6);
    EXPECT_EQ(m2.size(), 2);

    unordered_map<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, [{4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 2);
    EXPECT_EQ(m3.size(), 0);
  }

  /**
   * @brief merge another unordered_multimap.
   */
  SUBTEST(merge) {
    unordered_map<int, int> m1;
    unordered_multimap<int, int> m2 = {{1,1}, {1,1}, {4,4}, {2,2}, {2,2}, {5,5}, {0,0}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, [{2, 2}, {1, 1}]);
    EXPECT_EQ(m1.size(), 5);
    EXPECT_EQ(m2.size(), 2);

    m2 = {{3,3}, {4,4}, {4,4}, {5,5}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, [{5, 5}, {4, 4}, {4, 4}]);
    EXPECT_EQ(m1.size(), 6);
    EXPECT_EQ(m2.size(), 3);

    unordered_map<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, [{5, 5}, {4, 4}, {4, 4}]);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 3);
    EXPECT_EQ(m3.size(), 0);
  }
}

TEST(unordered_map, rehash) {
  unordered_map<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
  m.rehash(3);
  EXPECT_EQ(m.bucket_count(), 11);
  EXPECT_STRING_EQ(m, [{0, 0}, {1, 1}, {2, 2}, {4, 4}, {5, 5}]);
  m.rehash(5);
  EXPECT_EQ(m.bucket_count(), 11);
  EXPECT_STRING_EQ(m, [{0, 0}, {1, 1}, {2, 2}, {4, 4}, {5, 5}]);
  m.rehash(21);
  EXPECT_EQ(m.bucket_count(), 23);
  EXPECT_STRING_EQ(m, [{0, 0}, {1, 1}, {2, 2}, {4, 4}, {5, 5}]);
}

TEST(unordered_map, reserve) {
  unordered_map<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
  m.reserve(25);
  EXPECT_STRING_EQ(m, [{0, 0}, {1, 1}, {2, 2}, {4, 4}, {5, 5}]);
}

TEST(unordered_map, size) {
  unordered_map<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
  EXPECT_EQ(m.size(), 5);
}

TEST(unordered_map, swap) {
  unordered_map<int, std::string> m1, m2 {{1, "Hello"}, {2, "STL"}, {3, "World"}};
  m1.swap(m2);
  EXPECT_STRING_EQ(m1, [{1, Hello}, {2, STL}, {3, World}]);
  EXPECT_STRING_EQ(m2, []);
  m2 = {{1, "aa"}, {3, "bb"}};
  m1.swap(m2);
  EXPECT_STRING_EQ(m1, [{3, bb}, {1, aa}]);
  EXPECT_STRING_EQ(m2, [{1, Hello}, {2, STL}, {3, World}]);
}

TEST(unordered_map, compare_operator) {
  unordered_map<int, int> m1 {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
  unordered_map<int, int> m2 {{1,1}, {4,4}, {2,2}};
  unordered_map<int, int> m3 {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
  EXPECT_EQ(m1, m1);
  EXPECT_EQ(m1, m3);
  EXPECT_NE(m1, m2);
}

TEST(unordered_map, at_operator) {
  const unordered_map<int, std::string> m = {{1, "Hello"}};
  EXPECT_EQ(m[1], "Hello");
  EXPECT_THROW(m[2], std::range_error);
  unordered_map<int, std::string> m1 = m;
  m1[2] = "World";
  EXPECT_EQ(m1[2], "World");
}
