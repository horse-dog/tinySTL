#pragma once

#include "vector.h"
#include "tiny_alloc.h"
#include "tiny_iterator.h"

namespace tinySTL 
{

template <class _Tp>
struct _Hashtable_node
{
  _Hashtable_node* _M_next;
  _Tp _M_val; // TODO: memory buff ?.
};

template <class _Tp, class _Hash, class _Equal, class _Alloc = tinySTL::allocator<_Tp>>
class hashtable;


template <class _Tp, class _Hash, class _Equal, class _Alloc = tinySTL::allocator<_Tp>>
struct _Hashtable_iterator 
{
  typedef _Hashtable_node<_Tp> _Node;
  typedef hashtable<_Tp, _Hash, _Equal, _Alloc> _Hashtable;
  typedef _Hashtable_iterator<_Tp, _Hash, _Equal, _Alloc>
          iterator;

  typedef forward_iterator_tag iterator_category;
  typedef _Tp value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef _Tp& reference;
  typedef _Tp* pointer;

  _Node* _M_cur;
  _Hashtable* _M_ht;

  _Hashtable_iterator() : _M_cur(0), _M_ht(0) {}

  _Hashtable_iterator(_Node* __n, _Hashtable* __tab) 
    : _M_cur(__n), _M_ht(__tab) {}

  reference operator*() const { return _M_cur->_M_val; }

  pointer operator->() const { return &(operator*()); }

  iterator& operator++();

  iterator operator++(int);

  bool operator==(const iterator& __it) const
  { return _M_cur == __it._M_cur; }

  bool operator!=(const iterator& __it) const
  { return _M_cur != __it._M_cur; }
};

// Note: assumes long is at least 32 bits.
enum { __stl_num_primes = 28 };

static const unsigned long __stl_prime_list[__stl_num_primes] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
  1610612741ul, 3221225473ul, 4294967291ul
};

#include <algorithm> // TODO: remove.
inline unsigned long __stl_next_prime(unsigned long __n)
{
  const unsigned long* __first = __stl_prime_list;
  const unsigned long* __last = __stl_prime_list + (int)__stl_num_primes;
  // TODO: std::lower_bound -> tinySTL::lower_bound.
  const unsigned long* pos = std::lower_bound(__first, __last, __n);
  return pos == __last ? *(__last - 1) : *pos;
}


template <class _Tp, class _Hash, class _Equal, class _Alloc>
class hashtable {
public:
  typedef _Tp value_type;
  typedef _Hash hasher;
  typedef _Equal key_equal;

  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;

  hasher hash_func() const { return _M_hash; }
  key_equal key_eq() const { return _M_equals; }

private:
  typedef _Hashtable_node<_Tp> _Node;
  typedef typename _Alloc_rebind<_Alloc, _Node>::type _Node_allocator;

public:
  typedef _Alloc allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }
  _Node* _M_get_node() { return _M_alloc.allocate(1); }
  void _M_put_node(_Node* __p) { _M_alloc.deallocate(__p, 1); }

  typedef _Hashtable_iterator<_Tp, _Hash, _Equal, _Alloc>
          iterator;

private:
  _Node_allocator       _M_alloc;
  key_equal             _M_equals;
  hasher                _M_hash;
  vector<_Node*>        _M_buckets; // TODO: alloc ?.
  size_type             _M_num_elements;
};

}

#include <set>
#include <unordered_set>
#include <unordered_map>
#include "tiny_function.h"

class Eq__ {

bool operator()(const int& a, const int& b) const {
  return a == b;
}

};

int xx__x() {
  const int a = sizeof(tinySTL::hashtable<int, std::hash<int>, Eq__>);
  const int b = sizeof(std::unordered_set<int>);
  const int c = sizeof(tinySTL::vector<int>);
  const int d = sizeof(std::unordered_map<int, int>);
  std::set<int> s;
  return 0;
}