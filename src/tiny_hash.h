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

template <class _Key, class _Val, class _Hash, class _ExtractKey, 
          class _EqualKey, class _Alloc = tinySTL::allocator<_Val>>
class hashtable;

template <class _Key, class _Val, class _Hash, class _ExtractKey, 
          class _EqualKey, class _Alloc>
struct _Hashtable_iterator;

template <class _Key, class _Val, class _Hash, class _ExtractKey, 
          class _EqualKey, class _Alloc>
struct _Hashtable_const_iterator;

template <class _Key, class _Val, class _Hash, class _ExtractKey, 
          class _EqualKey, class _Alloc>
struct _Hashtable_iterator 
{
  typedef _Hashtable_node<_Val> _Node;
  typedef hashtable<_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc> 
          _Hashtable;
  typedef _Hashtable_iterator
          <_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc>
          iterator;
  typedef _Hashtable_const_iterator
          <_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc>
          const_iterator;

  typedef forward_iterator_tag iterator_category;
  typedef _Val value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef _Val& reference;
  typedef _Val* pointer;

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

template <class _Key, class _Val, class _Hash, class _ExtractKey, 
          class _EqualKey, class _Alloc>
struct _Hashtable_const_iterator 
{
  typedef _Hashtable_node<_Val> _Node;
  typedef hashtable<_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc> 
          _Hashtable;
  typedef _Hashtable_iterator
          <_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc>
          iterator;
  typedef _Hashtable_const_iterator
          <_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc>
          const_iterator;

  typedef forward_iterator_tag iterator_category;
  typedef _Val value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef const _Val& reference;
  typedef const _Val* pointer;

  const _Node* _M_cur;
  const _Hashtable* _M_ht;

  _Hashtable_const_iterator() : _M_cur(0), _M_ht(0) {}

  _Hashtable_const_iterator(const _Node* __n, const _Hashtable* __tab)
    : _M_cur(__n), _M_ht(__tab) {}

  _Hashtable_const_iterator(const iterator& __it) 
    : _M_cur(__it._M_cur), _M_ht(__it._M_ht) {}

  reference operator*() const { return _M_cur->_M_val; }

  pointer operator->() const { return &(operator*()); }

  const_iterator& operator++();

  const_iterator operator++(int);

  bool operator==(const const_iterator& __it) const 
    { return _M_cur == __it._M_cur; }
  
  bool operator!=(const const_iterator& __it) const 
    { return _M_cur != __it._M_cur; }
};

// Note: assumes long is at least 32 bits.
enum { __tiny_num_primes = 28 };

static const unsigned long __tiny_prime_list[__tiny_num_primes] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
  1610612741ul, 3221225473ul, 4294967291ul
};

#include <algorithm> // TODO: remove.
inline unsigned long __tiny_next_prime(unsigned long __n)
{
  const unsigned long* __first = __tiny_prime_list;
  const unsigned long* __last = __tiny_prime_list + (int)__tiny_num_primes;
  // TODO: std::lower_bound -> tinySTL::lower_bound.
  const unsigned long* pos = std::lower_bound(__first, __last, __n);
  return pos == __last ? *(__last - 1) : *pos;
}


template <class _Key, class _Val, class _Hash, class _ExtractKey, class _EqualKey, class _Alloc>
class hashtable {
public:
  typedef _Key key_type;
  typedef _Val value_type;
  typedef _Hash hasher;
  typedef _EqualKey key_equal;

  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;

  hasher hash_func() const { return _M_hash; }
  key_equal key_eq() const { return _M_equals; }

private:
  typedef _Hashtable_node<_Val> _Node;
  typedef typename _Alloc_rebind<_Alloc, _Node>::type _Node_allocator;

public:
  typedef _Alloc allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }
  _Node* _M_get_node() { return _M_alloc.allocate(1); }
  void _M_put_node(_Node* __p) { _M_alloc.deallocate(__p, 1); }

  typedef _Hashtable_iterator<_Val, _Key, _Hash, _ExtractKey, _EqualKey, _Alloc>
          iterator;
  typedef _Hashtable_const_iterator<_Val, _Key, _Hash, _ExtractKey, _EqualKey, _Alloc>
          const_iterator;

  friend struct
  _Hashtable_iterator<_Val, _Key, _Hash, _ExtractKey, _EqualKey, _Alloc>;
  friend struct
  _Hashtable_const_iterator<_Val, _Key, _Hash, _ExtractKey, _EqualKey, _Alloc>;

private:
  typedef typename _Alloc_rebind<_Alloc, _Node*>::type _Bkt_allocator;

private:
  _Node_allocator _M_alloc;
  key_equal       _M_equals;
  _ExtractKey     _M_get_key;
  hasher          _M_hash;
  vector<_Node*, _Bkt_allocator> _M_buckets;
  size_type       _M_num_elements;

public:
  hashtable(size_type __n,
            const _Hash&       __hf,
            const _EqualKey&   __eql,
            const _ExtractKey& __ext,
            const allocator_type& __a = allocator_type())
    : _M_alloc(__a),
      _M_equals(__eql),
      _M_get_key(__ext),
      _M_hash(__hf),
      _M_buckets(__a),
      _M_num_elements(0)
  {
    _M_initialize_buckets(__n);
  }

  hashtable(size_type __n,
            const _Hash&       __hf,
            const _EqualKey&   __eql,
            const allocator_type& __a = allocator_type())
    : _M_alloc(__a),
      _M_equals(__eql),
      _M_get_key(_ExtractKey()),
      _M_hash(__hf),
      _M_buckets(__a),
      _M_num_elements(0)
  {
    _M_initialize_buckets(__n);
  }

  hashtable(const hashtable& __ht)
    : _M_alloc(__ht.get_allocator()),
      _M_equals(__ht._M_equals),
      _M_get_key(__ht._M_get_key),
      _M_hash(__ht._M_hash),
      _M_buckets(__ht.get_allocator()),
      _M_num_elements(0)
  {
    _M_copy_from(__ht);
  }

  hashtable(hashtable&& __ht);

  ~hashtable() { clear(); }

  hashtable& operator=(const hashtable& __ht)
  {
    if (&__ht != this) {
      clear();
      _M_hash = __ht._M_hash;
      _M_equals = __ht._M_equals;
      _M_get_key = __ht._M_get_key;
      _M_copy_from(__ht);
    }
    return *this;
  }

  hashtable& operator=(hashtable&& __ht); // TODO.

  size_type size() const { return _M_num_elements; }

  size_type max_size() const { return size_type(-1); }

  bool empty() const { return size() == 0; }

  void swap();

  iterator begin();

  const_iterator begin() const;

  iterator end();

  const_iterator end() const;

  size_type bucket_count() const { return _M_buckets.size(); }

  size_type max_bucket_count() const
  { return __tiny_prime_list[(int)__tiny_num_primes - 1]; } 

  size_type elems_in_bucket(size_type __bucket) const 
  {
    size_type __result = 0;
    for (_Node* __cur = _M_buckets[__bucket]; __cur; __cur = __cur->_M_next)
      __result += 1;
    return __result;
  }

  // TODO: insert and emplace.

  pair<iterator, bool> insert_unique(const value_type& __obj);

  iterator insert_equal(const value_type& __obj);

  pair<iterator, bool> insert_unique_noresize(const value_type& __obj);
  
  iterator insert_equal_noresize(const value_type& __obj);

  reference find_or_insert(const value_type& __obj);

  iterator find(const key_type& __key);

  const_iterator find(const key_type& __key) const;

  size_type count(const key_type& __key) const;

  pair<iterator, iterator> 
  equal_range(const key_type& __key);

  pair<const_iterator, const_iterator> 
  equal_range(const key_type& __key) const;

  size_type erase(const key_type& __key);

  void erase(const iterator& __it);

  void erase(iterator __first, iterator __last);

  void erase(const const_iterator& __it);

  void erase(const_iterator __first, const_iterator __last);

  void resize(size_type __num_elements_hint);
   
  void clear() 
  {
    for (size_type __i = 0; __i < _M_buckets.size(); ++__i) {
      _Node* __cur = _M_buckets[__i];
      while (__cur != 0) {
        _Node* __next = __cur->_M_next;
        _M_delete_node(__cur);
        __cur = __next;
      }
      _M_buckets[__i] = 0;
    }
    _M_num_elements = 0;
  }

  // TODO: operator==.

private:
  size_type _M_next_size(size_type __n) const
    { return __tiny_next_prime(__n); }

  void _M_initialize_buckets(size_type __n) 
  {
    const size_type __n_buckets = _M_next_size(__n);
    _M_buckets.reserve(__n_buckets);
    _M_buckets.insert(_M_buckets.end(), __n_buckets, (_Node*) 0);
    _M_num_elements = 0;
  }

  void _M_copy_from(const hashtable& __ht) 
  {
    _M_buckets.clear();
    _M_buckets.reserve(__ht._M_buckets.size());
    _M_buckets.insert(_M_buckets.end(), __ht._M_buckets.size(), (_Node*)0);
    try {
      for (size_type __i = 0; __i < __ht._M_buckets.size(); ++__i) {
        const _Node* __cur = __ht._M_buckets[__i];
        if (__cur) {
          _Node* __copy = _M_new_node(__cur->_M_val);
          _M_buckets[__i] = __copy;

          for (_Node* __next = __cur->_M_next;
               __next != nullptr;
               __cur = __next, __next = __cur->_M_next) {
            __copy->_M_next = _M_new_node(__next->_M_val);
            __copy = __copy->_M_next;
          }
        }
      }
      _M_num_elements = __ht._M_num_elements;
    } catch (...) {
      clear();
      throw;
    }
  }

  _Node* _M_new_node(const value_type& __obj)
  {
    _Node* __n = _M_get_node();
    __n->_M_next = 0;
    try {
      tinySTL::construct(&__n->_M_val, __obj);
      return __n;
    } catch (...) {
      _M_put_node(__n);
      throw;
    }
  }

  void _M_delete_node(_Node* __n)
  {
    tinySTL::destroy(&__n->_M_val);
    _M_put_node(__n);
  }
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
  const int a = sizeof(tinySTL::hashtable<int, int, std::hash<int>, tinySTL::_Identity<int>, Eq__>);
  const int b = sizeof(std::unordered_set<int>);
  const int c = sizeof(tinySTL::vector<int>);
  const int d = sizeof(std::unordered_map<int, int>);
  std::set<int> s;
  return 0;
}