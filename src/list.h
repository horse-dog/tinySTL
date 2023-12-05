// tinySTL: list.
#pragma once

#include "tiny_alloc.h"
#include "tiny_construct.h"

namespace tinySTL
{

struct __list_node_base {
  __list_node_base *_M_prev = 0;
  __list_node_base *_M_next = 0;
};

struct __list_node_head : public __list_node_base
{
  using __list_node_base::_M_prev;
  using __list_node_base::_M_next;
  size_t _M_size = 0;

  __list_node_head() : _M_size(0) 
    { _M_prev = _M_next = this; }

 ~__list_node_head() 
  { 
    _M_prev = _M_next = this;
    _M_size = 0;
  }

  __list_node_head(const __list_node_head&) = delete;

  __list_node_head(__list_node_head&& __x)
  {
    if (__x._M_size == 0)
    {
      _M_prev = this;
      _M_next = this;
      _M_size = 0;
      return;
    }

    _M_size = __x._M_size;
    _M_prev = __x._M_prev;
    _M_next = __x._M_next;
    __x._M_prev->_M_next = this;
    __x._M_next->_M_prev = this;
    __x._M_prev = __x._M_next = &__x;
    __x._M_size = 0;
  }
};

template <class _Tp>
struct __list_node : public __list_node_base
{
  using __list_node_base::_M_prev;
  using __list_node_base::_M_next;
  _Tp                  _M_storage;  // TODO: replaced by aligned memory buf.

  __list_node(const _Tp& __val)
    { tinySTL::construct(&_M_storage, __val); }
};

template <class _Tp, class _Alloc = alloc>
class list
{

};

}