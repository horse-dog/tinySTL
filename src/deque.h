#pragma once

#include "tiny_pair.h"
#include "tiny_alloc.h"
#include "tiny_errors.h"
#include "tiny_concepts.h"
#include "tiny_iterator.h"
#include "tiny_uninitialized.h"

namespace tinySTL
{

template <class _Tp, class _Alloc> 
class deque {

 protected:
  class _Deque_iterator {
   friend class deque;
   public:
    using iterator_category = tinySTL::random_access_iterator_tag;
    using value_type = _Tp;
    using difference_type = ptrdiff_t;
    using pointer = _Tp*;
    using reference = _Tp&;

   protected:
    using Map_ptr = _Tp**;

    pointer   _M_cur;
    pointer _M_first;
    pointer  _M_last;
    Map_ptr  _M_node;

    static constexpr size_t _S_buffer_size()
    { return __deque_buf_size(sizeof(value_type)); }

    static constexpr inline size_t 
    __deque_buf_size(size_t __size)
    {
      return __size < 512 ? (512/__size) : 1;
    }

    constexpr void _M_set_node(Map_ptr __new_node) 
    {
      _M_node  =  __new_node;
      _M_first = *__new_node;
      _M_last  = _M_first + _S_buffer_size();
    }

    constexpr _Deque_iterator(pointer __x, Map_ptr __y)
    : _M_cur(__x), _M_first(*__y),
      _M_last(*__y + _S_buffer_size()),
      _M_node(__y) {}

   public:
    constexpr _Deque_iterator()
    : _M_cur(), _M_first(), _M_last(), _M_node() {}

    constexpr reference operator*() const 
      { return *_M_cur; }

    constexpr pointer operator->() const
      { return _M_cur; }

    constexpr _Deque_iterator& operator++() {
      ++_M_cur;
      if (_M_cur == _M_last) {
        _M_set_node(_M_node + 1);
        _M_cur = _M_first;
      }
      return *this;
    }

    constexpr _Deque_iterator operator++(int) {
      _Deque_iterator __tmp = *this;
      ++(*this);
      return __tmp;
    }

    constexpr _Deque_iterator& operator--() {
      if (_M_cur == _M_first) {
        _M_set_node(_M_node - 1);
        _M_cur = _M_last;
      }
      --_M_cur;
      return *this;
    }

    constexpr _Deque_iterator operator--(int) {
      _Deque_iterator __tmp = *this;
      --(*this);
      return __tmp;
    }

    constexpr _Deque_iterator& 
    operator+=(difference_type __n) {
      const difference_type __offset = __n + (_M_cur - _M_first);
      if (__offset >= 0 && __offset < _S_buffer_size())
        _M_cur += __n;
      else {
        const difference_type __node_offset = 
          __offset > 0 ? __offset / _S_buffer_size()
        : -difference_type((-__offset - 1) / _S_buffer_size()) - 1;
        _M_set_node(_M_node + __node_offset);
        _M_cur = _M_first + (__offset - __node_offset * _S_buffer_size());
      }
      return *this;
    }

    constexpr _Deque_iterator&
    operator-=(difference_type __n) 
      { return *this += -__n; }

    // constexpr reference 
    // operator[](difference_type __n) const
    //   { return *(*this + __n); }
  };

};

}