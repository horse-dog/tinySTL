#pragma once

#include "tiny_hash.h"
#include "tiny_function.h"

namespace tinySTL
{

template<class _Key, class _Hash = std::hash<_Key>, 
         class _Pred = tinySTL::equal_to<_Key>, 
         class _Alloc = tinySTL::allocator<_Key>>
class unordered_set {

 protected:
  typedef tinySTL::hashtable<_Key, _Key, _Hash, _Identity<_Key>, _Pred, _Alloc>
          _Ht;

  _Ht _M_ht;

 public:
  typedef typename _Ht::key_type key_type;
  typedef typename _Ht::value_type value_type;
  typedef typename _Ht::hasher hasher;
  typedef typename _Ht::key_equal key_equal;
  typedef typename _Ht::size_type size_type;
  typedef typename _Ht::difference_type difference_type;
  typedef typename _Ht::const_pointer pointer;
  typedef typename _Ht::const_pointer const_pointer;
  typedef typename _Ht::const_reference reference;
  typedef typename _Ht::const_reference const_reference;

  typedef typename _Ht::const_iterator iterator;
  typedef typename _Ht::const_iterator const_iterator;

  typedef typename _Ht::allocator_type allocator_type;

 public:
  unordered_set() 
  : _M_ht(0, hasher(), key_equal(), allocator_type()) {}

  unordered_set(const unordered_set&) = default;

  unordered_set(unordered_set&&) = default;

  unordered_set(std::initializer_list<value_type> __l) 
  : _M_ht(__l.size(), hasher(), key_equal(), allocator_type()) 
  { insert(__l); }

  template <InputIterator Iterator>
  unordered_set(Iterator __first, Iterator __last)
  : _M_ht(0, hasher(), key_equal(), allocator_type()) 
  { insert(__first, __last); }

  ~unordered_set() {}

  unordered_set& operator=(const unordered_set&) = default;

  unordered_set& operator=(unordered_set&&) = default;

  unordered_set& operator=(std::initializer_list<value_type> __l) 
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

  // TODO
  // merge.

  void rehash(size_t __n) { _M_ht._M_rehash(__n); }

  void reserve(size_t __n) { _M_ht._M_reserve(__n); }
  
  size_type size() const { return _M_ht.size(); }

  void swap(unordered_set& __u) { _M_ht.swap(__u._M_ht); }

  friend bool operator==(const unordered_set& __x, const unordered_set& __y) 
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

// class unordered_multiset; // TODO.

}