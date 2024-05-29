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
  typedef tinySTL::_Rb_tree<key_type, value_type, _Identity<value_type>, key_compare>
            _Rep_type;
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
  set() : _M_t() {}

  set(const set&) = default;

  set(set&&) = default;

  set(std::initializer_list<value_type> __l) : _M_t() 
  { insert(__l); }

  template <InputIterator Iterator>
  set(Iterator __first, Iterator __last) : _M_t() 
  { insert(__first, __last); }

  ~set() {}

  set& operator=(const set&) = default;

  set& operator=(set&&) = default;

  set& operator=(std::initializer_list<value_type> __l);

  key_compare key_comp() const { return _M_t.key_comp(); }

  value_compare value_comp() const { return _M_t.key_comp(); }

 public:
  allocator_type get_allocator() const { return _M_t._M_node_allocator; }

  iterator begin() { return _M_t.begin(); }

  iterator begin() const { return _M_t.begin(); }
 
  iterator cbegin() const { return _M_t.begin(); }

  iterator end() { return _M_t.end(); }

  iterator end() const { return _M_t.end(); }

  iterator cend() const { return _M_t.end(); }

  reverse_iterator rbegin() { return _M_t.rbegin(); }

  reverse_iterator rbegin() const { return _M_t.rbegin(); }

  reverse_iterator crbegin() const { return _M_t.rbegin(); }

  reverse_iterator rend() { return _M_t.rend(); }

  reverse_iterator rend() const { return _M_t.rend(); }

  reverse_iterator crend() const { return _M_t.rend(); }

  bool empty() const { return _M_t.size() == 0; }

  size_type size() const { return _M_t.size(); }

  size_type max_size() const { return _M_t.max_size(); }

  void swap(set& __x) { tinySTL::swap(*this, __x); }

  template<typename... _Args> tinySTL::pair<iterator, bool>
  emplace(_Args&&... __args) { return _M_t._M_emplace_unique(tinySTL::forward<_Args>(__args)...); }

  tinySTL::pair<iterator, bool>
  insert(const value_type& __x) { return _M_t._M_insert_unique(__x); }

  tinySTL::pair<iterator, bool>
  insert(value_type&& __x) { return _M_t._M_insert_unique(tinySTL::move(__x)); }

  template<typename _InputIterator> void
  insert(_InputIterator __first, _InputIterator __last) 
  {
    for (; __first != __last; ++__first) {
      insert(*__first);
    }
  }

  void insert(std::initializer_list<value_type> __l) {
    insert(__l.begin(), __l.end());
  }

  size_type erase(const key_type& __x); // TODO.

  iterator erase(const_iterator __position); // TODO.

  iterator erase(const_iterator __first, const_iterator __last); // TODO.

  void clear() { _M_t.clear(); }

  size_type count(const key_type& __x) const { return _M_t.count_unique(__x); }

  bool contains(const key_type& __x) const { return find(__x) != end(); }

  iterator find(const key_type& __x) { return _M_t.find(__x); }

  const_iterator find(const key_type& __x) const { return _M_t.find(__x); }

  iterator lower_bound(const key_type& __x) { return _M_t.lower_bound(__x); }

  const_iterator lower_bound(const key_type& __x) const { return _M_t.lower_bound(__x); }

  iterator upper_bound(const key_type& __x) { return _M_t.upper_bound(__x); }
  
  const_iterator upper_bound(const key_type& __x) const { return _M_t.upper_bound(__x); }

  tinySTL::pair<iterator, iterator> // TODO.
  equal_range(const key_type& __x);

  tinySTL::pair<const_iterator, const_iterator>  // TODO.
  equal_range(const key_type& __x) const;

  void merge(set& __source);
  void merge(set&& __source);
  // void merge(multiset& __source);  // TODO.
  // void merge(multiset&& __source); // TODO.

  // TODO: opeartor==
  // TODO: opeartor <

};

class multiset;

}