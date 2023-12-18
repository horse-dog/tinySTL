#pragma once

#include "deque.h"
#include "vector.h"
#include "tiny_heap.h"

namespace tinySTL {
template <typename _Tp, typename _Sequence = deque<_Tp>>
class queue {
 public:
  using value_type = typename _Sequence::value_type;
  using reference = typename _Sequence::reference;
  using const_reference = typename _Sequence::const_reference;
  using size_type = typename _Sequence::size_type;
  using container_type = _Sequence;

 protected:
  container_type c;

 public:
  queue() : c() {}

  queue(const queue& __q) : c(__q.c) {}

  queue(queue&& __q) : c(tinySTL::move(__q.c)) {}

  explicit
  queue(const container_type& __c) : c(__c) {}

  explicit
  queue(container_type&& __c) : c(tinySTL::move(__c)) {}

 public:
  const_reference back() const
  { return c.back(); }

  reference back()
  { return c.back(); }

  template <class... _Args>
  auto emplace(_Args&& ...__args)
  { return c.emplace_back(tinySTL::forward<_Args>(__args)...); }

  bool empty() const
  { return c.empty(); }

  const_reference front() const
  { return c.front(); }

  reference front()
  { return c.front(); }

  void pop()
  { c.pop_front(); }

  void push(const_reference __x)
  { c.push_back(__x); }

  void push(value_type&& __x)
  { c.push_back(tinySTL::move(__x)); }

  size_type size() const
  { return c.size(); }

  void swap(queue& __s)
  { tinySTL::swap(c, __s.c); }

  friend auto operator<=>(const queue& __x, const queue& __y) 
  { return operator<=>(__x.c, __y.c); }

  friend bool operator==(const queue& __x, const queue& __y)
  { return operator==(__x.c, __y.c); }

};

template < class _Tp, class _Sequence = vector<_Tp>, class _Cp = void >
class priority_queue {
 protected:
  template <class _Tx, class _Ty>
  struct _Cp_traits { using type = _Ty; };

  template <class _Tx>
  struct _Cp_traits<_Tx, void> { 
    struct _Less { 
      bool operator() 
      (const _Tp& a, const _Tp& b) const
      { return a < b; }
    }; 
    using type = _Less;
  };

  using _Compare = typename _Cp_traits<_Tp, _Cp>::type;

 public:
  using value_type = typename  _Sequence::value_type;
  using reference = typename _Sequence::reference;
  using const_reference = typename _Sequence::const_reference;
  using size_type = typename _Sequence::size_type;
  using container_type = _Sequence;
  using value_compare = _Compare;

 protected:
  _Sequence  c;
  _Compare   comp;

 public:
  priority_queue()
  : c(), comp() { }

  explicit
  priority_queue(const _Sequence& __s)
  : c(__s), comp()
  { tinySTL::make_heap(c.begin(), c.end(), comp); }

  explicit
  priority_queue(_Sequence&& __c)
  : c(tinySTL::move(__c)), comp()
  { tinySTL::make_heap(c.begin(), c.end(), comp); }

  priority_queue(const priority_queue& __q)
  : c(__q.c), comp(__q.comp) { }

  priority_queue(priority_queue&& __q)
  : c(tinySTL::move(__q.c)), comp(tinySTL::move(__q.comp)) { }

  template<Iterable _InputIterator>
  priority_queue(_InputIterator __first, _InputIterator __last,
           const _Compare& __x,
           const _Sequence& __s)
  : c(__s), comp(__x)
  { 
    c.insert(c.end(), __first, __last);
    tinySTL::make_heap(c.begin(), c.end(), comp);
  }

  template<Iterable _InputIterator>
  priority_queue(_InputIterator __first, _InputIterator __last,
           const _Compare& __x = _Compare(),
           _Sequence&& __s = _Sequence())
  : c(tinySTL::move(__s)), comp(__x)
  { 
    c.insert(c.end(), __first, __last);
    tinySTL::make_heap(c.begin(), c.end(), comp);
  }

 public:
  template <class... _Args>
  auto emplace(_Args&& ...__args)
  { 
    c.emplace_back(tinySTL::forward<_Args>(__args)...);
    tinySTL::push_heap(c.begin(), c.end(), comp);
  }

  bool empty() const
  { return c.empty(); }

  void pop()
  { 
    tinySTL::pop_heap(c.begin(), c.end(), comp);
    c.pop_back();
  }

  void push(const_reference __x)
  { 
    c.push_back(__x);
    tinySTL::push_heap(c.begin(), c.end(), comp);
  }

  void push(value_type&& __x)
  { 
    c.push_back(tinySTL::move(__x));
    tinySTL::push_heap(c.begin(), c.end(), comp);
  }

  size_type size() const
  { return c.size(); }

  void swap(priority_queue& __pq)
  { tinySTL::swap(c, __pq.c); }

  const_reference top() const
  { return c.front(); }

};

}