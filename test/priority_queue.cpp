#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "queue.h"
#include "list.h"

using namespace tinySTL;

TEST(priority_queue, constructor) {
  /**
   * @test  priority_queue()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    priority_queue<int> pq;
    EXPECT_TRUE(pq.empty());
    EXPECT_EQ(pq.size(), 0);
  }

  /**
   * @test  priority_queue(const _Sequence& __s)
   * @brief const sequence& constructor.
   */
  SUBTEST(constructor) {
    vector<int> vc {1, 4, 2, 5, 0};
    priority_queue<int> pq(vc);
    EXPECT_EQ(pq.top(), 5);
    EXPECT_EQ(pq.size(), 5);
    EXPECT_STRING_EQ(vc, [1, 4, 2, 5, 0]);
    EXPECT_EQ(vc.size(), 5);
  }

  /**
   * @test  priority_queue(_Sequence&& __s)
   * @brief sequence&& constructor.
   */
  SUBTEST(constructor) {
    vector<int> vc {1, 4, 2, 5, 0};
    priority_queue<int> pq(tinySTL::move(vc));
    EXPECT_EQ(pq.top(), 5);
    EXPECT_EQ(pq.size(), 5);
    EXPECT_STRING_EQ(vc, []);
    EXPECT_EQ(vc.size(), 0);
  }

  /**
   * @test  priority_queue(const priority_queue& __q)
   * @brief copy constructor.
   */
  SUBTEST(constructor) {
    priority_queue<int> pq1({1, 4, 2, 5, 0});
    priority_queue<int> pq2(pq1);
    EXPECT_EQ(pq1.top(), 5);
    EXPECT_EQ(pq1.size(), 5);
    EXPECT_EQ(pq2.top(), 5);
    EXPECT_EQ(pq2.size(), 5);
  }

  /**
   * @test  priority_queue(const priority_queue& __q)
   * @brief move constructor.
   */
  SUBTEST(constructor) {
    priority_queue<int> pq1({1, 4, 2, 5, 0});
    priority_queue<int> pq2(tinySTL::move(pq1));
    EXPECT_ANY_THROW(pq1.top());
    EXPECT_EQ(pq1.size(), 0);
    EXPECT_EQ(pq2.top(), 5);
    EXPECT_EQ(pq2.size(), 5);
  }

  /**
   * @test  template<Iterable _InputIterator>
            priority_queue(_InputIterator __first, _InputIterator __last,
            const _Compare& __x,
            const _Sequence& __s)
   * @brief copy with range constructor.
   */
  SUBTEST(constructor) {
    vector<int> vc1 = {1, 4, 2, 5, 0};
    vector<int> vc2 = {11, 22, 33};
    priority_queue<int> pq(vc1.begin(), vc1.end(), less<int>{}, vc2);
    EXPECT_EQ(pq.top(), 33);
    EXPECT_EQ(pq.size(), 8);
    EXPECT_EQ(vc1.size(), 5);
    EXPECT_EQ(vc2.size(), 3);
  }

  /**
   * @test  template<Iterable _InputIterator>
            priority_queue(_InputIterator __first, _InputIterator __last,
            const _Compare& __x = _Compare(),
            _Sequence&& __s = _Sequence())
   * @brief move with range constructor.
   */
  SUBTEST(constructor) {
    vector<int> vc1 = {1, 4, 2, 5, 0};
    vector<int> vc2 = {11, 22, 33};
    priority_queue<int> pq1(vc1.begin(), vc1.end(), less<int>{}, tinySTL::move(vc2));
    EXPECT_EQ(pq1.top(), 33);
    EXPECT_EQ(pq1.size(), 8);
    EXPECT_EQ(vc1.size(), 5);
    EXPECT_EQ(vc2.size(), 0);

    // range construct.
    list<int> l {1, 4, 2, 5, 0};
    priority_queue<int> pq2(l.begin(), l.end());
    EXPECT_EQ(pq2.top(), 5);
    EXPECT_EQ(pq2.size(), 5);
  }
}

TEST(priority_queue, emplace) {
  priority_queue<int, vector<int>,  greater<int>> pq;
  pq.emplace(1);
  pq.emplace(4);
  pq.emplace(2);
  pq.emplace(0);
  pq.emplace(5);
  EXPECT_EQ(pq.top(), 0);
  EXPECT_EQ(pq.size(), 5);
}

TEST(priority_queue, empty) {
  priority_queue<int> pq;
  EXPECT_TRUE(pq.empty());
  pq.push(1);
  pq.push(4);
  pq.push(1);
  pq.pop();
  pq.pop();
  EXPECT_FALSE(pq.empty());
}

TEST(priority_queue, pop) {
  priority_queue<int> pq;
  pq.pop();
  EXPECT_EQ(pq.size(), 0);
  pq.push(1);
  EXPECT_EQ(pq.size(), 1);
  EXPECT_NO_THROW(pq.pop());
  EXPECT_EQ(pq.size(), 0);
}

TEST(priority_queue, push) {
  priority_queue<int> pq;
  pq.push(1);
  pq.push(3);
  pq.push(2);
  EXPECT_EQ(pq.size(), 3);
  EXPECT_EQ(pq.top(), 3);
  int x = 6;
  pq.push(x);
  EXPECT_EQ(pq.size(), 4);
  EXPECT_EQ(pq.top(), 6);
}

TEST(priority_queue, size) {
  priority_queue<int> pq;
  EXPECT_EQ(pq.size(), 0);
  pq.push(1);
  pq.push(3);
  pq.push(2);
  EXPECT_EQ(pq.size(), 3);
}

TEST(priority_queue, swap) {
  priority_queue<int> pq1;
  priority_queue<int> pq2;
  pq1.push(1);
  pq1.push(3);
  pq1.swap(pq2);
  EXPECT_EQ(pq1.size(), 0);
  EXPECT_EQ(pq2.size(), 2);
  EXPECT_EQ(pq2.top(), 3);
}

TEST(priority_queue, top) {
  priority_queue<int> pq;
  EXPECT_THROW(pq.top(), std::range_error);
  pq.push(1);
  pq.push(4);
  pq.push(5);
  pq.push(2);
  pq.push(0);
  EXPECT_EQ(pq.top(), 5);
}
