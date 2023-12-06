#pragma once

// TODO: replace by deque.
#include "vector.h"

namespace tinySTL
{

template <typename _Tp, typename _Sequence = vector<_Tp>>
class stack {
 public:
  using value_type = typename _Sequence::value_type;
  using reference = typename _Sequence::reference;
  using const_reference = typename _Sequence::const_reference;
  using size_type = typename _Sequence::size_type;
  using container_type = _Sequence;

 protected:
  container_type c;

 public:
  stack() : c() {}

  stack(const stack& __q) : c(__q.c) {}

  stack(stack&& __q) : c(tinySTL::move(__q.c)) {}
  
  explicit
  stack(const container_type& __c) : c(__c) {}

  explicit
  stack(container_type&& __c) : c(tinySTL::move(__c)) {}

 public:
  template <typename... _Args>
  auto emplace(_Args&& ...__args)
    { return c.emplace_back(std::forward<_Args>(__args)...); }

  bool empty() const
    { return c.empty(); }

  void pop()
    { c.pop_back(); }

  void push(const_reference __x)
    { c.push_back(__x); }

  void push(value_type&& __x)
    { c.push_back(tinySTL::move(__x)); }

  size_type size() const
    { return c.size(); }

  void swap(stack& __s)
	  { tinySTL::swap(c, __s.c); }

  const_reference top() const
    { return c.back(); }

  reference top()
    { return c.back(); }

  friend bool operator==(const stack& __x, const stack& __y)
    { return operator==(__x.c, __y.c); }

  friend bool operator!=(const stack& __x, const stack& __y)
    { return !(__x == __y); }
};

}