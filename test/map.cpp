#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "map.h"
#include "list.h"

using namespace tinySTL;

TEST(map, constructor) {
  /**
   * @test  map()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    map<int, std::string> m;
    EXPECT_STRING_EQ(m, []);
  }

  /**
   * @test  map(const map&)
   * @brief copy constructor.
   */
  SUBTEST(constructor) {
    map<int, std::string> m1 {{1, "hello"}, {2, "world"}};
    EXPECT_STRING_EQ(m1, [{1, hello}, {2, world}]);
    map<int, std::string> m2(m1);
    EXPECT_STRING_EQ(m2, [{1, hello}, {2, world}]);
  }

  /**
   * @test  map(map&&)
   * @brief move constructor.
   */
  SUBTEST(constructor) {
    map<int, std::string> m1 {{1, "hello"}, {2, "world"}};
    EXPECT_STRING_EQ(m1, [{1, hello}, {2, world}]);
    map<int, std::string> m2(tinySTL::move(m1));
    EXPECT_EQ(m1.size(), 0);
    EXPECT_EQ(m2.size(), 2);
    EXPECT_STRING_EQ(m1, []);
    EXPECT_STRING_EQ(m2, [{1, hello}, {2, world}]);
  }

  /**
   * @test  map(std::initializer_list __l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    map<int, std::string> m = { {1, "Hello"}, {2, "Standard"}, {3, "Template"}, {4, "Library"} };
    EXPECT_STRING_EQ(m, [{1, Hello}, {2, Standard}, {3, Template}, {4, Library}]);
  }

  /**
   * @test  map(Iterator __first, Iterator __last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    list<tinySTL::pair<int, std::string>>
    l = {{1, "Hello"}, {2, "Standard"}, {3, "Template"}, {4, "Library"}};
    map<int, std::string> m(l.begin(), l.end());
    EXPECT_STRING_EQ(m, [{1, Hello}, {2, Standard}, {3, Template}, {4, Library}]);
  }
}

TEST(map, assign_operator) {
  /**
   * @test map& operator=(initializer_list l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    map<int, int> m = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    m = {{1, 11}, {2, 22}, {3, 33}};
    EXPECT_STRING_EQ(m, [{1, 11}, {2, 22}, {3, 33}]);
    EXPECT_EQ(m.size(), 3);
  }

  /**
   * @test map& operator=(const map& x)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    map<int, int> m0 = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    map<int, int> m1 = {{1, 11}, {2, 22}, {3, 33}};
    m0 = m1;
    EXPECT_STRING_EQ(m0, [{1, 11}, {2, 22}, {3, 33}]);
    EXPECT_EQ(m0.size(), 3);
    EXPECT_STRING_EQ(m1, [{1, 11}, {2, 22}, {3, 33}]);
    EXPECT_EQ(m1.size(), 3);
  }

  /**
   * @test map& operator=(map&& x)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    map<int, int> m = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    m = map<int, int>({{1, 11}, {2, 22}, {3, 33}});
    EXPECT_STRING_EQ(m, [{1, 11}, {2, 22}, {3, 33}]);
    EXPECT_EQ(m.size(), 3);
  }
}

TEST(map, key_compare) {
  map<int, std::string> m {{1, "he"}, {2, "she"}};
  auto comp = m.key_comp();
  EXPECT_TRUE(comp(m.begin()->first, (++m.begin())->first));
}

TEST(map, value_compare) {
  map<int, std::string> m {{1, "he"}, {2, "she"}};
  auto comp = m.value_comp();
  EXPECT_TRUE(comp(*m.begin(), *++m.begin()));
}

TEST(map, get_allocator) {
  map<int, std::string> s;
  auto alloc = s.get_allocator();
  static_assert(
    tinySTL::is_same_v<decltype(alloc.allocate(1)), tinySTL::pair<int, std::string>*>, 
    "alloc doesn't match pair<int, std::string>*"
  );
}

TEST(map, begin) {
  map<int, std::string> 
  m {{1, "Hello"}, {2, "STL"}, {3, "World"}};
  auto it = m.begin();
  EXPECT_EQ(it->first, 1);
  EXPECT_EQ(it->second, "Hello");
  it->second = "Fuck";
  EXPECT_STRING_EQ(m, [{1, Fuck}, {2, STL}, {3, World}]);
}

TEST(map, end) {
  map<int, std::string> m;
  auto it = --m.end();
  m.insert({0, "Hello"});
  it = --m.end();
  EXPECT_EQ(it->first, 0);
  EXPECT_EQ(it->second, "Hello");
  it->second = "Fuck";
  EXPECT_STRING_EQ(m, [{0, Fuck}]);
}

TEST(map, rbegin) {
  map<int, std::string> 
  m {{1, "Hello"}, {2, "STL"}, {3, "World"}};
  auto it = m.rbegin();
  EXPECT_EQ(it->first, 3);
  EXPECT_EQ(it->second, "World");
  it->second = "Project";
  EXPECT_STRING_EQ(m, [{1, Hello}, {2, STL}, {3, Project}]);
}

TEST(map, rend) {
  map<int, std::string> 
  m {{1, "Hello"}, {2, "STL"}, {3, "World"}};
  auto it = --m.rend();
  EXPECT_EQ(it->first, 1);
  EXPECT_EQ(it->second, "Hello");
  it->second = "Fuck";
  EXPECT_STRING_EQ(m, [{1, Fuck}, {2, STL}, {3, World}]);
}

TEST(map, empty) {
  map<int, std::string> m;
  EXPECT_TRUE(m.empty());
  m.insert({1, "hello"});
  EXPECT_FALSE(m.empty());
  m.erase(1);
  EXPECT_TRUE(m.empty());
}

TEST(map, size) {
  map<int, std::string> m;
  EXPECT_EQ(m.size(), 0);
  m.insert({1, "Hello"});
  m.insert({4, "STL"});
  m.insert({2, "World"});
  EXPECT_EQ(m.size(), 3);
  m.erase(1);
  EXPECT_EQ(m.size(), 2);
  m.erase(5);
  EXPECT_EQ(m.size(), 2);
  m.clear();
  EXPECT_EQ(m.size(), 0);
}

TEST(map, max_size) {
  map<int, std::string> m;
  EXPECT_TRUE(m.max_size() > 0);
}

TEST(map, swap) {
  map<int, std::string> m1, m2 {{1, "Hello"}, {2, "STL"}, {3, "World"}};
  m1.swap(m2);
  EXPECT_STRING_EQ(m1, [{1, Hello}, {2, STL}, {3, World}]);
  EXPECT_STRING_EQ(m2, []);
  m2 = {{1, "aa"}, {3, "bb"}};
  m1.swap(m2);
  EXPECT_STRING_EQ(m1, [{1, aa}, {3, bb}]);
  EXPECT_STRING_EQ(m2, [{1, Hello}, {2, STL}, {3, World}]);
}

TEST(map, emplace) {
  map<int, std::string> m;
  m.emplace(1, "Hello");
  m.emplace(2, "World");
  EXPECT_STRING_EQ(m, [{1, Hello}, {2, World}]);
}

TEST(map, emplace_hint) {
  map<int, std::string> m;
  auto it = m.emplace_hint(m.end(), 2, "World");
  it = m.emplace_hint(it, 1, "Hello");
  EXPECT_STRING_EQ(m, [{1, Hello}, {2, World}]);
}

TEST(map, insert) {
  /**
   * @test  tinySTL::pair<iterator, bool> insert(const value_type& __x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    map<int, std::string> m;
    auto [it, ok] = m.insert({1, "Hello"});
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, "Hello");
    EXPECT_TRUE(ok);
    auto pr = m.insert({1, "World"});
    EXPECT_EQ(pr.first->first, 1);
    EXPECT_EQ(pr.first->second, "Hello");
    EXPECT_FALSE(pr.second);
  }

  /**
   * @test  tinySTL::pair<iterator, bool> insert(value_type&& __x)
   * @brief insert an rvalue reference.
   */
  SUBTEST(insert) {
    map<int, std::string> m;
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
    map<int, int> m;
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
    map<int, int> m;
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
    map<int, int> m;
    list<pair<int, int>> l {{1, 1}, {2, 2}, {3, 3}};
    m.insert(l.begin(), l.end());
    EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {3, 3}]);
  }

  /**
   * @test  void insert(std::initializer_list<value_type> __l)
   * @brief insert initializer_list.
   */
  SUBTEST(insert) {
    map<int, int> m {{11, 1}, {111, 2}};
    m.insert({{1, 1}, {4, 4}, {2, 2}});
    EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {4, 4}, {11, 1}, {111, 2}]);
  }
}

TEST(map, erase) {
  /**
   * @test  size_type erase(const key_type& __k) 
   * @brief erase by key.
   */
  SUBTEST(erase) {
    map<int, std::string> 
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
    map<int, std::string> m;
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
    map<int, std::string> m;
    m = {{1, "Hello"}, {4, "World"}, {2, "STL"}, {5, "Project"}, {0, "Fuck"}};
    auto it = m.erase(m.begin(), ----m.end());
    EXPECT_EQ(it->first, 4);
    EXPECT_STRING_EQ(m, [{4, World}, {5, Project}]);
  }
}

TEST(map, clear) {
  map<int, int> m {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
  m.clear();
  EXPECT_EQ(m.size(), 0);
  EXPECT_STRING_EQ(m, []);
}

TEST(map, count) {
  map<int, int> m {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
  EXPECT_EQ(m.count(3), 0);
  EXPECT_EQ(m.count(4), 1);
}

TEST(map, contains) {
  map<int, int> m {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
  EXPECT_TRUE(m.contains(2));
  EXPECT_FALSE(m.contains(3));
}

TEST(map, find) {
  map<int, int> m1 {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
  map<int, int> m2;
  for (auto&& pr : m1) {
    if (m2.find(pr.first) == m2.end()) {
      m2.insert(pr);
    }
  }
  EXPECT_EQ(m1, m2);
}

TEST(map, lower_bound) {
  map<int, int> m;
  EXPECT_EQ(m.lower_bound(0), m.begin());
  m = {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
  EXPECT_EQ(m.lower_bound(0), m.begin());
  EXPECT_EQ(m.lower_bound(100), m.end());
  EXPECT_EQ(m.lower_bound(3)->first, 4);
  EXPECT_EQ(m.lower_bound(4)->first, 4);
  EXPECT_EQ(m.lower_bound(5)->first, 5);
}

TEST(map, upper_bound) {
  map<int, int> m;
  EXPECT_EQ(m.begin(), m.end());
  EXPECT_EQ(m.upper_bound(0), m.end());
  m = {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
  EXPECT_EQ(m.upper_bound(0)->first, 1);
  EXPECT_EQ(m.upper_bound(100), m.end());
  EXPECT_EQ(m.upper_bound(3)->first, 4);
  EXPECT_EQ(m.upper_bound(4)->first, 5);
  EXPECT_EQ(m.upper_bound(5), m.end());
}

TEST(map, equal_range) {
  map<int, int> m {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
  auto [l1, r1] = m.equal_range(3);
  EXPECT_EQ(l1, r1);
  auto [l2, r2] = m.equal_range(4);
  EXPECT_EQ(l2->first, 4);
  EXPECT_EQ(r2->first, 5);
  EXPECT_EQ(++l2, r2);
}

TEST(map, disp) {
  map<int, int> m;
  m.disp(std::cout);
  m = {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
  m.disp(std::cout);
}

TEST(map, merge) {
  /**
   * @brief merge another same compare map.
   */
  SUBTEST(merge) {
    map<int, int> m1;
    map<int, int> m2 {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_EQ(m1.size(), 5);
    EXPECT_EQ(m2.size(), 0);

    m2 = {{3, 3}, {4, 4}, {5, 5}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, [{4, 4}, {5, 5}]);
    EXPECT_EQ(m1.size(), 6);
    EXPECT_EQ(m2.size(), 2);

    map<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, [{4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 2);
    EXPECT_EQ(m3.size(), 0);
  }

  /**
   * @brief merge another different compare map.
   */
  SUBTEST(merge) {
    using smap = map<int, int, tinySTL::greater<int>>;
    map<int, int> m1;
    smap m2 = {{1, 1}, {4, 4}, {2, 2}, {5, 5}, {0, 0}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_EQ(m1.size(), 5);
    EXPECT_EQ(m2.size(), 0);

    m2 = {{3, 3}, {4, 4}, {5, 5}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, [{5, 5}, {4, 4}]);
    EXPECT_EQ(m1.size(), 6);
    EXPECT_EQ(m2.size(), 2);

    map<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, [{5, 5}, {4, 4}]);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 2);
    EXPECT_EQ(m3.size(), 0);
  }

  /**
   * @brief merge another same compare multimap.
   */
  SUBTEST(merge) {
    map<int, int> m1;
    multimap<int, int> 
    m2 = {{1,1}, {1,1}, {4,4}, {2,2}, {2,2}, {5,5}, {0,0}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, [{1, 1}, {2, 2}]);
    EXPECT_EQ(m1.size(), 5);
    EXPECT_EQ(m2.size(), 2);

    m2 = {{3,3}, {4,4}, {4, 4}, {5,5}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, [{4, 4}, {4, 4}, {5, 5}]);
    EXPECT_EQ(m1.size(), 6);
    EXPECT_EQ(m2.size(), 3);

    map<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, [{4, 4}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 3);
    EXPECT_EQ(m3.size(), 0);
  }

  /**
   * @brief merge another different compare multimap.
   */
  SUBTEST(merge) {
    using smultimap = multimap<int, int, tinySTL::greater<int>>;
    map<int, int> m1;
    smultimap m2 = {{1,1}, {1,1}, {4,4}, {2,2}, {2,2}, {5,5}, {0,0}};
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

    map<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, [{5, 5}, {4, 4}, {4, 4}]);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 3);
    EXPECT_EQ(m3.size(), 0);
  }
}

TEST(map, compare_operator) {
  map<int, int> m1 = {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
  map<int, int> m2 = {{1,1}, {4,4}, {2,2}, {0,0}};
  map<int, int> m3 = {{4,4}, {2,2}};
  EXPECT_TRUE(m1 == m1);
  EXPECT_FALSE(m1 == m2);
  EXPECT_TRUE(m2 < m1);
  EXPECT_TRUE(m2 < m3);
}

TEST(map, cout_operator) {
  map<int, int> m1 = {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
  std::cout << m1 << std::endl;
}