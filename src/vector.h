// tinySTL: vector.
#pragma once

#include <initializer_list>

#include "tiny_pair.h"
#include "tiny_alloc.h"
#include "tiny_errors.h"
#include "tiny_concepts.h"
#include "tiny_iterator.h"
#include "tiny_uninitialized.h"

namespace tinySTL
{

template <class _Tp, class _Alloc> 
class _Vector_base {

 protected:
  class _Vector_iterator {
  protected:
    friend class _Vector_base;
    template <class, class> friend class vector;

  public:
    using iterator_category = tinySTL::random_access_iterator_tag;
    using value_type = _Tp;
    using difference_type = ptrdiff_t;
    using pointer = _Tp*;
    using reference = _Tp&;

  protected:
    pointer _M_ptr;

    constexpr _Vector_iterator(pointer __x) : _M_ptr(__x) {}

  public:
    constexpr _Vector_iterator() : _M_ptr() {}
    constexpr reference operator*() const { return *_M_ptr; }
    constexpr pointer  operator->() const { return  _M_ptr; }
    constexpr _Vector_iterator& operator++() { ++_M_ptr; return *this; }
    constexpr _Vector_iterator& operator--() { --_M_ptr; return *this; }
    constexpr _Vector_iterator  operator++(int) { _Vector_iterator __tmp = *this; ++(*this); return __tmp; }
    constexpr _Vector_iterator  operator--(int) { _Vector_iterator __tmp = *this; --(*this); return __tmp; }
    constexpr _Vector_iterator& operator+=(difference_type __n) { _M_ptr += __n; return *this; }
    constexpr _Vector_iterator& operator-=(difference_type __n) { _M_ptr -= __n; return *this; }
    constexpr _Vector_iterator  operator+ (difference_type __n) const { _Vector_iterator __tmp = *this; __tmp += __n; return __tmp; }
    constexpr _Vector_iterator  operator- (difference_type __n) const { _Vector_iterator __tmp = *this; __tmp -= __n; return __tmp; }
    constexpr reference operator[](difference_type __n) const { return _M_ptr[__n]; }
    constexpr difference_type   operator-(const _Vector_iterator& __right) const { return difference_type(_M_ptr - __right._M_ptr); }
    friend constexpr _Vector_iterator operator+(difference_type __n, const _Vector_iterator& __x) { return __x + __n; }
    friend constexpr bool operator==(const _Vector_iterator& __x, const _Vector_iterator& __y) { return __x._M_ptr == __y._M_ptr; }
    friend constexpr bool operator!=(const _Vector_iterator& __x, const _Vector_iterator& __y) { return __x._M_ptr != __y._M_ptr; }
    friend constexpr bool operator< (const _Vector_iterator& __x, const _Vector_iterator& __y) { return __x._M_ptr <  __y._M_ptr; }
    friend constexpr bool operator> (const _Vector_iterator& __x, const _Vector_iterator& __y) { return __x._M_ptr >  __y._M_ptr; }
    friend constexpr bool operator<=(const _Vector_iterator& __x, const _Vector_iterator& __y) { return __x._M_ptr <= __y._M_ptr; }
    friend constexpr bool operator>=(const _Vector_iterator& __x, const _Vector_iterator& __y) { return __x._M_ptr >= __y._M_ptr; }
  };

 public:
  typedef _Alloc allocator_type;

  allocator_type get_allocator() const 
  { return allocator_type(_M_impl); }

 protected:
  _Vector_base(const _Alloc& __a) 
    : _M_impl(__a) {}
  
  _Vector_base(size_t __n, const _Alloc& __a)
    : _M_impl(__n, __a) {}

  _Vector_base(const _Vector_base& __x) = default;

  _Vector_base(_Vector_base&& __x) = default;

 protected:
  struct _Vector_impl : public _Alloc
  {
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    _Vector_impl(const _Alloc& __a)
      : _Alloc(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0) 
      { }

    _Vector_impl(size_t __n, const _Alloc& __a)
      : _Alloc(__a)
      {
        _M_start = this->allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + __n;
      }

    _Vector_impl(const _Vector_impl& __x)
      : _Vector_impl(__x._M_end_of_storage - __x._M_start, __x)
      { }

    _Vector_impl(_Vector_impl&& __x)
      : _Alloc(tinySTL::move(__x)) 
      {
        _M_start = __x._M_start;
        _M_finish = __x._M_finish;
        _M_end_of_storage = __x._M_end_of_storage;
        __x._M_start = __x._M_finish = __x._M_end_of_storage = 0;
      }

    ~_Vector_impl()
      {
        this->deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = _M_finish = _M_end_of_storage = 0;
      }

  };

  _Vector_impl _M_impl;

  _Tp* _M_allocate(size_t __n) { return _M_impl.allocate(__n); }

  void _M_deallocate(_Tp* __p, size_t __n) 
    { _M_impl.deallocate(__p, __n); }
};


template <class _Tp, class _Alloc = tinySTL::allocator<_Tp>>
class vector : protected _Vector_base<_Tp, _Alloc> {

 private:
  typedef _Vector_base<_Tp, _Alloc> _Base;

 public:
  typedef _Tp value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef _Base::_Vector_iterator iterator;
  typedef tinySTL::const_iterator<iterator> const_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef typename _Base::allocator_type allocator_type;
  typedef tinySTL::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef tinySTL::reverse_iterator<iterator> reverse_iterator;

 protected:
  using _Base::_M_allocate;
  using _Base::_M_deallocate;
  using _Base::_M_impl;

 public:
  explicit vector(const allocator_type& __a = allocator_type())
    : _Base(__a) 
    { }

  vector(size_type __n, 
         const allocator_type& __a = allocator_type())
    : vector(__n, _Tp(), __a)
    { }

  vector(size_type __n, const _Tp& __value,
         const allocator_type& __a = allocator_type()) 
    : _Base(__n, __a)
    { 
      _M_impl._M_finish = tinySTL::
        uninitialized_fill_n(_M_impl._M_start, __n, __value); 
    }

  vector(const vector& __x)
    : _Base(__x)
    { 
      _M_impl._M_finish = tinySTL::uninitialized_copy(
        __x.begin(), __x.end(), _M_impl._M_start);
    }

  vector(vector&& __x) = default;

  vector(std::initializer_list<_Tp> __l, 
         const allocator_type& __a = allocator_type())
    : _Base(__l.size(), __a)
    { 
      _M_impl._M_finish = tinySTL::uninitialized_copy(
        __l.begin(), __l.end(), _M_impl._M_start); 
    }

  template <InputIterator Iterator>
  vector(Iterator __first, Iterator __last,
         const allocator_type& __a = allocator_type())
    : _Base(tinySTL::distance(__first, __last), __a)
    { 
      _M_impl._M_finish = tinySTL::uninitialized_copy(
        __first, __last, _M_impl._M_start); 
    }

  ~vector() 
    { 
      tinySTL::destroy(
        _M_impl._M_start, _M_impl._M_finish); 
    }

  vector& operator=(std::initializer_list<_Tp> __l) 
  {
    assign(__l);
    return *this;
  }

  vector& operator=(const vector& __x)
  {
    if (&__x != this) {
      assign(__x.begin(), __x.end());
    }
    return *this;
  }

  vector& operator=(vector&& __x)
  {
    if (&__x != this) {
      tinySTL::destroy(_M_impl._M_start, _M_impl._M_finish);
      _M_deallocate(_M_impl._M_start, 
      _M_impl._M_end_of_storage - _M_impl._M_start);
      _M_impl._M_start = __x._M_impl._M_start;
      _M_impl._M_finish = __x._M_impl._M_finish;
      _M_impl._M_end_of_storage = __x._M_impl._M_end_of_storage;
      __x._M_impl._M_start = 0;
      __x._M_impl._M_finish = 0;
      __x._M_impl._M_end_of_storage = 0;
    }
    return *this;
  }

  void assign(std::initializer_list<_Tp> __l)
    { assign(__l.begin(), __l.end()); }

  void assign(size_type __n, const _Tp& __val) {
    if (__n > capacity()) {
      // new and swap.
      vector __tmp(__n, __val, get_allocator());
      __tmp.swap(*this);
    } else if (__n > size()) {
      tinySTL::fill(begin(), end(), __val);
      _M_impl._M_finish = tinySTL::uninitialized_fill_n(
        _M_impl._M_finish, __n - size(), __val);
    } else {
      erase(tinySTL::fill_n(begin(), __n, __val), end());
    }
  }

  template <InputIterator Iterator>
  void assign(Iterator __first, Iterator __last)
    { _M_assign_aux(__first, __last, iterator_category(__first)); }

 public:
  allocator_type get_allocator() const { return _Base::get_allocator(); }
  
  iterator begin() { return _M_impl._M_start; }
  
  const_iterator begin() const { return iterator(_M_impl._M_start); }

  const_iterator cbegin() const { return _M_impl._M_start; }
  
  iterator end() { return _M_impl._M_finish; }
  
  const_iterator end() const { return iterator(_M_impl._M_finish); }

  const_iterator cend() const { return _M_impl._M_finish; }

  reverse_iterator rbegin() 
    { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const
    { return const_reverse_iterator(end()); }

  const_reverse_iterator crbegin() const
    { return const_reverse_iterator(end()); }

  reverse_iterator rend()
    { return reverse_iterator(begin()); }
  
  const_reverse_iterator rend() const
    { return const_reverse_iterator(begin()); }

  const_reverse_iterator crend() const
    { return const_reverse_iterator(begin()); }

  size_type size() const
    { return size_type(end() - begin()); }

  size_type max_size() const
    { return size_type(-1) / sizeof(_Tp); }

  size_type capacity() const
    { return size_type(_M_impl._M_end_of_storage - _M_impl._M_start); }

  bool empty() const
    { return begin() == end(); }

  reference operator[](size_type __n) { return *(begin() + __n); }

  const_reference operator[](size_type __n) const { return *(begin() + __n); }

  reference at(size_type __n)
    { _M_range_check(__n); return (*this)[__n]; }
  
  const_reference at(size_type __n) const
    { _M_range_check(__n); return (*this)[__n]; }

  reference front() { 
    _M_range_check(0);
    return *begin(); 
  }

  const_reference front() const { 
    _M_range_check(0);
    return *begin(); 
  }

  reference back() { 
    _M_range_check(0);
    return *(end() - 1);
  }

  const_reference back() const { 
    _M_range_check(0);
    return *(end() - 1); 
  }

  void push_back(const _Tp& __x)
    { emplace(end(), __x); }

  void push_back(_Tp&& __x) 
    { emplace(end(), tinySTL::move(__x)); }

  void pop_back() {
    if (_M_impl._M_start != _M_impl._M_finish) {
      --_M_impl._M_finish;
      tinySTL::destroy(_M_impl._M_finish);
    }
  }

  iterator insert(const_iterator __pos, _Tp&& __x)
    { return emplace(__pos, tinySTL::move(__x)); }

  iterator insert(const_iterator __pos, const _Tp& __x)
    { return emplace(__pos, __x); }

  iterator insert(const_iterator __pos, size_type __n, const _Tp& __x)
    { return _M_insert_aux(__pos.base(), __n, __x); }
  

  iterator insert(const_iterator __pos, std::initializer_list<_Tp> __l)
    { return insert(__pos, __l.begin(), __l.end()); }

  template <InputIterator Iterator>
  iterator insert(const_iterator __position, Iterator __first, Iterator __last)
  {
    pointer __pos = __position.base()._M_ptr;
    if (__first == __last) return __pos;
      
    size_type __n = tinySTL::distance(__first, __last);
    const size_type __elems_before = __pos - _M_impl._M_start;
    if (size_type(_M_impl._M_end_of_storage - _M_impl._M_finish) >= __n) {
      const size_type __elems_after = _M_impl._M_finish - __pos;
      pointer __old_finish = _M_impl._M_finish;
      if (__elems_after > __n) {
        tinySTL::uninitialized_move(
          _M_impl._M_finish - __n, _M_impl._M_finish, _M_impl._M_finish);
        _M_impl._M_finish += __n;
        tinySTL::move_backward(__pos, __old_finish - __n, __old_finish);
        tinySTL::copy(__first, __last, __pos);
      } else {
        _M_impl._M_finish += __n - __elems_after;
        tinySTL::uninitialized_move(__pos, __old_finish, _M_impl._M_finish);
        pair __copy_n_result = tinySTL::copy_n(__first, __elems_after, __pos);
        tinySTL::uninitialized_copy(__copy_n_result.first, __last, __old_finish);
        _M_impl._M_finish += __elems_after;
      }
    } else {
      const size_type __old_size = size();
      const size_type __len = __old_size + max(__old_size, __n);
      pointer __new_start = _M_allocate(__len);
      pointer __new_finish = __new_start;
      try {
        __new_finish = tinySTL::uninitialized_move(_M_impl._M_start, __pos, __new_start);
        __new_finish = tinySTL::uninitialized_copy(__first, __last, __new_finish);
        __new_finish = tinySTL::uninitialized_move(__pos, _M_impl._M_finish, __new_finish);
      } catch (...) {
        tinySTL::destroy(__new_start, __new_finish);
        _M_deallocate(__new_start, __len);
        throw;
      }
      tinySTL::destroy(_M_impl._M_start, _M_impl._M_finish);
      _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
      _M_impl._M_start = __new_start;
      _M_impl._M_finish = __new_finish;
      _M_impl._M_end_of_storage = __new_start + __len;
    }
    return _M_impl._M_start + __elems_before;
  }

  void swap(vector& __x) {
    tinySTL::swap(_M_impl._M_start, __x._M_impl._M_start);
    tinySTL::swap(_M_impl._M_finish, __x._M_impl._M_finish);
    tinySTL::swap(_M_impl._M_end_of_storage, __x._M_impl._M_end_of_storage);
  }

  iterator erase(const_iterator __position) {
    pointer __pos = __position.base()._M_ptr;
    _M_range_check(size_type(__pos - _M_impl._M_start));
    if (__pos + 1 != end())
      tinySTL::move(__pos + 1, _M_impl._M_finish, __pos);
    --_M_impl._M_finish;
    tinySTL::destroy(_M_impl._M_finish);
    return __pos;
  }

  iterator erase(const_iterator first, const_iterator last) {
    pointer __first = first.base()._M_ptr;
    pointer __last = last.base()._M_ptr;
    
    if (__first - _M_impl._M_start > this->size())
      __tiny_throw_range_error("vector");
    if ( __last - _M_impl._M_start > this->size())
      __tiny_throw_range_error("vector");

    if ((__first - _M_impl._M_start) < 0 || (__last - _M_impl._M_start) < 0)
      __tiny_throw_range_error("vector");
    
    if (__last - __first > 0) {
      auto __i = tinySTL::move(__last, _M_impl._M_finish, __first);
      tinySTL::destroy(__i, _M_impl._M_finish);
      _M_impl._M_finish = _M_impl._M_finish - (__last - __first);
    }
    return __first;
  }

  void resize(size_type __new_size, const _Tp& __x) {
    if (__new_size < size()) 
      erase(begin() + __new_size, end());
    else
      _M_insert_aux(end(), __new_size - size(), __x, true);
  }

  void resize(size_type __new_size) { resize(__new_size, _Tp()); }

  void clear() { erase(begin(), end()); }

  void reserve(size_type __n) {
    if (capacity() < __n) {
      const size_type __old_size = size();
      pointer __tmp = _M_allocate_and_move(__n, _M_impl._M_start, _M_impl._M_finish);
      tinySTL::destroy(_M_impl._M_start, _M_impl._M_finish);
      _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
      _M_impl._M_start = __tmp;
      _M_impl._M_finish = __tmp + __old_size;
      _M_impl._M_end_of_storage = _M_impl._M_start + __n;
    }
  }

  void shrink_to_fit() {
    if (_M_impl._M_finish == _M_impl._M_end_of_storage) return;
    if (_M_impl._M_start == _M_impl._M_finish) {
      _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
      _M_impl._M_start = _M_impl._M_finish = _M_impl._M_end_of_storage = 0;
    } else {
      const size_type __size = size();
      pointer __tmp = _M_allocate_and_move(__size, _M_impl._M_start, _M_impl._M_finish);
      tinySTL::destroy(_M_impl._M_start, _M_impl._M_finish);
      _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
      _M_impl._M_start = __tmp;
      _M_impl._M_finish = __tmp + __size;
      _M_impl._M_end_of_storage = _M_impl._M_finish;
    }
  }

  const _Tp* data() const { return _M_impl._M_start; }

  _Tp* data() { return _M_impl._M_start; }

  template <typename... _Args>
  iterator emplace(const_iterator __pos, _Args&& ...__args)
  {
    size_type __n = __pos - begin();
    if (_M_impl._M_finish != _M_impl._M_end_of_storage && __pos == end()) {
      tinySTL::construct(_M_impl._M_finish, forward<_Args>(__args)...);
      ++_M_impl._M_finish;
    }
    else
      _M_emplace_aux(__pos, forward<_Args>(__args)...);
    return begin() + __n;
  }

  template <typename... _Args>
  reference emplace_back(_Args&& ...__args)
    { return *emplace(end(), forward<_Args>(__args)...); }

 protected:
  void _M_range_check(size_type __n) const {
    if (__n >= this->size())
      __tiny_throw_range_error("vector");
  }

  template <class _ForwardIterator>
  pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, 
                                               _ForwardIterator __last)
  {
    pointer __result = _M_allocate(__n);
    try {
      tinySTL::uninitialized_copy(__first, __last, __result);
      return __result;
    }
    catch (...) { 
      _M_deallocate(__result, __n);
      throw;
    }
  }

  template <class _ForwardIterator>
  pointer _M_allocate_and_move(size_type __n, _ForwardIterator __first, 
                                               _ForwardIterator __last)
  {
    pointer __result = _M_allocate(__n);
    try {
      tinySTL::uninitialized_move(__first, __last, __result);
      return __result;
    }
    catch (...) { 
      _M_deallocate(__result, __n);
      throw;
    }
  }

  template <class _InputIterator>
  void _M_assign_aux(_InputIterator __first, _InputIterator __last,
                     input_iterator_tag) 
  {
    iterator __cur = begin();
    for ( ; __first != __last && __cur != end(); ++__cur, ++__first)
      *__cur = *__first;
    if (__first == __last)
      erase(__cur, end());
    else
      insert(end(), __first, __last);
  }

  template <class _ForwardIterator>
  void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
                     forward_iterator_tag)
  {
    size_type __len = tinySTL::distance(__first, __last);

    if (__len > capacity()) {
      auto __tmp = _M_allocate_and_copy(__len, __first, __last);
      tinySTL::destroy(_M_impl._M_start, _M_impl._M_finish);
      _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
      _M_impl._M_start = __tmp;
      _M_impl._M_end_of_storage = _M_impl._M_finish = _M_impl._M_start + __len;
    }
    else if (size() >= __len) {
      auto __new_finish = tinySTL::copy(__first, __last, _M_impl._M_start);
      tinySTL::destroy(__new_finish, _M_impl._M_finish);
      _M_impl._M_finish = __new_finish;
    }
    else {
      _ForwardIterator __mid = __first;
      tinySTL::advance(__mid, size());
      tinySTL::copy(__first, __mid, _M_impl._M_start);
      _M_impl._M_finish = tinySTL::uninitialized_copy(__mid, __last, _M_impl._M_finish);
    }
  }

  // _M_finish == _M_end_of_storage || _M_finish != __position
  template <typename... _Args>
  void _M_emplace_aux(const_iterator __pos, _Args&& ...__args)
  {
    pointer __position = __pos.base()._M_ptr;
    if (_M_impl._M_finish != _M_impl._M_end_of_storage) {
      tinySTL::construct(_M_impl._M_finish, *(_M_impl._M_finish - 1));
      ++_M_impl._M_finish;
      tinySTL::move_backward(__position, _M_impl._M_finish - 2, _M_impl._M_finish - 1);
      *__position = _Tp(tinySTL::forward<_Args>(__args)...);
    } else {
      const size_type __old_size = size();
      const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
      pointer __new_start = _M_allocate(__len);
      pointer __new_finish = __new_start;
      try {
        __new_finish = tinySTL::uninitialized_move(_M_impl._M_start, __position, __new_start);
        tinySTL::construct(__new_finish, tinySTL::forward<_Args>(__args)...);
        ++__new_finish;
        __new_finish = tinySTL::uninitialized_move(__position, _M_impl._M_finish, __new_finish);
      } catch (...) {
        tinySTL::destroy(__new_start, __new_finish);
        _M_deallocate(__new_start, __len);
        throw;
      }
      tinySTL::destroy(begin(), end());
      _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
      _M_impl._M_start = __new_start;
      _M_impl._M_finish = __new_finish;
      _M_impl._M_end_of_storage = __new_start + __len;
    }
  }

  iterator _M_insert_aux(iterator __position, size_type __n, 
                         const _Tp& __x, bool extent_fix = false) 
  {
    if (__n == 0) return __position;
    pointer __pos = __position._M_ptr;

    const size_type __elems_before = __pos - _M_impl._M_start;
    if (size_type(_M_impl._M_end_of_storage - _M_impl._M_finish) >= __n) {
      _Tp __x_copy = __x;
      const size_type __elems_after = _M_impl._M_finish - __pos;
      pointer __old_finish = _M_impl._M_finish;
      if (__elems_after > __n) {
        tinySTL::uninitialized_move(
          _M_impl._M_finish - __n, _M_impl._M_finish, _M_impl._M_finish);
        _M_impl._M_finish += __n;
        tinySTL::move_backward(__pos, __old_finish - __n, __old_finish);
        tinySTL::fill(__pos, __pos + __n, __x_copy);
      } else {
        tinySTL::uninitialized_fill_n(_M_impl._M_finish, __n - __elems_after, __x_copy);
        _M_impl._M_finish += __n - __elems_after;
        tinySTL::uninitialized_move(__pos, __old_finish, _M_impl._M_finish);
        _M_impl._M_finish += __elems_after;
        tinySTL::fill(__pos, __old_finish, __x_copy);
      }
    } else {
      const size_type __old_size = size();        
      const size_type __len = __old_size + (extent_fix ? __n : max(__old_size, __n));
      pointer __new_start = _M_allocate(__len);
      pointer __new_finish = __new_start;
      try {
        __new_finish = tinySTL::uninitialized_move(_M_impl._M_start, __pos, __new_start);
        __new_finish = tinySTL::uninitialized_fill_n(__new_finish, __n, __x);
        __new_finish = tinySTL::uninitialized_move(__pos, _M_impl._M_finish, __new_finish);
      } catch (...) {
        tinySTL::destroy(__new_start, __new_finish);
        _M_deallocate(__new_start, __len);
        throw;
      }
      tinySTL::destroy(_M_impl._M_start, _M_impl._M_finish);
      _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
      _M_impl._M_start = __new_start;
      _M_impl._M_finish = __new_finish;
      _M_impl._M_end_of_storage = __new_start + __len;
    }
    return _M_impl._M_start + __elems_before;
  }

};

template <class _Tp, class _Alloc> inline void 
swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y)
  { __x.swap(__y); }

template <class _Tp, class _Alloc> inline bool 
operator==(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
  { return __x.size() == __y.size() 
    && equal(__x.begin(), __x.end(), __y.begin());
  }

template <class _Tp, class _Alloc> inline bool
operator!=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) 
  { return !(__x == __y); }

template <class _Tp, class _Alloc> inline bool
operator<(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
  { return lexicographical_compare(
            __x.begin(), __x.end(), 
            __y.begin(), __y.end());
  }

template <class _Tp, class _Alloc> inline bool
operator>(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) 
  { return __y < __x; }

template <class _Tp, class _Alloc> inline bool
operator<=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) 
  { return !(__y < __x); }

template <class _Tp, class _Alloc> inline bool
operator>=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) 
  { return !(__x < __y); }

}