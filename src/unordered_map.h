#pragma once

#include "tiny_hash.h"
#include "tiny_function.h"

namespace tinySTL
{

template<class, class, class, class, class> class unordered_multimap;

template<class _Key, class _Val, class _Hash = std::hash<_Key>, class _Pred = tinySTL::equal_to<_Key>, class _Alloc = tinySTL::allocator<tinySTL::pair<_Key, _Val>>>
class unordered_map {

template<class, class, class, class, class> friend class unordered_map;
template<class, class, class, class, class> friend class unordered_multimap;

public:
  typedef _Key    key_type;
  typedef tinySTL::pair<const _Key, _Val> value_type;
  typedef _Pred   key_equal;
  typedef _Alloc  allocator_type;

protected:
  typedef tinySTL::hashtable<key_type, value_type, _Hash, _Select1st<value_type>, _Pred, _Alloc> _Ht;
  _Ht _M_ht;

public:
  typedef typename _Ht::hasher hasher;
  typedef typename _Ht::pointer pointer;
  typedef typename _Ht::const_pointer const_pointer;
  typedef typename _Ht::reference reference;
  typedef typename _Ht::const_reference const_reference;
  typedef typename _Ht::iterator iterator;
  typedef typename _Ht::const_iterator const_iterator;
  typedef typename _Ht::size_type size_type;
  typedef typename _Ht::difference_type difference_type;

public:
  unordered_map() 
  : _M_ht(0, hasher(), key_equal(), allocator_type()) {}

  unordered_map(const unordered_map&) = default;

  unordered_map(unordered_map&&) = default;

  unordered_map(std::initializer_list<value_type> __l) 
  : _M_ht(__l.size(), hasher(), key_equal(), allocator_type()) 
  { insert(__l); }

  template <InputIterator Iterator>
  unordered_map(Iterator __first, Iterator __last)
  : _M_ht(0, hasher(), key_equal(), allocator_type()) 
  { insert(__first, __last); }

  ~unordered_map() {}

  unordered_map& operator=(const unordered_map&) = default;

  unordered_map& operator=(unordered_map&&) = default;

  unordered_map& operator=(std::initializer_list<value_type> __l) 
  {
    clear();
    insert(__l);
    return *this;
  }

  iterator begin() { return _M_ht.begin(); }

  const_iterator begin() const { return _M_ht.cbegin(); }
  
  const_iterator cbegin() const { return _M_ht.begin(); }
  
  iterator end() { return _M_ht.end(); }
  
  const_iterator end() const { return _M_ht.cend(); }
  
  const_iterator cend() const { return _M_ht.end(); }

  size_type bucket(const key_type& __x) const 
  { return _M_ht._M_bkt_num_key(__x); }

  size_type bucket_count() const 
  { return _M_ht.bucket_count(); }

  size_type bucket_size(size_t __n) const 
  { return _M_ht.elems_in_bucket(__n); }
  
  void clear() { _M_ht.clear(); }

  bool contains(const key_type& __x) const 
  { return find(__x) != end(); }

  size_type count(const key_type& __x) const 
  { return _M_ht.count_unique(__x); }

  template<typename... _Args> tinySTL::pair<iterator, bool>
  emplace(_Args &&...__args) 
  { return _M_ht._M_emplace_unique(tinySTL::forward<_Args>(__args)...); }

  template<typename... _Args> iterator
  emplace_hint(const_iterator __hint, _Args &&...__args) 
  { return _M_ht._M_emplace_hint_unique(__hint.constCast(), tinySTL::forward<_Args>(__args)...); }

  bool empty() const 
  { return _M_ht.empty(); }

  tinySTL::pair<iterator, iterator>
  equal_range(const key_type& __x) 
  { return _M_ht.equal_range(__x); }

  tinySTL::pair<const_iterator, const_iterator>
  equal_range(const key_type& __x) const 
  { return _M_ht.equal_range(__x); }

  size_type erase(const key_type& __k) 
  { return _M_ht.erase(__k); }

  iterator erase(const_iterator __p) 
  { return _M_ht.erase(__p.constCast()); }

  iterator erase(const_iterator __first, const_iterator __last) 
  { return _M_ht.erase(__first.constCast(), __last.constCast()); }

  const_iterator find(const key_type& __x) const 
  { return _M_ht.find(__x); }

  iterator find(const key_type& __x) 
  { return _M_ht.find(__x); }

  allocator_type get_allocator() const { return allocator_type{}; }
  
  hasher hash_function() const { return _M_ht.hash_func(); }

  tinySTL::pair<iterator, bool>
  insert(const value_type& __x) 
  { return _M_ht._M_insert_unique(__x); }

  tinySTL::pair<iterator, bool>
  insert(value_type&& __x) 
  { return _M_ht._M_insert_unique(tinySTL::move(__x)); }

  iterator insert(const_iterator __hint, const value_type& __x) 
  { return _M_ht._M_insert_unique(__hint.constCast(), __x); }

  iterator insert(const_iterator __hint, value_type&& __x) 
  { return _M_ht._M_insert_unique(__hint.constCast(), tinySTL::move(__x)); }

  template<typename _InputIterator> void
  insert(_InputIterator __first, _InputIterator __last) 
  {
    for (; __first != __last; ++__first) {
      insert(*__first);
    }
  }

  void insert(std::initializer_list<value_type> __l) 
  { insert(__l.begin(), __l.end()); }
  
  key_equal key_eq() const { return _M_ht.key_eq(); }

  float load_factor() const { return size()/(float)bucket_count(); }

  size_type max_bucket_count() const { return _M_ht.max_bucket_count(); }

  void max_load_factor(float __mlf) 
  { 
    _M_ht._M_factor = __mlf; 
    _M_ht._M_rehash(_M_ht.size());
  }

  float max_load_factor() const { return _M_ht._M_factor; }
  
  size_type max_size() const { return _M_ht.max_size(); }

  template <class _Pred2>
  void merge(unordered_map<_Key, _Val, _Hash, _Pred2, _Alloc>& __source) 
  { return merge(tinySTL::move(__source)); }

  template <class _Pred2>
  void merge(unordered_map<_Key, _Val, _Hash, _Pred2, _Alloc>&& __source) 
  { _M_ht._M_merge_unique(tinySTL::move(__source._M_ht)); }

  template <class _Pred2>
  void merge(unordered_multimap<_Key, _Val, _Hash, _Pred2, _Alloc>& __source) 
  { return merge(tinySTL::move(__source)); }

  template <class _Pred2>
  void merge(unordered_multimap<_Key, _Val, _Hash, _Pred2, _Alloc>&& __source)
  { _M_ht._M_merge_unique(tinySTL::move(__source._M_ht)); }

  void rehash(size_t __n) { _M_ht._M_rehash(__n); }

  void reserve(size_t __n) { _M_ht._M_reserve(__n); }
  
  size_type size() const { return _M_ht.size(); }

  void swap(unordered_map& __u) { _M_ht.swap(__u._M_ht); }

  friend bool operator==(const unordered_map& __x, const unordered_map& __y) 
  {
    if (&__x != &__y) {
      return __x.size() == __y.size() 
        && __x.bucket_count() == __y.bucket_count()
        && tinySTL::equal(__x.begin(), __x.end(), __y.begin());
    } else {
      return true;
    }
  }

  _Val& operator[](const key_type& __k) {
    iterator __i = find(__k);
    if (__i == end()) {
      __i = insert(__i, value_type(__k, _Val()));
    }
    return (*__i).second;
  }

  const _Val& operator[](const key_type& __k) const {
    const_iterator __i = find(__k);
    if (__i == end()) {
      __tiny_throw_range_error("map::operator[] const: key not found");
    }
    return (*__i).second;
  }

  _Val& at(const key_type& __k) 
  {
    return this->operator[](__k);
  }

  const _Val& at(const key_type& __k) const 
  {
    try {
      return this->operator[](__k);
    } catch (...) {
      __tiny_throw_range_error("map::at() const: key not found");
    }
  }
};

template<class _Key, class _Val, class _Hash = std::hash<_Key>, class _Pred = tinySTL::equal_to<_Key>, class _Alloc = tinySTL::allocator<tinySTL::pair<_Key, _Val>>>
class unordered_multimap {

template<class, class, class, class, class> friend class unordered_map;
template<class, class, class, class, class> friend class unordered_multimap;

public:
  typedef _Key    key_type;
  typedef tinySTL::pair<const _Key, _Val> value_type;
  typedef _Pred   key_equal;
  typedef _Alloc  allocator_type;

protected:
  typedef tinySTL::hashtable<key_type, value_type, _Hash, _Select1st<value_type>, _Pred, _Alloc> _Ht;
  _Ht _M_ht;

public:
  typedef typename _Ht::hasher hasher;
  typedef typename _Ht::pointer pointer;
  typedef typename _Ht::const_pointer const_pointer;
  typedef typename _Ht::reference reference;
  typedef typename _Ht::const_reference const_reference;
  typedef typename _Ht::iterator iterator;
  typedef typename _Ht::const_iterator const_iterator;
  typedef typename _Ht::size_type size_type;
  typedef typename _Ht::difference_type difference_type;

public:
  unordered_multimap() 
  : _M_ht(0, hasher(), key_equal(), allocator_type()) {}

  unordered_multimap(const unordered_multimap&) = default;

  unordered_multimap(unordered_multimap&&) = default;

  unordered_multimap(std::initializer_list<value_type> __l) 
  : _M_ht(__l.size(), hasher(), key_equal(), allocator_type()) 
  { insert(__l); }

  template <InputIterator Iterator>
  unordered_multimap(Iterator __first, Iterator __last)
  : _M_ht(0, hasher(), key_equal(), allocator_type()) 
  { insert(__first, __last); }

  ~unordered_multimap() {}

  unordered_multimap& operator=(const unordered_multimap&) = default;

  unordered_multimap& operator=(unordered_multimap&&) = default;

  unordered_multimap& operator=(std::initializer_list<value_type> __l) 
  {
    clear();
    insert(__l);
    return *this;
  }

  iterator begin() { return _M_ht.begin(); }

  const_iterator begin() const { return _M_ht.cbegin(); }
  
  const_iterator cbegin() const { return _M_ht.begin(); }
  
  iterator end() { return _M_ht.end(); }
  
  const_iterator end() const { return _M_ht.cend(); }
  
  const_iterator cend() const { return _M_ht.end(); }

  size_type bucket(const key_type& __x) const 
  { return _M_ht._M_bkt_num_key(__x); }

  size_type bucket_count() const 
  { return _M_ht.bucket_count(); }

  size_type bucket_size(size_t __n) const 
  { return _M_ht.elems_in_bucket(__n); }
  
  void clear() { _M_ht.clear(); }

  bool contains(const key_type& __x) const 
  { return find(__x) != end(); }

  size_type count(const key_type& __x) const 
  { return _M_ht.count_equal(__x); }

  template<typename... _Args> iterator
  emplace(_Args &&...__args) 
  { return _M_ht._M_emplace_equal(tinySTL::forward<_Args>(__args)...); }

  template<typename... _Args> iterator
  emplace_hint(const_iterator __hint, _Args &&...__args) 
  { return _M_ht._M_emplace_hint_equal(__hint.constCast(), tinySTL::forward<_Args>(__args)...); }

  bool empty() const 
  { return _M_ht.empty(); }

  tinySTL::pair<iterator, iterator>
  equal_range(const key_type& __x) 
  { return _M_ht.equal_range(__x); }

  tinySTL::pair<const_iterator, const_iterator>
  equal_range(const key_type& __x) const 
  { return _M_ht.equal_range(__x); }

  size_type erase(const key_type& __k) 
  { return _M_ht.erase(__k); }

  iterator erase(const_iterator __p) 
  { return _M_ht.erase(__p.constCast()); }

  iterator erase(const_iterator __first, const_iterator __last) 
  { return _M_ht.erase(__first.constCast(), __last.constCast()); }

  const_iterator find(const key_type& __x) const 
  { return _M_ht.find(__x); }

  iterator find(const key_type& __x) 
  { return _M_ht.find(__x); }

  allocator_type get_allocator() const { return allocator_type{}; }
  
  hasher hash_function() const { return _M_ht.hash_func(); }

  iterator insert(const value_type& __x) 
  { return _M_ht._M_insert_equal(__x); }

  iterator insert(value_type&& __x) 
  { return _M_ht._M_insert_equal(tinySTL::move(__x)); }

  iterator insert(const_iterator __hint, const value_type& __x) 
  { return _M_ht._M_insert_equal(__hint.constCast(), __x); }

  iterator insert(const_iterator __hint, value_type&& __x) 
  { return _M_ht._M_insert_equal(__hint.constCast(), tinySTL::move(__x)); }

  template<typename _InputIterator> void
  insert(_InputIterator __first, _InputIterator __last) 
  {
    for (; __first != __last; ++__first) {
      insert(*__first);
    }
  }

  void insert(std::initializer_list<value_type> __l) 
  { insert(__l.begin(), __l.end()); }
  
  key_equal key_eq() const { return _M_ht.key_eq(); }

  float load_factor() const { return size()/(float)bucket_count(); }

  size_type max_bucket_count() const { return _M_ht.max_bucket_count(); }

  void max_load_factor(float __mlf) 
  { 
    _M_ht._M_factor = __mlf; 
    _M_ht._M_rehash(_M_ht.size());
  }

  float max_load_factor() const { return _M_ht._M_factor; }
  
  size_type max_size() const { return _M_ht.max_size(); }


  template <class _Pred2>
  void merge(unordered_map<_Key, _Val, _Hash, _Pred2, _Alloc>& __source) 
  { return merge(tinySTL::move(__source)); }

  template <class _Pred2>
  void merge(unordered_map<_Key, _Val, _Hash, _Pred2, _Alloc>&& __source) 
  { _M_ht._M_merge_equal(tinySTL::move(__source._M_ht)); }

  template <class _Pred2>
  void merge(unordered_multimap<_Key, _Val, _Hash, _Pred2, _Alloc>& __source) 
  { return merge(tinySTL::move(__source)); }

  template <class _Pred2>
  void merge(unordered_multimap<_Key, _Val, _Hash, _Pred2, _Alloc>&& __source)
  { _M_ht._M_merge_equal(tinySTL::move(__source._M_ht)); }

  void rehash(size_t __n) { _M_ht._M_rehash(__n); }

  void reserve(size_t __n) { _M_ht._M_reserve(__n); }
  
  size_type size() const { return _M_ht.size(); }

  void swap(unordered_multimap& __u) { _M_ht.swap(__u._M_ht); }

  friend bool operator==(const unordered_multimap& __x, const unordered_multimap& __y) 
  {
    if (&__x != &__y) {
      return __x.size() == __y.size() 
        && __x.bucket_count() == __y.bucket_count()
        && tinySTL::equal(__x.begin(), __x.end(), __y.begin());
    } else {
      return true;
    }
  }
};

}