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
class _Deque_base {
 
 protected:
  class _Deque_iterator {
   friend class _Deque_base;
   public:
    using iterator_category = tinySTL::random_access_iterator_tag;
    using value_type = _Tp;
    using difference_type = ptrdiff_t;
    using pointer = _Tp*;
    using reference = _Tp&;

   protected:
    using _Map_pointer = _Tp**;

    pointer       _M_cur;
    pointer     _M_first;
    pointer      _M_last;
    _Map_pointer _M_node;

    static constexpr size_t _S_buffer_size()
    { return __deque_buf_size(sizeof(value_type)); }

    static constexpr inline size_t 
    __deque_buf_size(size_t __size)
    {
      // TODO: replace by _S_deque_buffer_size ? .
      return __size < 512 ? (512 / __size) : 1;
    }

    constexpr void _M_set_node(_Map_pointer __new_node) 
    {
      _M_node  =  __new_node;
      _M_first = *__new_node;
      _M_last  = _M_first + _S_buffer_size();
    }

    constexpr _Deque_iterator(pointer __x, _Map_pointer __y)
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

    constexpr _Deque_iterator
    operator+(difference_type __n) const {
      _Deque_iterator __tmp = *this;
      __tmp += __n;
      return __tmp;
    }

    constexpr _Deque_iterator
    operator-(difference_type __n) const {
      _Deque_iterator __tmp = *this;
      __tmp -= __n;
      return __tmp;
    }

    constexpr reference 
    operator[](difference_type __n) const
    { return *(*this + __n); }

    // TODO: what ? .
    constexpr difference_type
    operator-(const _Deque_iterator& __right) const {
      return difference_type(_S_buffer_size()) 
           * (_M_node - __right._M_node - int(_M_node != 0))
           + (_M_cur - _M_first) 
           + (__right._M_last - __right._M_cur);
    }

    friend constexpr bool
    operator==(const _Deque_iterator& __x,
               const _Deque_iterator& __y)
      {
        return __x._M_cur == __y._M_cur;
      }

    friend constexpr bool
    operator!=(const _Deque_iterator& __x,
               const _Deque_iterator& __y)
      {
        return !(__x == __y);
      }

    friend constexpr _Deque_iterator 
    operator+(difference_type __n,
       const _Deque_iterator& __x)
      {
        return __x + __n;
      }
  };

 public:
  typedef _Tp value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef _Deque_iterator iterator;
  typedef tinySTL::const_iterator<iterator> const_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Alloc allocator_type;
  typedef tinySTL::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef tinySTL::reverse_iterator<iterator> reverse_iterator;

 protected:
  typedef pointer* map_pointer;
  typedef simple_alloc<pointer, _Alloc> _Map_alloc_type;
  typedef simple_alloc<value_type, _Alloc> _Node_alloc_type;

  const static size_type _S_initial_map_size = 8;
  const static size_type _S_deque_buffer_size = 512;

 public:
  _Deque_base() 
  : _M_map(), _M_map_size(0), _M_start(), _M_finish()
  { _M_initialize_map(0); }

  _Deque_base(size_t __num_elements)
  : _M_map(), _M_map_size(0), _M_start(), _M_finish()
  { _M_initialize_map(__num_elements); }

  _Deque_base(const allocator_type&, size_t __num_elements)
  : _M_map(), _M_map_size(0), _M_start(), _M_finish()
  { _M_initialize_map(__num_elements); }

  _Deque_base(const allocator_type&)
  : _M_map(), _M_map_size(0), _M_start(), _M_finish()
  { /* Caller must initialize map. */ }

 ~_Deque_base()
  {
    if (_M_map != 0) {
      _M_destroy_nodes(_M_start._M_node, _M_finish._M_node + 1);
      _M_deallocate_map(_M_map, _M_map_size);
    }
  }

  allocator_type
  get_allocator() const noexcept
  { return allocator_type(); }

 protected:
  _Tp** _M_allocate_map(size_t __n) 
    { return _Map_alloc_type::allocate(__n); }

  void _M_deallocate_map(_Tp** __p, size_t __n) 
    { _Map_alloc_type::deallocate(__p, __n); }

  _Tp* _M_allocate_node()
    { 
      return _Node_alloc_type::allocate(
        iterator::__deque_buf_size(sizeof(_Tp)));
    }

  void _M_deallocate_node(_Tp* __p)
    { 
      _Node_alloc_type::deallocate(__p, 
      iterator::__deque_buf_size(sizeof(_Tp))); 
    }

  void _M_create_nodes(_Tp** __nstart, _Tp** __nfinish) {
    _Tp** __cur;
    try {
      for (__cur = __nstart; __cur < __nfinish; ++__cur)
        *__cur = _M_allocate_node();
    } catch (...) {
      _M_destroy_nodes(__nstart, __cur);
      throw;
    }
  }

  void _M_destroy_nodes(_Tp** __nstart, _Tp** __nfinish)
    {
      for (_Tp** __n = __nstart; __n < __nfinish; ++__n)
        _M_deallocate_node(*__n);
    }

  void _M_initialize_map(size_t __num_elements) {
    size_t __num_nodes = 
      __num_elements / iterator::__deque_buf_size(sizeof(_Tp)) + 1;
    _M_map_size = tinySTL::max(_S_initial_map_size, __num_nodes + 2);
    _M_map = _M_allocate_map(_M_map_size);
    _Tp** __nstart = _M_map + (_M_map_size - __num_nodes) / 2;
    _Tp** __nfinish = __nstart + __num_nodes;

    try {
      _M_create_nodes(__nstart, __nfinish);
    } catch (...) {
      _M_deallocate_map(_M_map, _M_map_size);
      _M_map = 0;
      _M_map_size = 0;
      throw;
    }
    _M_start._M_set_node(__nstart);
    _M_finish._M_set_node(__nfinish - 1);
    _M_start._M_cur = _M_start._M_first;
    _M_finish._M_cur = _M_finish._M_first + __num_elements 
                     % iterator::__deque_buf_size(sizeof(_Tp));
  }

 protected:
  map_pointer    _M_map;
  size_type _M_map_size;
  iterator     _M_start;
  iterator    _M_finish;
};

template <class _Tp, class _Alloc = alloc> 
class deque : public _Deque_base<_Tp, _Alloc> {

 protected:
  using _Base = _Deque_base<_Tp, _Alloc>;
  using _Base::_M_map;
  using _Base::_M_map_size;
  using _Base::_M_start;
  using _Base::_M_finish;

 public:
  typedef _Base::value_type value_type;
  typedef _Base::value_type pointer;
  typedef _Base::const_pointer const_pointer;
  typedef _Base::iterator iterator;
  typedef _Base::const_iterator const_iterator;
  typedef _Base::reference reference;
  typedef _Base::const_reference const_reference;
  typedef _Base::size_type size_type;
  typedef _Base::difference_type difference_type;
  typedef _Base::allocator_type allocator_type;
  typedef _Base::const_reverse_iterator const_reverse_iterator;
  typedef _Base::const_reverse_iterator reverse_iterator;

 public:
  explicit deque(const allocator_type& __a = allocator_type())
    : _Base(__a, 0) {}

  explicit deque(size_type __n) : _Base(allocator_type(), __n)
    { tinySTL::uninitialized_default(begin(), end()); }

  deque(size_type __n, const _Tp& __value,
        const allocator_type& __a = allocator_type())
    : _Base(__a, __n)
    { tinySTL::uninitialized_fill(begin(), end(), __value); }

  deque(const deque& __x) 
    : _Base(__x.get_allocator(), __x.size()) 
    { tinySTL::uninitialized_copy(__x.begin(), __x.end(), _M_start); }

  deque(deque&& __x);

  deque(std::initializer_list<_Tp> __l, 
        const allocator_type& __a = allocator_type())
    : _Base(__a, __l.size())
    { tinySTL::uninitialized_copy(__l.begin(), __l.end(), _M_start); }

  template <InputIterator Iterator>
  deque(Iterator __first, Iterator __last,
       const allocator_type& __a = allocator_type());

  ~deque() { tinySTL::destroy(_M_start, _M_finish); }

  deque& operator=(std::initializer_list<_Tp> __l);

  deque& operator=(const deque& __x);

  deque& operator=(deque&& __x);

  void assign(std::initializer_list<_Tp> __l);

  void assign(size_type __n, const _Tp& __val);

  template <InputIterator Iterator>
  void assign(Iterator __first, Iterator __last);

 public:
  allocator_type get_allocator() const { return allocator_type(); }

  iterator begin() { return _M_start; }

  const_iterator begin() const { return _M_start; }

  const_iterator cbegin() const { return _M_start; }

  iterator end() { return _M_finish; }

  const_iterator end() const { return _M_finish; }

  const_iterator cend() const { return _M_finish; }

  reverse_iterator rbegin() { return reverse_iterator(_M_finish); }

  const_reverse_iterator rbegin() const { return const_reverse_iterator(_M_finish); }

  const_reverse_iterator crbegin() const { return const_reverse_iterator(_M_finish); }

  reverse_iterator rend() { return reverse_iterator(_M_start); }

  const_reverse_iterator rend() const { return const_reverse_iterator(_M_start); }

  const_reverse_iterator crend() const { return const_reverse_iterator(_M_start); }

  bool empty() const { return _M_finish == _M_start; }

  reference at(size_type __n)
  { _M_range_check(__n); return (*this)[__n]; }

  const_reference at(size_type __n) const
  { _M_range_check(__n); return (*this)[__n]; }

  reference operator[](size_type __n)
  { return _M_start[difference_type(__n)]; }

  const_reference operator[](size_type __n) const
  { return _M_start[difference_type(__n)]; }

  size_type size() { return _M_finish - _M_start; }

  size_type max_size() const { return size_type(-1); }

  reference front();

  const_reference front() const;

  reference back();

  const_reference back() const;

  void swap(deque<_Tp, _Alloc>& __x) {
    tinySTL::swap(_M_start, __x._M_start);
    tinySTL::swap(_M_finish, __x._M_finish);
    tinySTL::swap(_M_map, __x._M_map);
    tinySTL::swap(_M_map_size, __x._M_map_size);
  }

  iterator insert(const_iterator __position, _Tp&& __x);

  iterator insert(const_iterator __position, const _Tp& __x);

  iterator insert(const_iterator __position, std::initializer_list<_Tp> __l);

  iterator insert(const_iterator __position, size_type __n, const _Tp& __x);

  template <InputIterator Iterator>
  iterator insert(const_iterator __position, Iterator __first, Iterator __last);

  iterator erase(const_iterator __position);

  iterator erase(const_iterator __first, const_iterator __last);

  void push_front(_Tp&& __x)
  { emplace_front(tinySTL::move(__x)); }

  void push_front(const _Tp& __x)
  { emplace_front(__x); }

  void push_back(_Tp&& __x)
  { emplace_back(tinySTL::move(__x)); }

  void push_back(const _Tp& __x)
  { emplace_back(__x); }

  void clear();

  void resize(size_type __new_size, const _Tp& __x);

  void resize(size_type __new_size);

  void pop_front();

  void pop_back();

  template <class... _Args>
  iterator emplace(const_iterator __pos, _Args &&...__args);

  template <class... _Args>
  reference emplace_back(_Args&&... __args);

  template <class... _Args>
  reference emplace_front(_Args&&... __args);

  void shrink_to_fit();

 protected:
  void _M_range_check(size_type __n) const {
    if (__n >= this->size())
      __tiny_throw_range_error("deque");
  }
};

}