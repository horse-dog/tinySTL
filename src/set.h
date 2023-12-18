#pragma once

#include "tiny_pair.h"
#include "tiny_alloc.h"
#include "tiny_errors.h"
#include "tiny_concepts.h"
#include "tiny_iterator.h"
#include "tiny_function.h"
#include "tiny_uninitialized.h"

#include <bits/stl_tree.h>

namespace tinySTL
{

template<class _Key, class _Cp = void , class _Alloc = alloc>
class set {
 
 protected:
  template <class _Tx, class _Ty>
  struct _Cp_traits { using type = _Ty; };

  template <class _Tx>
  struct _Cp_traits<_Tx, void> { 
    struct _Less { 
      bool operator() 
      (const _Key& a, const _Key& b) const
      { return a < b; }
    }; 
    using type = _Less;
  };

  using _Compare = typename _Cp_traits<_Key, _Cp>::type;

 public:
  typedef _Key     key_type;
  typedef _Key     value_type;
  typedef _Compare key_compare;
  typedef _Compare value_compare;
  typedef _Alloc   allocator_type;

 protected:
  typedef std::_Rb_tree<key_type, value_type, _Identity<value_type>, key_compare>
          _Rep_type; // TODO: std -> tiny.
  _Rep_type _M_t;
 
 public:
  typedef typename _Rep_type::const_pointer pointer;
  typedef typename _Rep_type::const_pointer const_pointer;
  typedef typename _Rep_type::const_reference reference;
  typedef typename _Rep_type::const_reference const_reference;
  typedef typename _Rep_type::const_iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::const_reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;

 public:
  set();

  set(const set&);

  set(set&&);

  set(std::initializer_list<value_type> __l);

  template <InputIterator Iterator>
  set(Iterator __first, Iterator __last);

  ~set();

  set& operator=(const set&);

  set& operator=(set&&);

  set& operator=(std::initializer_list<value_type> __l);

  key_compare key_comp() const;

  value_compare value_comp() const;

 public:
  allocator_type get_allocator() const;

  iterator begin() const;
 
  iterator cbegin() const;

  iterator end() const;

  iterator cend() const;

  reverse_iterator rbegin() const;

  reverse_iterator crbegin() const;

  reverse_iterator rend() const;

  reverse_iterator crend() const;

  bool empty() const;

  size_type size() const;

  size_type max_size() const;

  void swap(set& __x);

  template<typename... _Args> std::pair<iterator, bool> // TODO: std -> tiny.
  emplace(_Args&&... __args);

  template<typename... _Args> iterator
  emplace_hint(const_iterator __pos, _Args&&... __args);

  std::pair<iterator, bool> // TODO: std -> tiny.
  insert(const value_type& __x);

  std::pair<iterator, bool> // TODO: std -> tiny.
  insert(value_type&& __x);

  iterator
  insert(const_iterator __position, const value_type& __x);

  iterator
  insert(const_iterator __position, value_type&& __x);

  template<typename _InputIterator> void
  insert(_InputIterator __first, _InputIterator __last);

  void insert(std::initializer_list<value_type> __l);

  size_type erase(const key_type& __x);

  iterator erase(const_iterator __position);

  iterator erase(const_iterator __first, const_iterator __last);

  void clear();

  size_type count(const key_type& __x) const;

  bool contains(const key_type& __x) const;

  iterator find(const key_type& __x);

  const_iterator find(const key_type& __x) const;

  iterator lower_bound(const key_type& __x);

  const_iterator lower_bound(const key_type& __x) const;

  iterator upper_bound(const key_type& __x);
  
  const_iterator upper_bound(const key_type& __x) const;

  std::pair<iterator, iterator> // TODO: std -> tiny.
  equal_range(const key_type& __x);

  std::pair<const_iterator, const_iterator>  // TODO: std -> tiny.
  equal_range(const key_type& __x) const;

  // TODO: opeartor==
  // TODO: opeartor <

};

class multiset;

}