#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "map.h"

using namespace tinySTL;

TEST(map, constructor) {

  SUBTEST(constructor) {
    map<std::string, int> m = {{"1", 111}, {"3", 333}, {"0", 0}};
    multimap<std::string, int> m1 = {{"1", 1}, {"1", 111}, {"2", 2}};
    m["2"] = 234;
    std::cout << m << std::endl;
    std::cout << m1 << std::endl;
    printf("%d\n", m["3"]);
    m.disp(std::cout);
  }

}