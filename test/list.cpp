#include "list.h"
#include <list>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define test TEST
#define eq EXPECT_EQ
#define streq EXPECT_STREQ
#define tr EXPECT_THROW

template <class container>
static std::string toString(const container &__x) {
  std::string result = "[";
  for (const auto &item : __x) {
    std::ostringstream os;
    os << item;
    result += std::string(os.str()) + ", ";
  }
  result = result.substr(0, result.size() - 2) + ']';
  return result;
}

#define seq(x, y) EXPECT_STREQ(toString(x).c_str(), y)

using namespace tinySTL;

// list()
test(list, constructor0) {
  list<int> li;
  eq(li.size(), 0);
  eq(li.begin(), li.end());
  tr(li.front(), std::range_error);
  tr(li.back(), std::range_error);
}

// list(size_type __n)
test(list, constructor1) {
  list<int> li(8);
  eq(li.size(), 8);
  eq(li.front(), 0);
  eq(li.back(), 0);
}

// list(size_type __n, const T &__val)
test(list, constructor2) {
  list<const char *> li(8, "hello world!");
  eq(li.size(), 8);
  streq(li.front(), "hello world!");
  streq(li.back(), "hello world!");
}

// list(const list &__x)
test(list, constructor3) {
  list<int> li0 = {1, 2, 3, 4, 5};
  list<int> li1 = li0;
  eq(li1.size(), 5);
  eq(li1.front(), 1);
  eq(li1.back(), 5);
}

// list(list &&__x)
test(list, constructor4) {
  list<int> li0 = {1, 2, 3, 4, 5};
  list<int> li1 = std::move(li0);
  eq(li1.size(), 5);
  eq(li1.front(), 1);
  eq(li1.back(), 5);
}

// list(std::initializer_list<T> __l)
test(list, constructor5) {
  list<int> li = {1, 2, 3, 4, 5};
  eq(li.size(), 5);
  eq(li.front(), 1);
  eq(li.back(), 5);
}

// list(Iterable auto __first, Iterable auto __last)
test(list, constructor6) {
  std::vector<double> vc = {3.14, 6.28, 0.618};
  list<int> li(vc.begin(), vc.end());
  eq(li.size(), 3);
  eq(li.front(), 3);
  eq(li.back(), 0);
}

// void assign(std::initializer_list<T> __l)
test(list, assign0) {
  list<int> li = {1, 2, 3};
  li.assign({1, 2, 3, 4});
  eq(li.size(), 4);
  eq(li.front(), 1);
  eq(li.back(), 4);
}

// void assign(size_type __n, const T &__val)
test(list, assign1) {
  list<int> li = {1, 2, 3};
  li.assign(2, 100);
  eq(li.size(), 2);
  eq(li.front(), 100);
  eq(li.back(), 100);
}

// void assign(Iterable auto __first, Iterable auto __last)
test(list, assign2) {
  std::vector<double> vc = {3.14, 6.28, 0.618};
  list<int> li(2);
  li.assign(vc.begin(), vc.end());
  eq(li.size(), 3);
  eq(li.front(), 3);
  eq(li.back(), 0);
}

// reference back()
// const_reference back() const
test(list, back) {
  list<int> li;
  tr(li.back(), std::range_error);
  li = {1, 2, 3};
  eq(li.back(), 3);
  li.back() = 33;
  eq(li.back(), 33);
}

// iterator begin()
// const_iterator begin() const
test(list, begin) {
  list<int> li = {1, 2, 3};
  eq(*li.begin(), 1);
  *li.begin() = 11;
  eq(*li.begin(), 11);
}

// void clear()
test(list, clear) {
  list<int> li;
  li.clear();
  eq(li.size(), 0);
  li = {1, 2, 3};
  eq(li.size(), 3);
  eq(li.front(), 1);
  eq(li.back(), 3);
  li.clear();
  eq(li.size(), 0);
}

// template<typename... _Args>
// iterator emplace(const_iterator __position, _Args &&...__args)
test(list, emplace) {
  list<int> li = {1, 2, 3};
  li.emplace(li.begin(), 11);
  eq(li.front(), 11);
  eq(li.back(), 3);
}

// template<typename... _Args>
// iterator emplace_back(_Args &&...__args)
test(list, emplace_back) {
  list<int> li;
  li.emplace_back(1);
  eq(li.back(), 1);
  li.emplace_back(11);
  eq(li.back(), 11);
}

// template<typename... _Args>
// iterator emplace_front(_Args &&...__args)
test(list, emplace_front) {
  list<int> li;
  li.emplace_front(1);
  eq(li.front(), 1);
  li.emplace_front(11);
  eq(li.front(), 11);
}

// bool empty() const
test(list, empty) {
  list<int> li = {1, 2, 3};
  eq(li.empty(), false);
  li.clear();
  eq(li.empty(), true);
}

// iterator end()
// const_iterator end() const
test(list, end) {
  list<int> li;
  eq(--li.begin(), li.end());
  li = {1, 2, 3};
  eq(*--li.end(), 3);
}

// iterator erase(const_iterator __position)
test(list, erase0) {
  list<int> li = {1, 2, 3};
  auto it = li.erase(li.begin());
  eq(li.size(), 2);
  eq(li.front(), 2);
  eq(*it, 2);
}

// iterator erase(const_iterator __first, const_iterator __last)
test(list, erase1) {
  list<int> li = {1, 2, 3};
  auto it = li.erase(++li.begin(), li.end());
  eq(li.size(), 1);
  eq(li.front(), 1);
  eq(it, li.end());
}

// reference front()
// const_reference front() const
test(list, front) {
  list<int> li;
  tr(li.front(), std::range_error);
  li = {1, 2, 3};
  eq(li.front(), 1);
  li.front() = 11;
  eq(li.front(), 11);
}

// allocator_type get_allocator() const
test(list, get_allocator) {
  list<int> li;
  li.get_allocator();
}

// iterator insert(const_iterator __position, std::initializer_list<T> __l)
test(list, insert0) {
  list<int> li = {1, 2, 3};
  auto it = li.insert(++li.begin(), {11, 22, 33});
  eq(li.size(), 6);
  eq(li.front(), 1);
  eq(++li.begin(), it);
  eq(*it++, 11);
  eq(*it++, 22);
  eq(*it++, 33);
  eq(*it++, 2);
  eq(*it++, 3);
}

// iterator insert(const_iterator __position, T &&__x)
test(list, insert1) {
  list<int> li = {1, 2, 3};
  li.insert(li.begin(), 11);
  eq(li.size(), 4);
  eq(li.front(), 11);
}

// iterator insert(const_iterator __position, const T &__x)
test(list, insert2) {
  list<int> li = {1, 2, 3};
  int x = 12;
  li.insert(li.begin(), x);
  eq(li.size(), 4);
  eq(li.front(), 12);
}

// iterator insert(const_iterator __position, size_type __n, const T &__x)
test(list, insert3) {
  list<int> li = {1, 2, 3};
  li.insert(li.begin(), 3, 11);
  eq(li.size(), 6);
  eq(li.front(), 11);
}

// iterator insert(const_iterator __position,
//                   Iterable auto __first, Iterable auto __last)
test(list, insert4) {
  std::vector<int> vc = {11, 22, 33};
  list<int> li = {1, 2, 3};
  li.insert(li.begin(), vc.begin(), vc.end());
  eq(li.size(), 6);
  eq(li.front(), 11);
}

// size_type max_size() const
test(list, max_size) {
  list<int> li;
  eq(li.begin(), li.cbegin());
}

// // void merge(list &__x)
// test(list, merge0) {
//   list<int> li0 = {1, 3, 8, 9};
//   list<int> li1 = {2, 3, 4, 5, 6, 11, 12};
//   li0.merge(li1);
//   eq(li0.size(), 11);
//   eq(li1.size(), 0);
//   seq(li0, "[1, 2, 3, 3, 4, 5, 6, 8, 9, 11, 12]");
// }

// // void merge(list &&__x)
// test(list, merge1) {
//   list<int> li = {2, 3, 4, 5, 6, 11, 12};
//   li.merge(list({1, 3, 8, 9}));
//   eq(li.size(), 11);
//   auto it = li.begin();
//   seq(li, "[1, 2, 3, 3, 4, 5, 6, 8, 9, 11, 12]");
// }

// // void merge(list &__x, _StrictWeakOrdering __comp)
// test(list, merge2) {
//   struct op {
//     bool operator()(int a, int b) { return a > b; }
//   };
//   list<int> li0 = {9, 8, 3, 1};
//   list<int> li1 = {12, 11, 6, 5, 4, 3, 2};
//   li0.merge(li1, op());
//   eq(li0.size(), 11);
//   eq(li1.size(), 0);
//   seq(li0, "[12, 11, 9, 8, 6, 5, 4, 3, 3, 2, 1]");
// }

// // void merge(list &&__x, _StrictWeakOrdering __comp)
// test(list, merge3) {
//   struct op {
//     bool operator()(int a, int b) { return a > b; }
//   };
//   list<int> li = {12, 11, 6, 5, 4, 3, 2};
//   li.merge(list({9, 8, 3, 1}), op());
//   eq(li.size(), 11);
//   seq(li, "[12, 11, 9, 8, 6, 5, 4, 3, 3, 2, 1]");
// }

// list &operator=(std::initializer_list<T> __l)
test(list, op_il_asgnmt) {
  list<int> li = {1, 2, 3, 4};
  li = {1, 2, 3};
  eq(li.size(), 3);
  eq(li.front(), 1);
  eq(li.back(), 3);
}

// list &operator=(list &&__x)
test(list, op_mv_asgnmt) {
  list<int> li = {1, 2, 3, 4};
  li = list<int>({1, 2, 3});
  eq(li.size(), 3);
  eq(li.front(), 1);
  eq(li.back(), 3);
}

// list &operator=(const list &__x)
test(list, op_cp_asgnmt) {
  list<int> li0 = {1, 2, 3, 4};
  list<int> li1 = {1, 2, 3};
  li0 = li1;
  eq(li0.size(), 3);
  eq(li0.front(), 1);
  eq(li0.back(), 3);
  seq(li0, "[1, 2, 3]");
}

// void pop_back()
test(list, pop_back) {
  list<int> li = {1, 2, 3, 4};
  li.pop_back();
  eq(li.size(), 3);
  seq(li, "[1, 2, 3]");
}

// void pop_front()
test(list, pop_front) {
  list<int> li = {1, 2, 3, 4};
  li.pop_front();
  eq(li.size(), 3);
  seq(li, "[2, 3, 4]");
}

// void push_back(T &&__x)
test(list, push_back0) {
  list<int> li = {1, 2, 3, 4};
  li.push_back(33);
  eq(li.size(), 5);
  seq(li, "[1, 2, 3, 4, 33]");
}

// void push_back(const T &__x)
test(list, push_back1) {
  list<int> li = {1, 2, 3, 4};
  int x = 33;
  li.push_back(x);
  eq(li.size(), 5);
  seq(li, "[1, 2, 3, 4, 33]");
}

// void push_front(T &&__x)
test(list, push_front0) {
  list<int> li = {1, 2, 3, 4};
  li.push_front(33);
  eq(li.size(), 5);
  seq(li, "[33, 1, 2, 3, 4]");
}

// void push_front(const T &__x)
test(list, push_front1) {
  list<int> li = {1, 2, 3, 4};
  int x = 33;
  li.push_front(x);
  eq(li.size(), 5);
  seq(li, "[33, 1, 2, 3, 4]");
}

// reverse_iterator rbegin()
// const_reverse_iterator rbegin() const
test(list, rbegin) {
  list<int> li = {1, 2, 3, 4};
  eq(*li.rbegin(), 4);
  *li.rbegin() = 44;
  seq(li, "[1, 2, 3, 44]");
}

// // size_type remove(const T &__value)
// test(list, remove) {
//   list<foo> li = {{1, "Aa"}, {2, "Bb"}, {1, "Aa"}, {3, "Cc"}, {4, "Dd"}};
//   li.remove(li.front());
//   eq(li.size(), 3);
// }

// // size_type remove_if(_Predicate __pred)
// test(list, remove_if) {
//   list<foo> li = {{1, "Aa"}, {2, "Bb"}, {1, "Aaa"}, {3, "Cc"}, {4, "Dd"}};
//   li.remove_if([](const foo &__x) { return __x.value == 1; });
//   eq(li.size(), 3);
// }

// reverse_iterator rend()
// const_reverse_iterator rend() const
test(list, rend) {
  list<int> li = {1, 2, 3, 4};
  eq(*--li.rend(), 1);
  *--li.rend() = 11;
  seq(li, "[11, 2, 3, 4]");
}

// void resize(size_type __new_size)
test(list, resize0) {
  list<int> li = {1, 2, 3};
  li.resize(5);
  eq(li.size(), 5);
  seq(li, "[1, 2, 3, 0, 0]");
  li.resize(2);
  eq(li.size(), 2);
  seq(li, "[1, 2]");
}

// void resize(size_type __new_size, const T &__x)
test(list, resize1) {
  list<int> li = {1, 2, 3};
  li.resize(5, 10);
  eq(li.size(), 5);
  seq(li, "[1, 2, 3, 10, 10]");
  li.resize(2, 20);
  eq(li.size(), 2);
  seq(li, "[1, 2]");
}

// // void reverse()
// test(list, reverse) {
//   list<int> li;
//   li.reverse();
//   seq(li, "[]");

//   li = {1};
//   li.reverse();
//   seq(li, "[1]");

//   li = {1, 2};
//   li.reverse();
//   seq(li, "[2, 1]");

//   li = {1, 2, 3};
//   li.reverse();
//   seq(li, "[3, 2, 1]");
// }

// size_type size() const
test(list, size) {
  list<int> li = {1, 2, 3};
  eq(li.size(), 3);
}

// // void sort()
// test(list, sort0) {
//   list<int> li = {1, 3, 2, 7, 8, 6, 9, 5, 0, 4};
//   li.sort();
//   seq(li, "[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]");
// }

// // void sort(_StrictWeakOrdering __comp)
// test(list, sort1) {
//   list<int> li = {1, 3, 2, 7, 8, 6, 9, 5, 0, 4};
//   li.sort([](int x, int y) { return x > y; });
//   seq(li, "[9, 8, 7, 6, 5, 4, 3, 2, 1, 0]");
// }

// void splice(const_iterator __position, list &__x)
test(list, splice0) {
  list<int> li0 = {1, 2, 3, 4, 5};
  list<int> li1 = {11, 12, 13};
  li0.splice(++li0.begin(), li1);
  seq(li0, "[1, 11, 12, 13, 2, 3, 4, 5]");
  seq(li1, "[]");
}

// void splice(const_iterator __position, list &&__x)
test(list, splice1) {
  list<int> li = {1, 2, 3, 4, 5};
  li.splice(++li.begin(), list({11, 12, 13}));
  seq(li, "[1, 11, 12, 13, 2, 3, 4, 5]");
}

// void splice(const_iterator __position, list &__x, const_iterator __i)
test(list, splice2) {
  list<int> li0 = {1, 2, 3, 4, 5};
  list<int> li1 = {11, 12, 13};
  li0.splice(li0.begin(), li0, ++li0.begin());
  seq(li0, "[2, 1, 3, 4, 5]");
  li0.splice(++ ++li0.begin(), li1, li1.begin());
  seq(li0, "[2, 1, 11, 3, 4, 5]");
  seq(li1, "[12, 13]");
}

// void splice(const_iterator __position, list &&__x, const_iterator __i)
test(list, splice3) {
  list<int> li0 = {1, 2, 3, 4, 5};
  list<int> li1 = {11, 12, 13};
  li0.splice(li0.begin(), std::move(li0), ++li0.begin());
  seq(li0, "[2, 1, 3, 4, 5]");
  li0.splice(++ ++li0.begin(), std::move(li1), li1.begin());
  seq(li0, "[2, 1, 11, 3, 4, 5]");
  seq(li1, "[12, 13]");
}

// void splice(const_iterator __position, list &__x,
//               const_iterator __first, const_iterator __last)
test(list, splice4) {
  list<int> li0 = {1, 2, 3, 4, 5};
  list<int> li1 = {11, 12, 13};
  li0.splice(li0.begin(), li0, ++li0.begin(), li0.end());
  seq(li0, "[2, 3, 4, 5, 1]");
  li0.splice(++ ++li0.begin(), li1, li1.begin(), --li1.end());
  seq(li0, "[2, 3, 11, 12, 4, 5, 1]");
  seq(li1, "[13]");
}

// void splice(const_iterator __position, list &&__x,
//               const_iterator __first, const_iterator __last)
test(list, splice5) {
  list<int> li0 = {1, 2, 3, 4, 5};
  list<int> li1 = {11, 12, 13};
  li0.splice(li0.begin(), std::move(li0), ++li0.begin(), li0.end());
  seq(li0, "[2, 3, 4, 5, 1]");
  li0.splice(++ ++li0.begin(), std::move(li1), li1.begin(), --li1.end());
  seq(li0, "[2, 3, 11, 12, 4, 5, 1]");
  seq(li1, "[13]");
}

// void swap(list &__x)
test(list, swap) {
  list<int> li0;
  list<int> li1;

  li0.swap(li1);
  seq(li0, "[]");
  seq(li1, "[]");

  li1 = {1, 2, 3};
  li0.swap(li1);
  seq(li0, "[1, 2, 3]");
  seq(li1, "[]");

  li0.swap(li1);
  seq(li0, "[]");
  seq(li1, "[1, 2, 3]");

  li0 = {11, 12, 13};
  li0.swap(li1);
  seq(li0, "[1, 2, 3]");
  seq(li1, "[11, 12, 13]");
}

// // size_type unique()
// test(list, unique0) {
//   list<int> li = {1, 1, 4, 5, 5, 5, 8};
//   li.unique();
//   seq(li, "[1, 4, 5, 8]");
// }

// // size_type unique(_BinaryPredicate __pred)
// test(list, unique1) {
//   list<int> li = {1, 1, 4, 20, 50, 53, 81};
//   li.unique([](int a, int b) { return a / 10 == b / 10; });
//   seq(li, "[1, 20, 50, 81]");
// }