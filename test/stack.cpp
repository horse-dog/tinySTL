#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "stack.h"
#include <stack>

using namespace tinySTL;

TEST(stack, constructor) {
  /**
   * @test  stack()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    stack<int> st;
    EXPECT_TRUE(st.empty());
  }

  /**
   * @test  stack(const stack&)
   * @brief copy constructor. 
   */
  SUBTEST(constructor) {
    stack<int> st1;
    st1.push(1);
    st1.push(2);
    st1.push(3);
    stack<int> st2 = st1;
    EXPECT_EQ(st2.size(), 3);
    EXPECT_EQ(st2.top(), 3);
    EXPECT_EQ(st1.size(), 3);
  }

  /**
   * @test  stack(stack&&)
   * @brief move constructor. 
   */
  SUBTEST(constructor) {
    stack<int> st1;
    st1.push(1);
    st1.push(2);
    st1.push(3);
    stack<int> st2 = tinySTL::move(st1);
    EXPECT_EQ(st2.size(), 3);
    EXPECT_EQ(st2.top(), 3);
    EXPECT_EQ(st1.size(), 0);
  }

  /**
   * @test  stack(const container_type&)
   * @brief container copy constructor. 
   */
  SUBTEST(constructor) {
    deque<int> dq {1, 2, 3};
    stack<int> st(dq);
    EXPECT_EQ(st.size(), 3);
    EXPECT_EQ(st.top(), 3);
    EXPECT_EQ(dq.size(), 3);
  }

  /**
   * @test  stack(container_type&&)
   * @brief container move constructor. 
   */
  SUBTEST(constructor) {
    deque<int> dq {1, 2, 3};
    stack<int> st(tinySTL::move(dq));
    EXPECT_EQ(st.size(), 3);
    EXPECT_EQ(st.top(), 3);
    EXPECT_EQ(dq.size(), 0);
  }
}

TEST(stack, emplace) {
  stack<int> st;
  st.emplace(3);
  EXPECT_EQ(st.top(), 3);
  st.emplace(4);
  EXPECT_EQ(st.top(), 4);
}

TEST(stack, empty) {
  stack<int> st;
  EXPECT_TRUE(st.empty());
  st.push(1);
  EXPECT_FALSE(st.empty());
}

TEST(stack, pop) {
  stack<int> st({1, 2, 3});
  st.pop();
  EXPECT_FALSE(st.empty());
  st.pop();
  EXPECT_FALSE(st.empty());
  st.pop();
  EXPECT_TRUE(st.empty());
  st.pop();
  EXPECT_TRUE(st.empty());
}

TEST(stack, push) {
  stack<int> st;
  st.push(1);
  st.push(2);
  st.push(3);
  EXPECT_EQ(st.size(), 3);
  EXPECT_EQ(st.top(), 3);
}

TEST(stack, size) {
  stack<int> st;
  EXPECT_EQ(st.size(), 0);
  st.push(1);
  EXPECT_EQ(st.size(), 1);
  st.push(2);
  EXPECT_EQ(st.size(), 2);
  st.push(3);
  EXPECT_EQ(st.size(), 3);
}

TEST(stack, swap) {
  stack<int> st1({1, 2, 3, 4});
  stack<int> st2({4, 3, 2, 1});
  st1.swap(st2);
  EXPECT_EQ(st1.top(), 1);
  EXPECT_EQ(st2.top(), 4);
}

TEST(stack, top) {
  stack<int> st;
  EXPECT_THROW(st.top(), std::range_error);
  st.push(1);
  EXPECT_EQ(st.top(), 1);
}