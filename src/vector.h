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
 public:
  typedef _Alloc allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }

 protected:
  _Vector_base(const _Alloc&)
    : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
  
  _Vector_base(size_t __n, const _Alloc&)
    : _M_start(0), _M_finish(0), _M_end_of_storage(0) 
  {
    _M_start = _M_allocate(__n);
    _M_finish = _M_start;
    _M_end_of_storage = _M_start + __n;
  }

  ~_Vector_base() 
  { 
    _M_deallocate(_M_start, _M_end_of_storage - _M_start);
    _M_start = _M_finish = _M_end_of_storage = 0;
  }

 protected:
  _Tp* _M_start;
  _Tp* _M_finish;
  _Tp* _M_end_of_storage;

  typedef simple_alloc<_Tp, _Alloc> _M_data_allocator;
  _Tp* _M_allocate(size_t __n)
    { return _M_data_allocator::allocate(__n); }
  void _M_deallocate(_Tp* __p, size_t __n) 
    { _M_data_allocator::deallocate(__p, __n); }
};


template <class _Tp, class _Alloc = alloc>
class vector : protected _Vector_base<_Tp, _Alloc> {

 private:
  typedef _Vector_base<_Tp, _Alloc> _Base;

 public:
  typedef _Tp value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type* iterator;
  typedef const value_type* const_iterator;
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
  using _Base::_M_start;
  using _Base::_M_finish;
  using _Base::_M_end_of_storage;

 public:
  explicit vector(const allocator_type& __a = allocator_type())
    : _Base(__a) {}

  vector(size_type __n, 
         const allocator_type& __a = allocator_type())
    : _Base(__n, __a)
    { _M_finish = tinySTL::uninitialized_fill_n(_M_start, __n, _Tp()); }

  vector(size_type __n, const _Tp& __value,
         const allocator_type& __a = allocator_type()) 
    : _Base(__n, __a)
    { _M_finish = tinySTL::uninitialized_fill_n(_M_start, __n, __value); }

  vector(const vector& __x)
    : _Base(__x.size(), __x.get_allocator())
    { _M_finish = tinySTL::uninitialized_copy(__x.begin(), __x.end(), _M_start); }

  vector(vector&& __x)
    : _Base(__x.get_allocator())
    {
      _M_start = __x._M_start;
      _M_finish = __x._M_finish;
      _M_end_of_storage = __x._M_end_of_storage;
      __x._M_start = __x._M_finish = __x._M_end_of_storage = 0;
    }

  vector(std::initializer_list<_Tp> __l, 
         const allocator_type& __a = allocator_type())
    : _Base(__l.size(), __a)
    { _M_finish = tinySTL::uninitialized_copy(__l.begin(), __l.end(), _M_start); }

  template <InputIterator Iterator>
  vector(Iterator __first, Iterator __last,
         const allocator_type& __a = allocator_type())
    : _Base(tinySTL::distance(__first, __last), __a)
    { _M_finish = tinySTL::uninitialized_copy(__first, __last, _M_start); }

  ~vector() { tinySTL::destroy(_M_start, _M_finish); }

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
      tinySTL::destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __x._M_start;
      _M_finish = __x._M_finish;
      _M_end_of_storage = __x._M_end_of_storage;
      __x._M_start = 0;
      __x._M_finish = 0;
      __x._M_end_of_storage = 0;
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
      _M_finish = tinySTL::uninitialized_fill_n(_M_finish, __n - size(), __val);
    } else {
      erase(tinySTL::fill_n(begin(), __n, __val), end());
    }
  }

  template <InputIterator Iterator>
  void assign(Iterator __first, Iterator __last)
    { _M_assign_aux(__first, __last, iterator_category(__first)); }

 public:
  allocator_type get_allocator() const { return _Base::get_allocator(); }
  
  iterator begin() { return _M_start; }
  
  const_iterator begin() const { return _M_start; }

  const_iterator cbegin() const { return _M_start; }
  
  iterator end() { return _M_finish; }
  
  const_iterator end() const { return _M_finish; }

  const_iterator cend() const { return _M_finish; }

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
    { return size_type(_M_end_of_storage - begin()); }

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
    if (_M_start != _M_finish) {
      --_M_finish;
      tinySTL::destroy(_M_finish);
    }
  }

  iterator insert(const_iterator __pos, _Tp&& __x)
    { return emplace(__pos, tinySTL::move(__x)); }

  iterator insert(const_iterator __pos, const _Tp& __x)
    { return emplace(__pos, __x); }

  iterator insert(const_iterator __pos, size_type __n, const _Tp& __x)
    { return _M_insert_aux(const_cast<iterator>(__pos), __n, __x); }
  

  iterator insert(const_iterator __pos, std::initializer_list<_Tp> __l)
    { return insert(__pos, __l.begin(), __l.end()); }

  template <InputIterator Iterator>
  iterator insert(const_iterator __position, Iterator __first, Iterator __last)
  {
    iterator __pos = const_cast<iterator>(__position);
    if (__first == __last) return __pos;
      
    size_type __n = tinySTL::distance(__first, __last);
    const size_type __elems_before = __pos - _M_start;
    if (size_type(_M_end_of_storage - _M_finish) >= __n) {
      const size_type __elems_after = _M_finish - __pos;
      iterator __old_finish = _M_finish;
      if (__elems_after > __n) {
        tinySTL::uninitialized_move(_M_finish - __n, _M_finish, _M_finish);
        _M_finish += __n;
        tinySTL::move_backward(__pos, __old_finish - __n, __old_finish);
        tinySTL::copy(__first, __last, __pos);
      } else {
        _M_finish += __n - __elems_after;
        tinySTL::uninitialized_move(__pos, __old_finish, _M_finish);
        pair __copy_n_result = tinySTL::copy_n(__first, __elems_after, __pos);
        tinySTL::uninitialized_copy(__copy_n_result.first, __last, __old_finish);
        _M_finish += __elems_after;
      }
    } else {
      const size_type __old_size = size();
      const size_type __len = __old_size + max(__old_size, __n);
      iterator __new_start = _M_allocate(__len);
      iterator __new_finish = __new_start;
      try {
        __new_finish = tinySTL::uninitialized_move(_M_start, __pos, __new_start);
        __new_finish = tinySTL::uninitialized_copy(__first, __last, __new_finish);
        __new_finish = tinySTL::uninitialized_move(__pos, _M_finish, __new_finish);
      } catch (...) {
        tinySTL::destroy(__new_start,__new_finish);
        _M_deallocate(__new_start,__len);
        throw;
      }
      tinySTL::destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len;
    }
    return _M_start + __elems_before;
  }

  void swap(vector& __x) {
    tinySTL::swap(_M_start, __x._M_start);
    tinySTL::swap(_M_finish, __x._M_finish);
    tinySTL::swap(_M_end_of_storage, __x._M_end_of_storage);
  }

  iterator erase(const_iterator __position) {
    iterator __pos = const_cast<iterator>(__position);
    _M_range_check(size_type(__pos - _M_start));
    if (__pos + 1 != end())
      tinySTL::move(__pos + 1, _M_finish, __pos);
    --_M_finish;
    tinySTL::destroy(_M_finish);
    return __pos;
  }

  iterator erase(const_iterator first, const_iterator last) {
    iterator __first = const_cast<iterator>(first);
    iterator __last = const_cast<iterator>(last);
    
    if (__first - _M_start > this->size())
      __tiny_throw_range_error("vector");
    if ( __last - _M_start > this->size())
      __tiny_throw_range_error("vector");

    if ((__first - _M_start) < 0 || (__last - _M_start) < 0)
      __tiny_throw_range_error("vector");
    
    if (__last - __first > 0) {
      iterator __i = tinySTL::move(__last, _M_finish, __first);
      tinySTL::destroy(__i, _M_finish);
      _M_finish = _M_finish - (__last - __first);
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
      iterator __tmp = _M_allocate_and_move(__n, _M_start, _M_finish);
      tinySTL::destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __tmp;
      _M_finish = __tmp + __old_size;
      _M_end_of_storage = _M_start + __n;
    }
  }

  void shrink_to_fit() {
    if (_M_finish == _M_end_of_storage) return;
    if (_M_start == _M_finish) {
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = _M_finish = _M_end_of_storage = 0;
    } else {
      const size_type __size = size();
      iterator __tmp = _M_allocate_and_move(__size, _M_start, _M_finish);
      tinySTL::destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __tmp;
      _M_finish = __tmp + __size;
      _M_end_of_storage = _M_finish;
    }
  }

  const _Tp* data() const { return _M_start; }

  _Tp* data() { return _M_start; }

  template <typename... _Args>
  iterator emplace(const_iterator __pos, _Args&& ...__args)
  {
    size_type __n = __pos - begin();
    if (_M_finish != _M_end_of_storage && __pos == end()) {
      tinySTL::construct(_M_finish, forward<_Args>(__args)...);
      ++_M_finish;
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
  iterator _M_allocate_and_copy(size_type __n, _ForwardIterator __first, 
                                               _ForwardIterator __last)
  {
    iterator __result = _M_allocate(__n);
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
  iterator _M_allocate_and_move(size_type __n, _ForwardIterator __first, 
                                               _ForwardIterator __last)
  {
    iterator __result = _M_allocate(__n);
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
      iterator __tmp = _M_allocate_and_copy(__len, __first, __last);
      tinySTL::destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __tmp;
      _M_end_of_storage = _M_finish = _M_start + __len;
    }
    else if (size() >= __len) {
      iterator __new_finish = tinySTL::copy(__first, __last, _M_start);
      tinySTL::destroy(__new_finish, _M_finish);
      _M_finish = __new_finish;
    }
    else {
      _ForwardIterator __mid = __first;
      tinySTL::advance(__mid, size());
      tinySTL::copy(__first, __mid, _M_start);
      _M_finish = tinySTL::uninitialized_copy(__mid, __last, _M_finish);
    }
  }

  // _M_finish == _M_end_of_storage || _M_finish != __position
  template <typename... _Args>
  void _M_emplace_aux(const_iterator __pos, _Args&& ...__args)
  {
    iterator __position = const_cast<iterator>(__pos);
    if (_M_finish != _M_end_of_storage) {
      tinySTL::construct(_M_finish, *(_M_finish - 1));
      ++_M_finish;
      tinySTL::move_backward(__position, _M_finish - 2, _M_finish - 1);
      *__position = _Tp(std::forward<_Args>(__args)...);
    } else {
      const size_type __old_size = size();
      const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
      iterator __new_start = _M_allocate(__len);
      iterator __new_finish = __new_start;
      try {
        __new_finish = tinySTL::uninitialized_move(_M_start, __position, __new_start);
        tinySTL::construct(__new_finish, std::forward<_Args>(__args)...);
        ++__new_finish;
        __new_finish = tinySTL::uninitialized_move(__position, _M_finish, __new_finish);
      } catch (...) {
        tinySTL::destroy(__new_start, __new_finish);
        _M_deallocate(__new_start, __len);
        throw;
      }
      tinySTL::destroy(begin(), end());
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len;
    }
  }

  iterator _M_insert_aux(iterator __pos, size_type __n, 
                         const _Tp& __x, bool extent_fix = false) 
  {
    if (__n == 0) return __pos;

    const size_type __elems_before = __pos - _M_start;
    if (size_type(_M_end_of_storage - _M_finish) >= __n) {
      _Tp __x_copy = __x;
      const size_type __elems_after = _M_finish - __pos;
      iterator __old_finish = _M_finish;
      if (__elems_after > __n) {
        tinySTL::uninitialized_move(_M_finish - __n, _M_finish, _M_finish);
        _M_finish += __n;
        tinySTL::move_backward(__pos, __old_finish - __n, __old_finish);
        tinySTL::fill(__pos, __pos + __n, __x_copy);
      } else {
        tinySTL::uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
        _M_finish += __n - __elems_after;
        tinySTL::uninitialized_move(__pos, __old_finish, _M_finish);
        _M_finish += __elems_after;
        tinySTL::fill(__pos, __old_finish, __x_copy);
      }
    } else {
      const size_type __old_size = size();        
      const size_type __len = __old_size + (extent_fix ? __n : max(__old_size, __n));
      iterator __new_start = _M_allocate(__len);
      iterator __new_finish = __new_start;
      try {
        __new_finish = tinySTL::uninitialized_move(_M_start, __pos, __new_start);
        __new_finish = tinySTL::uninitialized_fill_n(__new_finish, __n, __x);
        __new_finish = tinySTL::uninitialized_move(__pos, _M_finish, __new_finish);
      } catch (...) {
        tinySTL::destroy(__new_start, __new_finish);
        _M_deallocate(__new_start, __len);
        throw;
      }
      tinySTL::destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len;
    }
    return _M_start + __elems_before;
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