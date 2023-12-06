#pragma once
#include "tiny_iterator_base.h"


namespace tinySTL
{

template <class _Iterator, class _IteratorTag>
class __const_iterator 
{
protected:
  _Iterator current;
public:
  typedef typename iterator_traits<_Iterator>::iterator_category
          iterator_category;
  typedef const typename iterator_traits<_Iterator>::value_type
          value_type;
  typedef typename iterator_traits<_Iterator>::difference_type
          difference_type;
  typedef value_type* pointer;
  typedef value_type& reference;

  typedef _Iterator iterator_type;
  typedef __const_iterator<_Iterator, _IteratorTag> _Self;

public:
  __const_iterator() {}
  __const_iterator(const iterator_type& __x) : current(__x) {}

  __const_iterator(const _Self& __x) : current(__x.current) {}
    
  iterator_type base() const { return current; }

  friend bool
  operator==(const _Self& __x, const _Self& __y)
    { return __x.current == __y.current; }
  
  friend bool
  operator!=(const _Self& __x, const _Self& __y) 
    { return !(__x == __y); }

  friend bool
  operator<(const _Self& __x, const _Self& __y) 
    { return __x.current < __y.current; }

  friend bool
  operator>(const _Self& __x, const _Self& __y) 
    { return __y < __x; }

  friend bool
  operator<=(const _Self& __x, const _Self& __y) 
    { return !(__y < __x); }

  friend bool
  operator>=(const _Self& __x, const _Self& __y) 
    { return !(__x < __y); }
};

template <class _Iterator, class _IteratorTag>
class __reverse_iterator 
{
protected:
  _Iterator current;
public:
  typedef typename iterator_traits<_Iterator>::iterator_category
          iterator_category;
  typedef typename iterator_traits<_Iterator>::value_type
          value_type;
  typedef typename iterator_traits<_Iterator>::difference_type
          difference_type;
  typedef typename iterator_traits<_Iterator>::pointer
          pointer;
  typedef typename iterator_traits<_Iterator>::reference
          reference;

  typedef _Iterator iterator_type;
  typedef __reverse_iterator<_Iterator, _IteratorTag> _Self;

public:
  __reverse_iterator() {}
  explicit __reverse_iterator(iterator_type __x) : current(__x) {}

  __reverse_iterator(const _Self& __x) : current(__x.current) {}
    
  iterator_type base() const { return current; }

  friend bool
  operator==(const _Self& __x, const _Self& __y)
    { return __x.current == __y.current; }
  
  friend bool
  operator!=(const _Self& __x, const _Self& __y) 
    { return !(__x == __y); }

  friend bool
  operator<(const _Self& __x, const _Self& __y) 
    { return __y.current < __x.current; }

  friend bool
  operator>(const _Self& __x, const _Self& __y) 
    { return __y < __x; }

  friend bool
  operator<=(const _Self& __x, const _Self& __y) 
    { return !(__y < __x); }

  friend bool
  operator>=(const _Self& __x, const _Self& __y) 
    { return !(__x < __y); }
};


// bidirectional version.
template <class _Iterator>
class __const_iterator<_Iterator, bidirectional_iterator_tag>
{
protected:
  _Iterator current;
public:
  typedef typename iterator_traits<_Iterator>::iterator_category
          iterator_category;
  typedef const typename iterator_traits<_Iterator>::value_type
          value_type;
  typedef typename iterator_traits<_Iterator>::difference_type
          difference_type;
  typedef value_type* pointer;
  typedef value_type& reference;

  typedef _Iterator iterator_type;
  typedef __const_iterator<_Iterator, bidirectional_iterator_tag> _Self;

public:
  __const_iterator() {}
  
  __const_iterator(const iterator_type& __x) : current(__x) {}

  __const_iterator(const _Self& __x) : current(__x.current) {}
    
  iterator_type base() const { return current; }
  
  reference operator*() const { return *current; }

  pointer operator->() const { return &(operator*()); }

  _Self& operator++() {
    ++current;
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    ++current;
    return __tmp;
  }
  _Self& operator--() {
    --current;
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    --current;
    return __tmp;
  }

  friend bool
  operator==(const _Self& __x, const _Self& __y)
    { return __x.current == __y.current; }
  
  friend bool
  operator!=(const _Self& __x, const _Self& __y) 
    { return !(__x == __y); }

  friend bool
  operator<(const _Self& __x, const _Self& __y) 
    { return __x.current < __y.current; }

  friend bool
  operator>(const _Self& __x, const _Self& __y) 
    { return __y < __x; }

  friend bool
  operator<=(const _Self& __x, const _Self& __y) 
    { return !(__y < __x); }

  friend bool
  operator>=(const _Self& __x, const _Self& __y) 
    { return !(__x < __y); }
};

// bidirectional version.
template <class _Iterator>
class __reverse_iterator<_Iterator, bidirectional_iterator_tag>
{
protected:
  _Iterator current;
public:
  typedef typename iterator_traits<_Iterator>::iterator_category
          iterator_category;
  typedef typename iterator_traits<_Iterator>::value_type
          value_type;
  typedef typename iterator_traits<_Iterator>::difference_type
          difference_type;
  typedef typename iterator_traits<_Iterator>::pointer
          pointer;
  typedef typename iterator_traits<_Iterator>::reference
          reference;

  typedef _Iterator iterator_type;
  typedef __reverse_iterator<_Iterator, bidirectional_iterator_tag> _Self;

public:
  __reverse_iterator() {}
  explicit __reverse_iterator(iterator_type __x) : current(__x) {}

  __reverse_iterator(const _Self& __x) : current(__x.current) {}
    
  iterator_type base() const { return current; }
  reference operator*() const {
    _Iterator __tmp = current;
    return *--__tmp;
  }

  pointer operator->() const { return &(operator*()); }

  _Self& operator++() {
    --current;
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    --current;
    return __tmp;
  }
  _Self& operator--() {
    ++current;
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    ++current;
    return __tmp;
  }

  friend bool
  operator==(const _Self& __x, const _Self& __y)
    { return __x.current == __y.current; }
  
  friend bool
  operator!=(const _Self& __x, const _Self& __y) 
    { return !(__x == __y); }

  friend bool
  operator<(const _Self& __x, const _Self& __y) 
    { return __y.current < __x.current; }

  friend bool
  operator>(const _Self& __x, const _Self& __y) 
    { return __y < __x; }

  friend bool
  operator<=(const _Self& __x, const _Self& __y) 
    { return !(__y < __x); }

  friend bool
  operator>=(const _Self& __x, const _Self& __y) 
    { return !(__x < __y); }
};

template <class _Iterator>
class __const_iterator<_Iterator, random_access_iterator_tag>
{
protected:
  _Iterator current;
public:
  typedef typename iterator_traits<_Iterator>::iterator_category
          iterator_category;
  typedef const typename iterator_traits<_Iterator>::value_type
          value_type;
  typedef typename iterator_traits<_Iterator>::difference_type
          difference_type;
  typedef value_type* pointer;
  typedef value_type& reference;

  typedef _Iterator iterator_type;
  typedef __const_iterator<_Iterator, random_access_iterator_tag> _Self;

public:
  __const_iterator() {}
  
  __const_iterator(const iterator_type& __x) : current(__x) {}

  __const_iterator(const _Self& __x) : current(__x.current) {}
    
  iterator_type base() const { return current; }
  
  reference operator*() const { return *current; }

  pointer operator->() const { return &(operator*()); }

  _Self& operator++() {
    ++current;
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    ++current;
    return __tmp;
  }
  _Self& operator--() {
    --current;
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    --current;
    return __tmp;
  }
  _Self operator+(difference_type __n) const {
    return _Self(current - __n);
  }
  _Self& operator+=(difference_type __n) {
    current -= __n;
    return *this;
  }
  _Self operator-(difference_type __n) const {
    return _Self(current + __n);
  }
  _Self& operator-=(difference_type __n) {
    current += __n;
    return *this;
  }
  reference operator[](difference_type __n) const { return *(*this + __n); }

  friend bool
  operator==(const _Self& __x, const _Self& __y)
    { return __x.current == __y.current; }
  
  friend bool
  operator!=(const _Self& __x, const _Self& __y) 
    { return !(__x == __y); }

  friend bool
  operator<(const _Self& __x, const _Self& __y) 
    { return __x.current < __y.current; }

  friend bool
  operator>(const _Self& __x, const _Self& __y) 
    { return __y < __x; }

  friend bool
  operator<=(const _Self& __x, const _Self& __y) 
    { return !(__y < __x); }

  friend bool
  operator>=(const _Self& __x, const _Self& __y) 
    { return !(__x < __y); }

  friend difference_type
  operator-(const _Self& __x, const _Self& __y) 
    { return __x.current - __y.current; }

  friend _Self
  operator+(difference_type __n, const _Self& __x) 
    { return const_iterator(__x.current + __n); }
};

// random_access version.
template <class _Iterator>
class __reverse_iterator<_Iterator, random_access_iterator_tag>
{
protected:
  _Iterator current;
public:
  typedef typename iterator_traits<_Iterator>::iterator_category
          iterator_category;
  typedef typename iterator_traits<_Iterator>::value_type
          value_type;
  typedef typename iterator_traits<_Iterator>::difference_type
          difference_type;
  typedef typename iterator_traits<_Iterator>::pointer
          pointer;
  typedef typename iterator_traits<_Iterator>::reference
          reference;

  typedef _Iterator iterator_type;
  typedef __reverse_iterator<_Iterator, random_access_iterator_tag> _Self;

public:
  __reverse_iterator() {}
  explicit __reverse_iterator(iterator_type __x) : current(__x) {}

  __reverse_iterator(const _Self& __x) : current(__x.current) {}
    
  iterator_type base() const { return current; }
  reference operator*() const {
    _Iterator __tmp = current;
    return *--__tmp;
  }

  pointer operator->() const { return &(operator*()); }

  _Self& operator++() {
    --current;
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    --current;
    return __tmp;
  }
  _Self& operator--() {
    ++current;
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    ++current;
    return __tmp;
  }

  _Self operator+(difference_type __n) const {
    return _Self(current - __n);
  }
  _Self& operator+=(difference_type __n) {
    current -= __n;
    return *this;
  }
  _Self operator-(difference_type __n) const {
    return _Self(current + __n);
  }
  _Self& operator-=(difference_type __n) {
    current += __n;
    return *this;
  }
  reference operator[](difference_type __n) const { return *(*this + __n); }

  friend bool
  operator==(const _Self& __x, const _Self& __y)
    { return __x.current == __y.current; }
  
  friend bool
  operator!=(const _Self& __x, const _Self& __y) 
    { return !(__x == __y); }

  friend bool
  operator<(const _Self& __x, const _Self& __y) 
    { return __y.current < __x.current; }

  friend bool
  operator>(const _Self& __x, const _Self& __y) 
    { return __y < __x; }

  friend bool
  operator<=(const _Self& __x, const _Self& __y) 
    { return !(__y < __x); }

  friend bool
  operator>=(const _Self& __x, const _Self& __y) 
    { return !(__x < __y); }

  friend difference_type
  operator-(const _Self& __x, const _Self& __y) 
    { return __x.current - __y.current; }

  friend _Self
  operator+(difference_type __n, const _Self& __x) 
    { return const_iterator(__x.current + __n); }
};


template <class _Iterator>
using const_iterator = __const_iterator<
  _Iterator, typename iterator_traits<_Iterator>::iterator_category>;

template <class _Iterator>
using reverse_iterator = __reverse_iterator<
  _Iterator, typename iterator_traits<_Iterator>::iterator_category>;

}