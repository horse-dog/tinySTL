#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "map.h"
#include "list.h"

using namespace tinySTL;

TEST(multimap, constructor) {
  /**
   * @test  multimap()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    multimap<int, int> m;
    EXPECT_STRING_EQ(m, []);
  }

  /**
   * @test  multimap(const multimap&)
   * @brief copy constructor.
   */
  SUBTEST(constructor) {
    multimap<int, int> m1{{4,4}, {3,3}, {4,4}, {7,7}, {1,1}};
    EXPECT_STRING_EQ(m1, [{1, 1}, {3, 3}, {4, 4}, {4, 4}, {7, 7}]);
    multimap<int, int> m2(m1);
    EXPECT_STRING_EQ(m2, [{1, 1}, {3, 3}, {4, 4}, {4, 4}, {7, 7}]);
  }

  /**
   * @test  multimap(multimap&&)
   * @brief move constructor.
   */
  SUBTEST(constructor) {
    multimap<int, int> m1{{4,4}, {3,3}, {4,4}, {7,7}, {1,1}};
    EXPECT_STRING_EQ(m1, [{1, 1}, {3, 3}, {4, 4}, {4, 4}, {7, 7}]);
    multimap<int, int> m2(tinySTL::move(m1));
    EXPECT_EQ(m1.size(), 0);
    EXPECT_EQ(m2.size(), 5);
    EXPECT_STRING_EQ(m1, []);
    EXPECT_STRING_EQ(m2, [{1, 1}, {3, 3}, {4, 4}, {4, 4}, {7, 7}]);
  }

  /**
   * @test  multimap(std::initializer_list<value_type> __l)
   * @brief initializer_list constructor. 
   */
  SUBTEST(constructor) {
    multimap<int, std::string> m {{1, "Hello"}, {1, "World"}, {2, "STL"}};
    EXPECT_STRING_EQ(m, [{1, World}, {1, Hello}, {2, STL}]);
  }

  /**
   * @test  multimap(Iterator __first, Iterator __last)
   * @brief range constructor. 
   */
  SUBTEST(constructor) {
    list<tinySTL::pair<int, int>> l {{1, 1}, {3, 3}, {3, 3}, {0, 0}};
    multimap<int, int> m(l.begin(), l.end());
    EXPECT_STRING_EQ(m, [{0, 0}, {1, 1}, {3, 3}, {3, 3}]);
  }
}

TEST(multimap, assign_operator) {
  /**
   * @test  multimap& operator=(std::initializer_list<value_type> __l)
   * @brief assign operator by initializer_list.
   */
  SUBTEST(operator=) {
    multimap<int, int> m {{1,1}, {2,2}, {3,3}, {4,4}};
    m = {{1,1}, {2,2}, {3,3}, {3,3}, {5,5}};
    EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {3, 3}, {3, 3}, {5, 5}]);
    EXPECT_EQ(m.size(), 5);
  }

  /**
   * @test  multimap& operator=(const multimap&)
   * @brief assign operator by copy.
   */
  SUBTEST(operator=) {
    multimap<int, int> m0 {{1,1}, {2,2}, {2,2}, {4,4}};
    multimap<int, int> m1 {{1,1}, {2,2}, {3,3}, {3,3}, {5,5}};
    m0 = m1;
    EXPECT_STRING_EQ(m0, [{1, 1}, {2, 2}, {3, 3}, {3, 3}, {5, 5}]);
    EXPECT_EQ(m0.size(), 5);
    EXPECT_STRING_EQ(m1, [{1, 1}, {2, 2}, {3, 3}, {3, 3}, {5, 5}]);
    EXPECT_EQ(m1.size(), 5);
  }

  /**
   * @test  multimap& operator=(multimap&&)
   * @brief assign operator by move.
   */
  SUBTEST(operator=) {
    multimap<int, int> m0 {{1,1}, {2,2}, {2,2}, {4,4}};
    multimap<int, int> m1 {{1,1}, {2,2}, {3,3}, {3,3}, {5,5}};
    m0 = tinySTL::move(m1);
    EXPECT_STRING_EQ(m0, [{1, 1}, {2, 2}, {3, 3}, {3, 3}, {5, 5}]);
    EXPECT_EQ(m0.size(), 5);
    EXPECT_STRING_EQ(m1, []);
    EXPECT_EQ(m1.size(), 0);
  }
}

TEST(multimap, key_comp) {
  multimap<int, int> m {{1,1}, {2,2}, {3,3}, {4,4}};
  auto comp = m.key_comp();
  EXPECT_TRUE(comp(m.begin()->first, (++m.begin())->first));
}

TEST(multimap, value_comp) {
  multimap<int, int> m {{1,1}, {2,2}, {3,3}, {4,4}};
  auto comp = m.value_comp();
  EXPECT_TRUE(comp(*m.begin(), *++m.begin()));
}

TEST(multimap, get_allocator) {
  multimap<int, int> m;
  auto alloc = m.get_allocator();
}

TEST(multimap, begin) {
  multimap<int, int> m {{5,5}, {4,4}, {0,0}, {5,5}, {0,0}};
  auto it = m.begin();
  EXPECT_EQ(it->first, 0);
  it->second = 111;
  EXPECT_STRING_EQ(m, [{0, 111}, {0, 0}, {4, 4}, {5, 5}, {5, 5}]);
}

TEST(multimap, end) {
  multimap<int, int> m;
  auto it = --m.end();
  m = {{5,5}, {4,4}, {0,0}, {5,5}, {0,0}};
  it = --m.end();
  EXPECT_EQ(it->first, 5);
  it->second = 55;
  EXPECT_STRING_EQ(m, [{0, 0}, {0, 0}, {4, 4}, {5, 5}, {5, 55}]);
}

TEST(multimap, rbegin) {
  multimap<int, int> m {{2,2}, {4,4}, {2,2}, {5,5}, {2,2}};
  auto it = m.rbegin();
  EXPECT_EQ(it->first, 5);
  it->second = 55;
  EXPECT_STRING_EQ(m, [{2, 2}, {2, 2}, {2, 2}, {4, 4}, {5, 55}]);
}

TEST(multimap, rend) {
  multimap<int, int> m {{2,2}, {4,4}, {2,2}, {5,5}, {2,2}};
  auto it = --m.rend();
  EXPECT_EQ(it->first, 2);
  it->second = 22;
  EXPECT_STRING_EQ(m, [{2, 22}, {2, 2}, {2, 2}, {4, 4}, {5, 5}]);
}

TEST(multimap, empty) {
  multimap<int, int> m;
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

TEST(multimap, size) {
  multimap<int, int> m;
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

TEST(multimap, max_size) {
  multimap<int, int> m;
  EXPECT_TRUE(m.max_size() > 0);
}

TEST(multimap, swap) {
  multimap<int, int> m1, m2 {{1,1}, {2,2}, {2,2}, {1,1}, {0,0}};
  m1.swap(m2);
  EXPECT_STRING_EQ(m1, [{0, 0}, {1, 1}, {1, 1}, {2, 2}, {2, 2}]);
  EXPECT_STRING_EQ(m2, []);
  m2 = {{111,111}, {111,111}, {333,333}, {111,111}};
  m1.swap(m2);
  EXPECT_STRING_EQ(m1, [{111, 111}, {111, 111}, {111, 111}, {333, 333}]);
  EXPECT_STRING_EQ(m2, [{0, 0}, {1, 1}, {1, 1}, {2, 2}, {2, 2}]);
}

TEST(multimap, emplace) {
  multimap<int, std::string> m;
  m.emplace(1, "Hello");
  m.emplace(2, "World");
  m.emplace(1, "Fuck");
  EXPECT_STRING_EQ(m, [{1, Hello}, {1, Fuck}, {2, World}]);
}

TEST(multimap, emplace_hint) {
  multimap<int, std::string> m;
  auto it = m.emplace_hint(m.end(), 2, "World");
  it = m.emplace_hint(it, 1, "Hello");
  it = m.emplace_hint(it, 1, "Fuck");
  EXPECT_STRING_EQ(m, [{1, Fuck}, {1, Hello}, {2, World}]);
}

TEST(multimap, insert) {
  /**
   * @test  iterator insert(const value_type& __x)
   * @brief insert an lvalue reference.
   */
  SUBTEST(insert) {
    multimap<int, int> m;
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
    EXPECT_STRING_EQ(m, [{1, 111}, {1, 112}, {5, 520}]);
  }

  /**
   * @test  iterator insert(value_type&& __x)
   * @brief insert an rvalue reference.
   */
  SUBTEST(insert) {
    multimap<int, list<int>> m;
    pair node = {1, list{1, 10, 100}};
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
    multimap<int, int> m;
    pair node {5, 5};
    --node.first;
    m.insert(m.begin(), node);
    --node.first;
    m.insert(m.begin(), node);
    --node.first;
    m.insert(m.begin(), node);
    m.insert(m.begin(), node);
    EXPECT_STRING_EQ(m, [{2, 5}, {2, 5}, {3, 5}, {4, 5}]);
  }

  /**
   * @test  iterator insert(const_iterator __hint, value_type&& __x)
   * @brief insert an rvalue reference and using hint.
   */
  SUBTEST(insert) {
    multimap<int, int> m;
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
    multimap<int, int> m;
    std::vector<pair<int, int>> vc {{1,1}, {2,2}, {2,2}, {5,5}, {2,2}};
    m.insert(vc.begin(), vc.begin()+4);
    EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {2, 2}, {5, 5}]);
  }

  /**
   * @test  void insert(std::initializer_list<value_type> __l)
   * @brief insert initializer_list.
   */
  SUBTEST(insert) {
    multimap<int, int> m;
    m.insert({{1,1}, {2,2}, {2,2}, {5,5}, {2,2}});
    EXPECT_STRING_EQ(m, [{1, 1}, {2, 2}, {2, 2}, {2, 2}, {5, 5}]);
  }
}

TEST(multimap, erase) {
  /**
   * @test  size_type erase(const key_type& __k)
   * @brief erase by value.
   */
  SUBTEST(erase) {
    multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {1,11}};
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
    multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {1,10}};
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
    multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {1,10}};
    auto it = m.erase(m.begin(), ----m.end());
    EXPECT_EQ(it->first, 4);
    EXPECT_EQ(it->second, 4);
    EXPECT_STRING_EQ(m, [{4, 4}, {5, 5}]);
  }
}

TEST(multimap, clear) {
  multimap<int, int> m {{1,1}, {4,4}, {2,2}, {4,42}, {0,0}};
  m.clear();
  EXPECT_EQ(m.size(), 0);
  EXPECT_STRING_EQ(m, []);
}

TEST(multimap, count) {
  multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {2,21}};
  EXPECT_EQ(m.count(3), 0);
  EXPECT_EQ(m.count(2), 2);
}

TEST(multimap, contains) {
  multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {2,21}};
  EXPECT_TRUE(m.contains(2));
  EXPECT_FALSE(m.contains(3));
}

TEST(multimap, find) {
  multimap<int, int> m1 {{1,1}, {4,4}, {2,2}, {5,5}, {2,21}};
  multimap<int, int> m2;
  for (auto&& [k, v] : m1) {
    if (m2.find(k) == m2.end()) {
      m2.emplace(k, v);
    }
  }
  EXPECT_STRING_EQ(m2, [{1, 1}, {2, 2}, {4, 4}, {5, 5}]);
}

TEST(multimap, lower_bound) {
  multimap<int, int> m;
  EXPECT_EQ(m.lower_bound(0), m.begin());
  m = {{1,1}, {0,0}, {2,2}, {5,5}, {0,0}};
  EXPECT_EQ(m.lower_bound(0), m.begin());
  EXPECT_EQ(m.lower_bound(100), m.end());
  EXPECT_EQ(m.lower_bound(2)->first, 2);
  EXPECT_EQ(m.lower_bound(4)->first, 5);
  EXPECT_EQ(m.lower_bound(5)->first, 5);
}

TEST(multimap, upper_bound) {
  multimap<int, int> m;
  EXPECT_EQ(m.upper_bound(0), m.end());
  m = {{1,1}, {0,0}, {2,2}, {5,5}, {0,0}};
  EXPECT_EQ(m.upper_bound(0)->first, 1);
  EXPECT_EQ(m.upper_bound(100), m.end());
  EXPECT_EQ(m.upper_bound(1)->first, 2);
  EXPECT_EQ(m.upper_bound(2)->first, 5);
  EXPECT_EQ(m.upper_bound(4)->first, 5);
  EXPECT_EQ(m.upper_bound(5), m.end());
}

TEST(multimap, equal_range) {
  multimap<int, int> m {{1,1}, {0,0}, {2,2}, {5,5}, {0,0}};
  auto [l1, r1] = m.equal_range(3);
  EXPECT_EQ(l1, r1);
  auto [l2, r2] = m.equal_range(0);
  EXPECT_EQ(l2->first, 0);
  EXPECT_EQ(r2->first, 1);
  EXPECT_EQ(++++l2, r2);
}

TEST(multimap, disp) {
  multimap<int, int> m;
  m.disp(std::cout);
  m = {{1,1}, {0,0}, {2,2}, {5,5}, {0,0}};
  m.disp(std::cout);
}

TEST(multimap, merge) {
  /**
   * @brief merge another same compare multimap.
   */
  SUBTEST(merge) {
    multimap<int, int> m1;
    multimap<int, int> m2 {{1,1}, {4,4}, {2,2}, {5,5}, {2,2}};
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

    multimap<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 0);
    EXPECT_EQ(m3.size(), 0);
  }

  /**
   * @brief merge another different compare multimap.
   */
  SUBTEST(merge) {
    using smmap = multimap<int, int, tinySTL::greater<int>>;
    multimap<int, int> m1;
    smmap m2 = {{1,1}, {4,4}, {2,2}, {5,5}, {2,2}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{1, 1}, {2, 2}, {2, 2}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_EQ(m1.size(), 5);
    EXPECT_EQ(m2.size(), 0);

    m2 = {{3,3}, {3,3}, {4,4}};
    m1.merge(m2);
    EXPECT_STRING_EQ(m1, [{1, 1}, {2, 2}, {2, 2}, {3, 3}, {3, 3}, {4, 4}, {4, 4}, {5, 5}]);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_EQ(m1.size(), 8);
    EXPECT_EQ(m2.size(), 0);

    map<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 0);
    EXPECT_EQ(m3.size(), 0);
  }

  /**
   * @brief merge another same compare map.
   */
  SUBTEST(merge) {
    multimap<int, int> m1;
    map<int, int> m2 {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
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

    map<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 0);
    EXPECT_EQ(m3.size(), 0);
  }

  /**
   * @brief merge another different compare map.
   */
  SUBTEST(merge) {
    using smap = map<int, int, tinySTL::greater<int>>;
    multimap<int, int> m1;
    smap m2 {{1,1}, {4,4}, {2,2}, {5,5}, {0,0}};
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

    map<int, int> m3;
    m2.merge(m3);
    EXPECT_STRING_EQ(m2, []);
    EXPECT_STRING_EQ(m3, []);
    EXPECT_EQ(m2.size(), 0);
    EXPECT_EQ(m3.size(), 0);
  }
}

TEST(multimap, compare_operator) {
  multimap<int, int> m1 {{1,1}, {2,2}, {2,2}, {5,5}, {0,0}};
  multimap<int, int> m2 {{1,1}, {2,2}, {2,2}, {0,0}};
  multimap<int, int> m3 {{4,4}, {2,2}};
  EXPECT_TRUE(m1 == m1);
  EXPECT_FALSE(m1 == m2);
  EXPECT_TRUE(m2 < m1);
  EXPECT_TRUE(m2 < m3);
}

TEST(multimap, cout_operator) {
  multimap<int, int> m {{1,1}, {4,4}, {2,2}, {5,5}, {2,2}};
  std::cout << m << std::endl;
}