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
  aligned_membuf<_Tp> _M_storage;
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

  reference operator*() const { return *operator->(); }

  pointer operator->() const { return _M_cur->_M_storage.ptr(); }

  iterator& operator++() 
  {
    const _Node* __old = _M_cur;
    _M_cur = _M_cur->_M_next;
    if (!_M_cur) {
      size_type __bucket = _M_ht->_M_bkt_num(*__old->_M_storage.ptr());
      while (!_M_cur && ++__bucket < _M_ht->_M_buckets.size())
        _M_cur = _M_ht->_M_buckets[__bucket];
    }
    return *this;
  }

  iterator operator++(int) 
  {
    iterator __tmp = *this;
    ++*this;
    return __tmp;
  }

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

  iterator constCast() const {
    return iterator((_Node*)_M_cur, (_Hashtable*)_M_ht);
  }

  reference operator*() const { return *operator->(); }

  pointer operator->() const { return _M_cur->_M_storage.ptr(); }

  const_iterator& operator++() 
  {
    const _Node* __old = _M_cur;
    _M_cur = _M_cur->_M_next;
    if (!_M_cur) {
      size_type __bucket = _M_ht->_M_bkt_num(*__old->_M_storage.ptr());
      while (!_M_cur && ++__bucket < _M_ht->_M_buckets.size())
        _M_cur = _M_ht->_M_buckets[__bucket];
    }
    return *this;
  }

  const_iterator operator++(int) 
  {
    const_iterator __tmp = *this;
    ++*this;
    return __tmp;
  }

  bool operator==(const const_iterator& __it) const 
    { return _M_cur == __it._M_cur; }
  
  bool operator!=(const const_iterator& __it) const 
    { return _M_cur != __it._M_cur; }
};

// Note: assumes long is at least 32 bits.
enum { __tiny_num_primes = 32 };

static const unsigned long __tiny_prime_list[__tiny_num_primes] =
{
  2ul,          5ul,          11ul,         23ul,
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

template<class, class, class, class> friend class unordered_set;
template<class, class, class, class> friend class unordered_multiset;
template<class, class, class, class, class> friend class unordered_map;
template<class, class, class, class, class> friend class unordered_multimap;
template<class, class, class, class, class, class> friend class hashtable;
template<class, class, class, class, class, class> friend class _Hashtable_iterator;
template<class, class, class, class, class, class> friend class _Hashtable_const_iterator;

protected:
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
  typedef _Hashtable_node<value_type> _Node;
  typedef typename _Alloc_rebind<_Alloc, _Node>::type _Node_allocator;

protected:
  typedef _Alloc allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }
  _Node* _M_get_node() { return _M_alloc.allocate(1); }
  void _M_put_node(_Node* __p) { _M_alloc.deallocate(__p, 1); }

  typedef _Hashtable_iterator<_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc>
          iterator;
  typedef _Hashtable_const_iterator<_Key, _Val, _Hash, _ExtractKey, _EqualKey, _Alloc>
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
  float           _M_factor = 0.75f;
  vector<_Node*, _Bkt_allocator> _M_buckets;
  size_type       _M_num_elements;

protected:
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

  hashtable(hashtable&& __ht) 
    : _M_alloc(tinySTL::move(__ht._M_alloc)),
      _M_equals(tinySTL::move(__ht._M_equals)),
      _M_get_key(tinySTL::move(__ht._M_get_key)),
      _M_hash(tinySTL::move(__ht._M_hash)),
      _M_buckets(tinySTL::move(__ht._M_buckets)),
      _M_num_elements(__ht._M_num_elements) 
  {
    __ht._M_num_elements = 0;
  }

  ~hashtable() { clear(); }

  hashtable& operator=(const hashtable& __ht)
  {
    if (&__ht != this) {
      clear();
      _M_alloc = __ht._M_alloc;
      _M_hash = __ht._M_hash;
      _M_equals = __ht._M_equals;
      _M_get_key = __ht._M_get_key;
      _M_copy_from(__ht);
    }
    return *this;
  }

  hashtable& operator=(hashtable&& __ht) 
  {
    if (&__ht != this) {
      clear();
      _M_alloc = tinySTL::move(__ht._M_alloc);
      _M_equals = tinySTL::move(__ht._M_equals);
      _M_get_key = tinySTL::move(__ht._M_get_key);
      _M_hash = tinySTL::move(__ht._M_hash);
      _M_buckets = tinySTL::move(__ht._M_buckets);
      _M_num_elements = __ht._M_num_elements;
      __ht._M_num_elements = 0;
    }
    return *this;
  }

  size_type size() const { return _M_num_elements; }

  size_type max_size() const { return size_type(-1); }

  bool empty() const { return size() == 0; }

  void swap(hashtable& __ht) 
  {
    tinySTL::swap(_M_hash, __ht._M_hash);
    tinySTL::swap(_M_equals, __ht._M_equals);
    tinySTL::swap(_M_get_key, __ht._M_get_key);
    _M_buckets.swap(__ht._M_buckets);
    tinySTL::swap(_M_num_elements, __ht._M_num_elements);
  }

  iterator begin() 
  {
    for (size_type __n = 0; __n < _M_buckets.size(); ++__n) {
      if (_M_buckets[__n]) {
        return iterator(_M_buckets[__n], this);
      }
    }
    return end();
  }

  const_iterator begin() const 
  {
    for (size_type __n = 0; __n < _M_buckets.size(); ++__n) {
      if (_M_buckets[__n]) {
        return const_iterator(_M_buckets[__n], this);
      }
    }
    return end();
  }

  const_iterator cbegin() const { return begin(); }

  iterator end() { return iterator(0, this); }

  const_iterator end() const { return const_iterator(0, this); }

  const_iterator cend() const { return end(); }

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

  template <class _Arg>
  tinySTL::pair<iterator, bool>
  _M_insert_unique(_Arg&& __x) 
  {
    _M_ensure(_M_num_elements + 1);
    return _M_insert_unique_noresize(tinySTL::forward<_Arg>(__x));
  }

  template <class _Arg> iterator
  _M_insert_equal(_Arg&& __x) 
  {
    _M_ensure(_M_num_elements + 1);
    return _M_insert_equal_noresize(__x);
  }

  template <class _Arg> pair<iterator, bool> 
  _M_insert_unique_noresize(_Arg&& __obj) 
  {
    const size_type __n = _M_bkt_num(__obj);
    _Node* __first = _M_buckets[__n];

    for (_Node* __cur = __first; __cur; __cur = __cur->_M_next) 
      if (_M_equals(_M_get_key(*__cur->_M_storage.ptr()), _M_get_key(__obj)))
        return pair<iterator, bool>(iterator(__cur, this), false);

    _Node* __tmp = _M_new_node(tinySTL::forward<_Arg>(__obj));
    __tmp->_M_next = __first;
    _M_buckets[__n] = __tmp;
    ++_M_num_elements;
    return pair<iterator, bool>(iterator(__tmp, this), true);
  }
  
  template <class _Arg> iterator 
  _M_insert_equal_noresize(_Arg&& __obj) 
  {
    const size_type __n = _M_bkt_num(__obj);
    _Node* __first = _M_buckets[__n];

    for (_Node* __cur = __first; __cur; __cur = __cur->_M_next) 
      if (_M_equals(_M_get_key(*__cur->_M_storage.ptr()), _M_get_key(__obj))) {
        _Node* __tmp = _M_new_node(tinySTL::forward<_Arg>(__obj));
        __tmp->_M_next = __cur->_M_next;
        __cur->_M_next = __tmp;
        ++_M_num_elements;
        return iterator(__tmp, this);
      }

    _Node* __tmp = _M_new_node(tinySTL::forward<_Arg>(__obj));
    __tmp->_M_next = __first;
    _M_buckets[__n] = __tmp;
    ++_M_num_elements;
    return iterator(__tmp, this);
  }

  iterator _M_insert_unique(iterator __position, const value_type& __x) 
  { return _M_insert_unique(__x).first; }

  iterator _M_insert_unique(iterator __position, const value_type&& __x) 
  { return _M_insert_unique(__x).first; }

  iterator _M_insert_equal(iterator __position, const value_type& __x)
  { return _M_insert_equal(__x); }

  iterator _M_insert_equal(iterator __position, value_type&& __x)
  { return _M_insert_equal(__x); }


  tinySTL::pair<iterator, bool>
  _M_insert_node_unique(_Node* __node) 
  {
    _M_ensure(_M_num_elements + 1);
    value_type* valptr = __node->_M_storage.ptr();
    const size_type __n = _M_bkt_num(*valptr);
    _Node* __first = _M_buckets[__n];

    for (_Node* __cur = __first; __cur; __cur = __cur->_M_next) 
      if (_M_equals(_M_get_key(*__cur->_M_storage.ptr()), _M_get_key(*valptr)))
        return pair<iterator, bool>(iterator(__cur, this), false);
    _Node* __tmp = __node;
    __tmp->_M_next = __first;
    _M_buckets[__n] = __tmp;
    ++_M_num_elements;
    return pair<iterator, bool>(iterator(__tmp, this), true);
  }

  iterator _M_insert_node_equal(_Node* __node)
  {
    _M_ensure(_M_num_elements + 1);
    value_type* valptr = __node->_M_storage.ptr();
    const size_type __n = _M_bkt_num(*valptr);
    _Node* __first = _M_buckets[__n];

    for (_Node* __cur = __first; __cur; __cur = __cur->_M_next) 
      if (_M_equals(_M_get_key(*__cur->_M_storage.ptr()), _M_get_key(*valptr))) {
        _Node* __tmp = __node;
        __tmp->_M_next = __cur->_M_next;
        __cur->_M_next = __tmp;
        ++_M_num_elements;
        return iterator(__tmp, this);
      }

    _Node* __tmp = __node;
    __tmp->_M_next = __first;
    _M_buckets[__n] = __tmp;
    ++_M_num_elements;
    return iterator(__tmp, this);
  }

  template <class... _Args>
  tinySTL::pair<iterator, bool>
  _M_emplace_unique(_Args&&... __args) 
  {
    return _M_insert_unique(value_type(tinySTL::forward<_Args>(__args)...));
  }

  template <class... _Args> iterator
  _M_emplace_equal(_Args&&... __args) 
  {
    return _M_insert_equal(value_type(tinySTL::forward<_Args>(__args)...));
  }

  template <class... _Args> iterator
  _M_emplace_hint_unique(iterator __position, _Args&&... __args) 
  {
    return _M_insert_unique(__position, value_type(tinySTL::forward<_Args>(__args)...));
  }

  template <class... _Args> iterator
  _M_emplace_hint_equal(iterator __position, _Args&&... __args) 
  {
    return _M_insert_equal(__position, value_type(tinySTL::forward<_Args>(__args)...));
  }

  iterator find(const key_type& __key) 
  {
    size_type __n = _M_bkt_num_key(__key);
    _Node* __first;
    for ( __first = _M_buckets[__n];
          __first && !_M_equals(_M_get_key(*__first->_M_storage.ptr()), __key);
          __first = __first->_M_next)
      {}
    return iterator(__first, this);
  }

  const_iterator find(const key_type& __key) const 
  {
    size_type __n = _M_bkt_num_key(__key);
    const _Node* __first;
    for ( __first = _M_buckets[__n];
          __first && !_M_equals(_M_get_key(*__first->_M_storage.ptr()), __key);
          __first = __first->_M_next)
      {}
    return const_iterator(__first, this);
  } 

  size_type count_unique(const key_type& __key) const 
  {
    const size_type __n = _M_bkt_num_key(__key);
    for (const _Node* __cur = _M_buckets[__n]; __cur; __cur = __cur->_M_next)
      if (_M_equals(_M_get_key(*__cur->_M_storage.ptr()), __key)) {
        return 1;
      } 
    return 0;
  }

  size_type count_equal(const key_type& __key) const 
  {
    const size_type __n = _M_bkt_num_key(__key);
    size_type __result = 0;
    bool __match = false;
    for (const _Node* __cur = _M_buckets[__n]; __cur; __cur = __cur->_M_next)
      if (_M_equals(_M_get_key(*__cur->_M_storage.ptr()), __key)) {
        ++__result;
        __match = true;
      } else if (__match) {
        break;
      }
    return __result;
  }

  pair<iterator, iterator> 
  equal_range(const key_type& __key) 
  {
    typedef pair<iterator, iterator> _Pii;
    const size_type __n = _M_bkt_num_key(__key);

    for (_Node* __first = _M_buckets[__n]; __first; __first = __first->_M_next)
      if (_M_equals(_M_get_key(*__first->_M_storage.ptr()), __key)) {
        for (_Node* __cur = __first->_M_next; __cur; __cur = __cur->_M_next)
          if (!_M_equals(_M_get_key(*__cur->_M_storage.ptr()), __key))
            return _Pii(iterator(__first, this), iterator(__cur, this));
        for (size_type __m = __n + 1; __m < _M_buckets.size(); ++__m)
          if (_M_buckets[__m])
            return _Pii(iterator(__first, this),
                      iterator(_M_buckets[__m], this));
        return _Pii(iterator(__first, this), end());
      }
    return _Pii(end(), end());
  }

  pair<const_iterator, const_iterator> 
  equal_range(const key_type& __key) const 
  {
    typedef pair<const_iterator, const_iterator> _Pii;
    const size_type __n = _M_bkt_num_key(__key);

    for (const _Node* __first = _M_buckets[__n] ;
        __first; 
        __first = __first->_M_next) {
      if (_M_equals(_M_get_key(*__first->_M_storage.ptr()), __key)) {
        for (const _Node* __cur = __first->_M_next;
            __cur;
            __cur = __cur->_M_next)
          if (!_M_equals(_M_get_key(*__cur->_M_storage.ptr()), __key))
            return _Pii(const_iterator(__first, this),
                        const_iterator(__cur, this));
        for (size_type __m = __n + 1; __m < _M_buckets.size(); ++__m)
          if (_M_buckets[__m])
            return _Pii(const_iterator(__first, this),
                        const_iterator(_M_buckets[__m], this));
        return _Pii(const_iterator(__first, this), end());
      }
    }
    return _Pii(end(), end());
  }

  size_type erase(const key_type& __key) 
  {
    const size_type __n = _M_bkt_num_key(__key);
    _Node* __first = _M_buckets[__n];
    size_type __erased = 0;

    if (__first) {
      _Node* __cur = __first;
      _Node* __next = __cur->_M_next;
      while (__next) {
        if (_M_equals(_M_get_key(*__next->_M_storage.ptr()), __key)) {
          __cur->_M_next = __next->_M_next;
          _M_delete_node(__next);
          __next = __cur->_M_next;
          ++__erased;
          --_M_num_elements;
        }
        else {
          __cur = __next;
          __next = __cur->_M_next;
        }
      }
      if (_M_equals(_M_get_key(*__first->_M_storage.ptr()), __key)) {
        _M_buckets[__n] = __first->_M_next;
        _M_delete_node(__first);
        ++__erased;
        --_M_num_elements;
      }
    }
    return __erased;
  }

  iterator erase(const iterator& __it) 
  {
    _Node* __p = __it._M_cur;
    if (__p) {
      const size_type __n = _M_bkt_num(*__p->_M_storage.ptr());
      _Node* __cur = _M_buckets[__n];
      iterator __it = iterator(__cur, this);
      ++__it;
      if (__cur == __p) {
        _M_buckets[__n] = __cur->_M_next;
        _M_delete_node(__cur);
        --_M_num_elements;
      } else {
        _Node* __next = __cur->_M_next;
        while (__next) {
          if (__next == __p) {
            __cur->_M_next = __next->_M_next;
            _M_delete_node(__next);
            --_M_num_elements;
            break;
          } else {
            __cur = __next;
            __next = __cur->_M_next;
          }
        }
      }
      return __it;
    }
    __tiny_throw_range_error("unordered_set: erase");
    return end();
  }

  iterator erase(iterator __first, iterator __last) 
  {
    size_type __f_bucket = __first._M_cur ? 
      _M_bkt_num(*(__first._M_cur->_M_storage.ptr())) : _M_buckets.size();
    size_type __l_bucket = __last._M_cur ? 
      _M_bkt_num(*(__last._M_cur->_M_storage.ptr())) : _M_buckets.size();

    if (__first._M_cur == __last._M_cur)
      return __last;
    else if (__f_bucket == __l_bucket)
      _M_erase_bucket(__f_bucket, __first._M_cur, __last._M_cur);
    else {
      _M_erase_bucket(__f_bucket, __first._M_cur, 0);
      for (size_type __n = __f_bucket + 1; __n < __l_bucket; ++__n)
        _M_erase_bucket(__n, 0);
      if (__l_bucket != _M_buckets.size())
        _M_erase_bucket(__l_bucket, __last._M_cur);
    }
    return __last;
  }

  void erase(const const_iterator& __it) 
  {
    erase(iterator(const_cast<_Node*>(__it._M_cur),
                  const_cast<hashtable*>(__it._M_ht)));
  }

  void erase(const_iterator __first, const_iterator __last) 
  {
    erase(iterator(const_cast<_Node*>(__first._M_cur),
                  const_cast<hashtable*>(__first._M_ht)),
          iterator(const_cast<_Node*>(__last._M_cur),
                  const_cast<hashtable*>(__last._M_ht)));
  }
   
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
          _Node* __copy = _M_new_node(*__cur->_M_storage.ptr());
          _M_buckets[__i] = __copy;

          for (_Node* __next = __cur->_M_next;
               __next != nullptr;
               __cur = __next, __next = __cur->_M_next) {
            __copy->_M_next = _M_new_node(*__next->_M_storage.ptr());
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
      tinySTL::construct(__n->_M_storage.ptr(), __obj);
      return __n;
    } catch (...) {
      _M_put_node(__n);
      throw;
    }
  }

  void _M_delete_node(_Node* __n)
  {
    tinySTL::destroy(__n->_M_storage.ptr());
    _M_put_node(__n);
  }

  size_type _M_bkt_num_key(const key_type& __key) const
  {
    return _M_bkt_num_key(__key, _M_buckets.size());
  }

  size_type _M_bkt_num(const value_type& __obj) const
  {
    return _M_bkt_num_key(_M_get_key(__obj));
  }

  size_type _M_bkt_num_key(const key_type& __key, size_t __n) const
  {
    return _M_hash(__key) % __n;
  }

  size_type _M_bkt_num(const value_type& __obj, size_t __n) const
  {
    return _M_bkt_num_key(_M_get_key(__obj), __n);
  }

  void _M_erase_bucket(const size_type __n, _Node* __first, _Node* __last)
  {
    _Node* __cur = _M_buckets[__n];
    if (__cur == __first)
      _M_erase_bucket(__n, __last);
    else {
      _Node* __next;
      for (__next = __cur->_M_next; 
          __next != __first; 
          __cur = __next, __next = __cur->_M_next)
        ;
      while (__next != __last) {
        __cur->_M_next = __next->_M_next;
        _M_delete_node(__next);
        __next = __cur->_M_next;
        --_M_num_elements;
      }
    }
  }

  void _M_erase_bucket(const size_type __n, _Node* __last)
  {
    _Node* __cur = _M_buckets[__n];
    while (__cur != __last) {
      _Node* __next = __cur->_M_next;
      _M_delete_node(__cur);
      __cur = __next;
      _M_buckets[__n] = __cur;
      --_M_num_elements;
    }
  }

  void _M_rehash(size_type __nhint) 
  {
    const size_type __n = _M_next_size(__nhint);
    if (__n == bucket_count()) {
      return;
    }
    if ((__n * _M_factor) >= _M_num_elements) {
      decltype(_M_buckets) 
      __tmp(__n, (_Node*)(0), _M_buckets.get_allocator());
      size_type __old_n = bucket_count();
      try {
        for (size_type __bucket = 0; __bucket < __old_n; ++__bucket) {
          _Node* __first = _M_buckets[__bucket];
          while (__first) {
            size_type __new_bucket = _M_bkt_num(*__first->_M_storage.ptr(), __n);
            _M_buckets[__bucket] = __first->_M_next;
            __first->_M_next = __tmp[__new_bucket];
            __tmp[__new_bucket] = __first;
            __first = _M_buckets[__bucket];
          }
        }
        _M_buckets.swap(__tmp);
      } catch(...) {
        for (size_type __bucket = 0; __bucket < __tmp.size(); ++__bucket) {
          while (__tmp[__bucket]) {
            _Node* __next = __tmp[__bucket]->_M_next;
            _M_delete_node(__tmp[__bucket]);
            __tmp[__bucket] = __next;
          }
        }
        throw;
      }
    }
  }

  void _M_reserve(size_type __nhint) {
    size_type __n = _M_next_size(__nhint);
    _M_buckets.reserve(__n);
  }

  void _M_ensure(size_type __num_elements_hint) 
  {
    const size_type __old_n = _M_buckets.size();
    if (__num_elements_hint > __old_n * _M_factor) {
      const size_type __n = _M_next_size(__num_elements_hint/_M_factor);
      if (__n > __old_n) {
        decltype(_M_buckets) __tmp(__n, (_Node*)(0),
                                   _M_buckets.get_allocator());
        try {
          for (size_type __bucket = 0; __bucket < __old_n; ++__bucket) {
            _Node* __first = _M_buckets[__bucket];
            while (__first) {
              size_type __new_bucket = _M_bkt_num(*__first->_M_storage.ptr(), __n);
              _M_buckets[__bucket] = __first->_M_next;
              __first->_M_next = __tmp[__new_bucket];
              __tmp[__new_bucket] = __first;
              __first = _M_buckets[__bucket];
            }
          }
          _M_buckets.swap(__tmp);
        } catch(...) {
          for (size_type __bucket = 0; __bucket < __tmp.size(); ++__bucket) {
            while (__tmp[__bucket]) {
              _Node* __next = __tmp[__bucket]->_M_next;
              _M_delete_node(__tmp[__bucket]);
              __tmp[__bucket] = __next;
            }
          }
          throw;
        }
      }
    }
  }

  void _M_reset() {
    for (size_type i = 0; i < bucket_count(); i++) {
      _M_buckets[i] = 0;
    }
    _M_num_elements = 0;
  }

  template <class _EqualKey2>
  void _M_merge_unique(hashtable<_Key, _Val, _Hash, _ExtractKey, _EqualKey2, _Alloc>&& __table) 
  {
    if (__table.empty()) {
      return;
    }
    hashtable<_Key, _Val, _Hash, _ExtractKey, _EqualKey2, _Alloc> __tmp(0, hasher(), _EqualKey2(), allocator_type());

    size_type __num_buckets_x = __table.bucket_count();
    size_type __num_elements_x = __table.size();
    for (size_type i = 0; __num_elements_x > 0 && i < __num_buckets_x; i++) {
      _Node* __cur = __table._M_buckets[i];
      while (__cur) {
        _Node* __next = __cur->_M_next;
        --__num_elements_x;
        auto [it, ok] = _M_insert_node_unique(__cur);
        if (!ok) {
          __tmp._M_insert_node_equal(__cur);
        }
        __cur = __next;
      }
    }
    __table._M_reset();
    __table = tinySTL::move(__tmp);
  }

  template <class _EqualKey2>
  void _M_merge_equal(hashtable<_Key, _Val, _Hash, _ExtractKey, _EqualKey2, _Alloc>&& __table) 
  {
    if (__table.empty()) {
      return;
    }
    size_type __num_buckets_x = __table.bucket_count();
    size_type __num_elements_x = __table.size();
    for (size_type i = 0; __num_elements_x > 0 && i < __num_buckets_x; i++) {
      _Node* __cur = __table._M_buckets[i];
      while (__cur) {
        _Node* __next = __cur->_M_next;
        --__num_elements_x;
        _M_insert_node_equal(__cur);
        __cur = __next;
      }
    }
    __table._M_reset();
  }
};

}
