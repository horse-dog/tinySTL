#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "set.h"

using namespace tinySTL;

TEST(set, constructor) {
  /**
   * @test  set()
   * @brief default constructor. 
   */
  SUBTEST(constructor) {
    set<int> s;
    s.insert(1);
    s.insert(4);
    s.insert(2);
    s.insert(5);
    s.insert(0);
    EXPECT_EQ(s.size(), 5);
    EXPECT_STRING_EQ(s, [0, 1, 2, 4, 5]);
  }
  
  // /**
  //  * @test  vector(size_type n). 
  //  * @brief size constructor. 
  //  */
  // SUBTEST(constructor) {
  //   vector<int> vc(5);
  //   EXPECT_STRING_EQ(vc, [0, 0, 0, 0, 0]);
  // }

  // /**
  //  * @test  vector(size_type n, const T& val)
  //  * @brief size with value constructor. 
  //  */
  // SUBTEST(constructor) {
  //   vector<std::string> vc(3, "Hello");
  //   EXPECT_STRING_EQ(vc, [Hello, Hello, Hello]);
  // }

  // /**
  //  * @test  vector(const vector& x)
  //  * @brief copy constructor. 
  //  */
  // SUBTEST(constructor) {
  //   vector<std::string> vc1(3, "Hello");
  //   EXPECT_STRING_EQ(vc1, [Hello, Hello, Hello]);
  //   vector<std::string> vc2(vc1);
  //   EXPECT_STRING_EQ(vc2, [Hello, Hello, Hello]);
  // }

  // /**
  //  * @test  vector(vector&& x)
  //  * @brief move constructor. 
  //  */
  // SUBTEST(constructor) {
  //   vector<std::string> vc1(3, "Hello");
  //   EXPECT_STRING_EQ(vc1, [Hello, Hello, Hello]);
  //   vector<std::string> vc2(tinySTL::move(vc1));
  //   EXPECT_STRING_EQ(vc1, []);
  //   EXPECT_STRING_EQ(vc2, [Hello, Hello, Hello]);
  // }

  // /**
  //  * @test  vector(initializer_list l)
  //  * @brief initializer_list constructor. 
  //  */
  // SUBTEST(constructor) {
  //   vector<std::string> vc = { "Hello", "Standard", "Template", "Library" };
  //   EXPECT_STRING_EQ(vc, [Hello, Standard, Template, Library]);
  // }

  // /**
  //  * @test  vector(Iterator first, Iterator last)
  //  * @brief range constructor. 
  //  */
  // SUBTEST(constructor) {
  //   std::list<double> l = { 3.14, 6.28, 0.618 };
  //   vector<int> vc(l.begin(), l.end());
  //   EXPECT_STRING_EQ(vc, [3, 6, 0]);
  // }
}