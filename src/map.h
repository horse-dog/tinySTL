#pragma once

#include "tiny_tree.h"
#include "tiny_pair.h"
#include "tiny_alloc.h"
#include "tiny_errors.h"
#include "tiny_concepts.h"
#include "tiny_iterator.h"
#include "tiny_function.h"
#include "tiny_uninitialized.h"

namespace tinySTL
{

template<class, class, class, class> class multimap;

template<class _Key, class _Val, class _Compare = less<_Key>, class _Alloc = alloc>
class map {

template<class, class, class, class> friend class map;
template<class, class, class, class> friend class multimap;

 public:
  typedef _Key     key_type;
  typedef tinySTL::pair<const _Key, _Val> value_type;
  typedef _Compare key_compare;
  typedef _Compare value_compare; // TODO.
  typedef _Alloc   allocator_type;

 protected:
  typedef tinySTL::_Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare>
            _Rep_type;
  _Rep_type _M_t;
 
 public:
  typedef typename _Rep_type::pointer pointer;
  typedef typename _Rep_type::const_pointer const_pointer;
  typedef typename _Rep_type::reference reference;
  typedef typename _Rep_type::const_reference const_reference;
  typedef typename _Rep_type::iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;

 public:
  map() : _M_t() {}

  map(const map&) = default;

  map(map&&) = default;

  map(std::initializer_list<value_type> __l) : _M_t() 
  { insert(__l); }

  template <InputIterator Iterator>
  map(Iterator __first, Iterator __last) : _M_t() 
  { insert(__first, __last); }

  ~map() {}

  map& operator=(const map&) = default;

  map& operator=(map&&) = default;

  map& operator=(std::initializer_list<value_type> __l) 
  {
    clear();
    insert(__l);
    return *this;
  }

  key_compare key_comp() const { return _M_t.key_comp(); }

  value_compare value_comp() const { return _M_t.key_comp(); }

 public:
  allocator_type get_allocator() const { return _M_t._M_node_allocator; }

  iterator begin() { return _M_t.begin(); }

  const_iterator begin() const { return _M_t.begin(); }
 
  const_iterator cbegin() const { return _M_t.begin(); }

  iterator end() { return _M_t.end(); }

  const_iterator end() const { return _M_t.end(); }

  const_iterator cend() const { return _M_t.end(); }

  reverse_iterator rbegin() { return _M_t.rbegin(); }

  const_reverse_iterator rbegin() const { return _M_t.rbegin(); }

  const_reverse_iterator crbegin() const { return _M_t.rbegin(); }

  reverse_iterator rend() { return _M_t.rend(); }

  const_reverse_iterator rend() const { return _M_t.rend(); }

  const_reverse_iterator crend() const { return _M_t.rend(); }

  bool empty() const { return _M_t.size() == 0; }

  size_type size() const { return _M_t.size(); }

  size_type max_size() const { return _M_t.max_size(); }

  void swap(map& __x) { tinySTL::swap(*this, __x); }

  _Val& operator[](const key_type& __k) {
    iterator __i = lower_bound(__k);
    if (__i == end() || key_comp()(__k, (*__i).first)) {
      __i = insert(__i, value_type(__k, _Val()));
    }
    return (*__i).second;
  }

  const _Val& operator[](const key_type& __k) const {
    const_iterator __i = lower_bound(__k);
    if (__i == end() || key_comp()(__k, (*__i).first)) {
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

  template<typename... _Args> tinySTL::pair<iterator, bool>
  emplace(_Args&&... __args) { return _M_t._M_emplace_unique(tinySTL::forward<_Args>(__args)...); }

  template<typename... _Args> iterator
  emplace_hint(const_iterator __hint, _Args &&...__args) 
  { return _M_t._M_emplace_hint_unique(__hint.base(), tinySTL::forward<_Args>(__args)...); }

  tinySTL::pair<iterator, bool>
  insert(const value_type& __x) { return _M_t._M_insert_unique(__x); }

  tinySTL::pair<iterator, bool>
  insert(value_type&& __x) { return _M_t._M_insert_unique(tinySTL::move(__x)); }

  iterator insert(const_iterator __hint, const value_type& __x) 
  { return _M_t._M_insert_unique(__hint.base(), __x); }

  iterator insert(const_iterator __hint, value_type&& __x) 
  { return _M_t._M_insert_unique(__hint.base(), tinySTL::move(__x)); }

  template<typename _InputIterator> void
  insert(_InputIterator __first, _InputIterator __last) 
  {
    iterator it = end();
    for (; __first != __last; ++__first) {
      it = insert(it, *__first);
    }
  }

  void insert(std::initializer_list<value_type> __l) {
    insert(__l.begin(), __l.end());
  }

  size_type erase(const key_type& __k) 
  { return _M_t.erase(__k); }

  iterator erase(const_iterator __position) 
  { return _M_t.erase(__position.base()); }

  iterator erase(const_iterator __first, const_iterator __last) 
  { return _M_t.erase(__first.base(), __last.base()); }

  void clear() { _M_t.clear(); }

  size_type count(const key_type& __x) const { return _M_t.count_unique(__x); }

  bool contains(const key_type& __x) const { return find(__x) != end(); }

  iterator find(const key_type& __x) { return _M_t.find(__x); }

  const_iterator find(const key_type& __x) const { return _M_t.find(__x); }

  iterator lower_bound(const key_type& __x) { return _M_t.lower_bound(__x); }

  const_iterator lower_bound(const key_type& __x) const { return _M_t.lower_bound(__x); }

  iterator upper_bound(const key_type& __x) { return _M_t.upper_bound(__x); }
  
  const_iterator upper_bound(const key_type& __x) const { return _M_t.upper_bound(__x); }

  tinySTL::pair<iterator, iterator>
  equal_range(const key_type& __x) 
  { return _M_t.equal_range(__x); }

  tinySTL::pair<const_iterator, const_iterator>
  equal_range(const key_type& __x) const 
  { return _M_t.equal_range(__x); }

  void disp(std::ostream& os) { return _M_t.disp(os); }

  template <class _Compare2>
  void merge(map<_Key, _Val, _Compare2, _Alloc>& __source) 
  { return merge(tinySTL::move(__source)); }

  template <class _Compare2>
  void merge(map<_Key, _Val, _Compare2, _Alloc>&& __source) 
  { _M_t._M_merge_unique(tinySTL::move(__source._M_t)); }

  template <class _Compare2>
  void merge(multimap<_Key, _Val, _Compare2, _Alloc>& __source) 
  { return merge(tinySTL::move(__source)); }

  template <class _Compare2>
  void merge(multimap<_Key, _Val, _Compare2, _Alloc>&& __source)
  { _M_t._M_merge_unique(tinySTL::move(__source._M_t)); }

  friend bool operator==(const map& __x, const map& __y) 
  {
    return __x.size() == __y.size()
    && tinySTL::equal(__x.begin(), __x.end(), __y.begin());
  }
  friend bool operator<(const map& __x, const map& __y) 
  {
    return lexicographical_compare(
      __x.begin(), __x.end(),
      __y.begin(), __y.end()
    );
  }

  friend std::ostream& operator<<(std::ostream& os, const map& s) 
  { return os << s._M_t; }

};

template<class _Key, class _Val, class _Compare = less<_Key>, class _Alloc = alloc>
class multimap {
 
template<class, class, class, class> friend class map;
template<class, class, class, class> friend class multimap;

 public:
  typedef _Key     key_type;
  typedef tinySTL::pair<const _Key, _Val> value_type;
  typedef _Compare key_compare;
  typedef _Compare value_compare; // TODO.
  typedef _Alloc   allocator_type;

 protected:
  typedef tinySTL::_Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare>
            _Rep_type;
  _Rep_type _M_t;
 
 public:
  typedef typename _Rep_type::pointer pointer;
  typedef typename _Rep_type::const_pointer const_pointer;
  typedef typename _Rep_type::reference reference;
  typedef typename _Rep_type::const_reference const_reference;
  typedef typename _Rep_type::iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;

 public:
  multimap() : _M_t() {}

  multimap(const multimap&) = default;

  multimap(multimap&&) = default;

  multimap(std::initializer_list<value_type> __l) : _M_t() 
  { insert(__l); }

  template <InputIterator Iterator>
  multimap(Iterator __first, Iterator __last) : _M_t() 
  { insert(__first, __last); }

  ~multimap() {}

  multimap& operator=(const multimap&) = default;

  multimap& operator=(multimap&&) = default;

  multimap& operator=(std::initializer_list<value_type> __l) 
  {
    clear();
    insert(__l);
    return *this;
  }

  key_compare key_comp() const { return _M_t.key_comp(); }

  value_compare value_comp() const { return _M_t.key_comp(); }

 public:
  allocator_type get_allocator() const { return _M_t._M_node_allocator; }

  iterator begin() { return _M_t.begin(); }

  const_iterator begin() const { return _M_t.begin(); }
 
  const_iterator cbegin() const { return _M_t.begin(); }

  iterator end() { return _M_t.end(); }

  const_iterator end() const { return _M_t.end(); }

  const_iterator cend() const { return _M_t.end(); }

  reverse_iterator rbegin() { return _M_t.rbegin(); }

  const_reverse_iterator rbegin() const { return _M_t.rbegin(); }

  const_reverse_iterator crbegin() const { return _M_t.rbegin(); }

  reverse_iterator rend() { return _M_t.rend(); }

  const_reverse_iterator rend() const { return _M_t.rend(); }

  const_reverse_iterator crend() const { return _M_t.rend(); }

  bool empty() const { return _M_t.size() == 0; }

  size_type size() const { return _M_t.size(); }

  size_type max_size() const { return _M_t.max_size(); }

  void swap(multimap& __x) { tinySTL::swap(*this, __x); }

  template<typename... _Args> tinySTL::pair<iterator, bool>
  emplace(_Args&&... __args) { return _M_t._M_emplace_equal(tinySTL::forward<_Args>(__args)...); }

  template<typename... _Args> iterator
  emplace_hint(const_iterator __hint, _Args &&...__args) 
  { return _M_t._M_emplace_hint_equal(__hint.base(), tinySTL::forward<_Args>(__args)...); }

  tinySTL::pair<iterator, bool>
  insert(const value_type& __x) { return _M_t._M_insert_equal(__x); }

  tinySTL::pair<iterator, bool>
  insert(value_type&& __x) { return _M_t._M_insert_equal(tinySTL::move(__x)); }

  iterator insert(const_iterator __hint, const value_type& __x) 
  { return _M_t._M_insert_equal(__hint.base(), __x); }

  iterator insert(const_iterator __hint, value_type&& __x) 
  { return _M_t._M_insert_equal(__hint.base(), tinySTL::move(__x)); }

  template<typename _InputIterator> void
  insert(_InputIterator __first, _InputIterator __last) 
  {
    iterator it = end();
    for (; __first != __last; ++__first) {
      it = insert(it, *__first);
    }
  }

  void insert(std::initializer_list<value_type> __l) {
    insert(__l.begin(), __l.end());
  }

  size_type erase(const key_type& __k) 
  { return _M_t.erase(__k); }

  iterator erase(const_iterator __position) 
  { return _M_t.erase(__position.base()); }

  iterator erase(const_iterator __first, const_iterator __last) 
  { return _M_t.erase(__first.base(), __last.base()); }

  void clear() { _M_t.clear(); }

  size_type count(const key_type& __x) const { return _M_t.count_multi(__x); }

  bool contains(const key_type& __x) const { return find(__x) != end(); }

  iterator find(const key_type& __x) { return _M_t.find(__x); }

  const_iterator find(const key_type& __x) const { return _M_t.find(__x); }

  iterator lower_bound(const key_type& __x) { return _M_t.lower_bound(__x); }

  const_iterator lower_bound(const key_type& __x) const { return _M_t.lower_bound(__x); }

  iterator upper_bound(const key_type& __x) { return _M_t.upper_bound(__x); }
  
  const_iterator upper_bound(const key_type& __x) const { return _M_t.upper_bound(__x); }

  tinySTL::pair<iterator, iterator>
  equal_range(const key_type& __x) 
  { return _M_t.equal_range(__x); }

  tinySTL::pair<const_iterator, const_iterator>
  equal_range(const key_type& __x) const 
  { return _M_t.equal_range(__x); }

  void disp(std::ostream& os) { return _M_t.disp(os); }

  template <class _Compare2>
  void merge(map<_Key, _Val, _Compare2, _Alloc>& __source) 
  { return merge(tinySTL::move(__source)); }

  template <class _Compare2>
  void merge(map<_Key, _Val, _Compare2, _Alloc>&& __source) 
  { _M_t._M_merge_equal(tinySTL::move(__source._M_t)); }

  template <class _Compare2>
  void merge(multimap<_Key, _Val, _Compare2, _Alloc>& __source) 
  { return merge(tinySTL::move(__source)); }

  template <class _Compare2>
  void merge(multimap<_Key, _Val, _Compare2, _Alloc>&& __source) 
  { _M_t._M_merge_equal(tinySTL::move(__source._M_t)); }

  friend bool operator==(const multimap& __x, const multimap& __y) 
  {
    return __x.size() == __y.size()
    && tinySTL::equal(__x.begin(), __x.end(), __y.begin());
  }
  friend bool operator<(const multimap& __x, const multimap& __y) 
  {
    return lexicographical_compare(
      __x.begin(), __x.end(),
      __y.begin(), __y.end()
    );
  }

  friend std::ostream& operator<<(std::ostream& os, const multimap& s) 
  { return os << s._M_t; }

};

}