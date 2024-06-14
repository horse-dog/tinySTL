#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "queue.h"

using namespace tinySTL;

TEST(queue, constructor) {
  /**
   * @test  queue()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    queue<int> qu;
    EXPECT_EQ(qu.size(), 0);
    EXPECT_TRUE(qu.empty());
  }

  /**
   * @test  queue(const queue& __q)
   * @brief copy constructor.
   */
  SUBTEST(constructor) {
    queue<int> qu1;
    qu1.push(1);
    qu1.push(2);
    qu1.push(3);
    queue<int> qu2(qu1);
    EXPECT_EQ(qu1.size(), 3);
    EXPECT_EQ(qu2.size(), 3);
    EXPECT_EQ(qu1.front(), 1);
    EXPECT_EQ(qu1.back(), 3);
    EXPECT_EQ(qu2.front(), 1);
    EXPECT_EQ(qu2.back(), 3);
  }

  /**
   * @test  queue(queue&& __q)
   * @brief move constructor.
   */
  SUBTEST(constructor) {
  queue<int> qu1;
    qu1.push(1);
    qu1.push(2);
    qu1.push(3);
    queue<int> qu2(tinySTL::move(qu1));
    EXPECT_EQ(qu1.size(), 0);
    EXPECT_EQ(qu2.size(), 3);
    EXPECT_ANY_THROW(qu1.front());
    EXPECT_ANY_THROW(qu1.back());
    EXPECT_EQ(qu2.front(), 1);
    EXPECT_EQ(qu2.back(), 3);
  }

  /**
   * @test  explicit queue(const container_type& __c)
   * @brief copy container constructor.
   */
  SUBTEST(constructor) {
    deque<int> dq {1, 4, 2, 5, 0};
    queue<int> qu(dq);
    EXPECT_EQ(qu.size(), 5);
    EXPECT_EQ(dq.size(), 5);
    EXPECT_EQ(qu.front(), 1);
    EXPECT_EQ(qu.back(), 0);
    EXPECT_EQ(dq.front(), 1);
    EXPECT_EQ(dq.back(), 0);
  }

  /**
   * @test  explicit queue(container_type&& __c)
   * @brief move container constructor.
   */
  SUBTEST(constructor) {
    deque<int> dq {1, 4, 2, 5, 0};
    queue<int> qu(tinySTL::move(dq));
    EXPECT_EQ(qu.size(), 5);
    EXPECT_EQ(dq.size(), 0);
    EXPECT_EQ(qu.front(), 1);
    EXPECT_EQ(qu.back(), 0);
    EXPECT_ANY_THROW(dq.front());
    EXPECT_ANY_THROW(dq.back());
  }
}

TEST(queue, back) {
  queue<int> qu;
  EXPECT_THROW(qu.back(), std::range_error);
  qu.push(1);
  qu.push(2);
  qu.push(3);
  EXPECT_EQ(qu.back(), 3);
  qu.back() = 4;
  EXPECT_EQ(qu.back(), 4);
}

TEST(queue, emplace) {
  queue<tinySTL::pair<int, std::string>> qu;
  qu.emplace(3, "hello");
  qu.emplace(4, "world");
  EXPECT_EQ(qu.size(), 2);
  EXPECT_EQ(qu.back().first, 4);
  EXPECT_EQ(qu.back().second, "world");
}

TEST(queue, empty) {
  queue<int> qu;
  EXPECT_TRUE(qu.empty());
  EXPECT_EQ(qu.size(), 0);
  qu.push(1);
  EXPECT_FALSE(qu.empty());
  EXPECT_EQ(qu.size(), 1);
}

TEST(queue, front) {
  queue<int> qu;
  EXPECT_THROW(qu.front(), std::range_error);
  qu.push(1);
  qu.push(2);
  qu.push(3);
  EXPECT_EQ(qu.front(), 1);
  qu.front() = 11;
  EXPECT_EQ(qu.front(), 11);
}

TEST(queue, pop) {
  queue<int> qu;
  qu.push(1);
  qu.push(2);
  qu.push(3);
  int __pop_times {0};
  while (!qu.empty()) {
    qu.pop();
    ++__pop_times;
  }
  EXPECT_EQ(__pop_times, 3);
  EXPECT_EQ(qu.size(), 0);
  EXPECT_TRUE(qu.empty());
}

TEST(queue, push) {
  queue<int> qu;
  qu.push(1);
  qu.push(2);
  qu.push(3);
  int __pop_times {0};
  while (!qu.empty()) {
    qu.pop();
    ++__pop_times;
  }
  EXPECT_EQ(__pop_times, 3);
  EXPECT_EQ(qu.size(), 0);
  EXPECT_TRUE(qu.empty());
}

TEST(queue, size) {
  queue<int> qu;
  EXPECT_EQ(qu.size(), 0);
  int __size_sync {0};
  for (int i = 0; i < 3; i++) {
    qu.push(i);
    EXPECT_EQ(qu.size(), ++__size_sync);
  }
  for (int i = 0; i < 3; i++) {
    qu.pop();
    EXPECT_EQ(qu.size(), --__size_sync);
  }
}

TEST(queue, swap) {
  queue<int> qu1, qu2({1, 4, 2, 5, 0});
  qu1.swap(qu2);
  EXPECT_EQ(qu1.size(), 5);
  EXPECT_EQ(qu1.front(), 1);
  EXPECT_EQ(qu1.back(), 0);
  EXPECT_EQ(qu2.size(), 0);
}

TEST(queue, operator_less) {
  queue<int> qu1({5, 2, 3});
  queue<int> qu2({5, 2, 1, 4});
  EXPECT_FALSE(qu1 < qu2);
}

TEST(queue, operator_equal) {
  queue<int> qu1({5, 2, 3});
  queue<int> qu2({2, 2, 3, 4});
  queue<int> qu3({5, 2, 3});
  EXPECT_TRUE(qu1 != qu2);
  EXPECT_TRUE(qu1 == qu3);
}