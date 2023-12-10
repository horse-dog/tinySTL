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
  
    static constexpr size_t _S_buffer_size()
    { return __deque_buf_size(sizeof(value_type)); }

    static constexpr inline size_t 
    __deque_buf_size(size_t __size)
    {
      // TODO: replace by _S_deque_buffer_size ? .
      // TODO: 8 -> 512.
      return __size < 8 ? (8 / __size) : 1;
    }

   // TODO: protected.
   public:
    using _Map_pointer = _Tp**;

    pointer       _M_cur;
    pointer     _M_first;
    pointer      _M_last;
    _Map_pointer _M_node;

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
  const static size_type _S_deque_buffer_size = 8; // TODO: 8 -> 512.

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

  _Deque_base(_Deque_base&& __x)
  : _M_map(), _M_map_size(0), _M_start(), _M_finish()
  {
    _M_initialize_map(0);
    _M_swap_data(__x);
  }

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

  void _M_swap_data(_Deque_base& __x) noexcept
  {
    tinySTL::swap(_M_map, __x._M_map);
    tinySTL::swap(_M_map_size, __x._M_map_size);
    tinySTL::swap(_M_start, __x._M_start);
    tinySTL::swap(_M_finish, __x._M_finish);
  }

 protected:
  map_pointer    _M_map;
  size_type _M_map_size;
  iterator     _M_start;
  iterator    _M_finish;
};

template <class _Tp, class _Alloc = alloc> 
class deque : protected _Deque_base<_Tp, _Alloc> {

 protected:
  using _Base = _Deque_base<_Tp, _Alloc>;
  using _Map_pointer = _Base::map_pointer;
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

  // TODO: uninitialized_fill or uninitialized_default ? .
  explicit deque(size_type __n) : _Base(allocator_type(), __n)
    { tinySTL::uninitialized_fill(begin(), end(), _Tp()); }

  deque(size_type __n, const _Tp& __value,
        const allocator_type& __a = allocator_type())
    : _Base(__a, __n)
    { tinySTL::uninitialized_fill(begin(), end(), __value); }

  deque(const deque& __x) 
    : _Base(__x.get_allocator(), __x.size()) 
    { tinySTL::uninitialized_copy(__x.begin(), __x.end(), _M_start); }

  deque(deque&& __x) = default;

  deque(std::initializer_list<_Tp> __l, 
        const allocator_type& __a = allocator_type())
    : _Base(__a)
    {
      _M_range_initialize(
        __l.begin(), __l.end(), random_access_iterator_tag());
    }

  template <InputIterator Iterator>
  deque(Iterator __first, Iterator __last,
       const allocator_type& __a = allocator_type())
    : _Base(__a)
    {
      _M_range_initialize(
        __first, __last, tinySTL::iterator_category(__first));
    }

  ~deque() { tinySTL::destroy(_M_start, _M_finish); }

  deque& operator=(std::initializer_list<_Tp> __l)
  {
    _M_assign_aux(__l.begin(), __l.end(),
          random_access_iterator_tag());
    return *this;
  }

  deque& operator=(const deque& __x) {
    const size_type __len = size();
    if (&__x != this) {
      if (__len >= __x.size())
        erase(tinySTL::copy(__x.begin(), __x.end(), _M_start), _M_finish);
      else {
        iterator __mid = __x.begin() + difference_type(__len);
        tinySTL::copy(__x.begin(), __mid, _M_start);
        insert(_M_finish, __mid, __x.end());
      }
    }
    return *this;
  }

  deque& operator=(deque&& __x)
  {
    if (&__x != this) {
      this->_M_swap_data(__x);
      __x.clear();
    }
    return *this;
  }

  void assign(std::initializer_list<_Tp> __l)
  { _M_assign_aux(__l.begin(), __l.end(), random_access_iterator_tag()); }

  void assign(size_type __n, const _Tp& __val)
  {
    if (__n > size()) {
      tinySTL::fill(begin(), end(), __val);
      insert(end(), __n - size(), __val);
    } else {
      _M_erase_at_end(begin() + difference_type(__n));
      tinySTL::fill(begin(), end(), __val);
    }
  }

  template <InputIterator Iterator>
  void assign(Iterator __first, Iterator __last)
  { _M_assign_aux(__first, __last, iterator_category(__first)); }

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

  size_type size() const { return _M_finish - _M_start; }

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

  void clear() 
  { _M_erase_at_end(begin()); }

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

  void _M_erase_at_end(iterator __pos)
  {
    _M_destroy_data(__pos, end());
    _M_destroy_nodes(__pos._M_node + 1, _M_finish._M_node + 1);
    this->_M_finish = __pos;
  }
  
  void _M_destroy_data(iterator __first, iterator __last)
  {
    for (_Map_pointer __node = __first._M_node + 1;
         __node < __last._M_node; ++__node)
      tinySTL::destroy(*__node, *__node + 
               iterator::__deque_buf_size(sizeof(_Tp)));
    if (__first._M_node != __last._M_node) {
      tinySTL::destroy(__first._M_cur, __first._M_last);
      tinySTL::destroy(__last._M_first, __last._M_cur);
    } else
      tinySTL::destroy(__first._M_cur, __last._M_cur);
  }

  template <class _InputIterator>
  void _M_range_initialize(
    _InputIterator __first, _InputIterator __last,
    input_iterator_tag) 
  {
    _Base::_M_initialize_map(0);
    try {
      for ( ; __first != __last; ++__first)
        push_back(*__first);
    } catch (...) {
      clear();
      throw;
    }
  }

  template <class _ForwardIterator>
  void _M_range_initialize(
    _ForwardIterator __first, _ForwardIterator __last,
    forward_iterator_tag) 
  {
    size_type __n = tinySTL::distance(__first, __last);
    _Base::_M_initialize_map(__n);

    _Map_pointer __cur_node;
    const size_t __bufsz = iterator::_S_buffer_size();
    try {
      for (__cur_node = _M_start._M_node; 
           __cur_node < _M_finish._M_node; 
           ++__cur_node) {
        _ForwardIterator __mid = __first;
        __mid = tinySTL::uninitialized_copy_n(
                __first, __bufsz, *__cur_node).first;
        __first = __mid;
      }
      tinySTL::uninitialized_copy(__first, __last, _M_finish._M_first);
    } catch (...) {
      tinySTL::destroy(_M_start, iterator(*__cur_node, __cur_node));
      throw;
    }
  }

  template <class _InputIterator>
  void _M_assign_aux(_InputIterator __first, _InputIterator __last,
                     input_iterator_tag)
  {
    iterator __cur = begin();
    for (; __first != __last && __cur != end(); ++__cur, ++__first)
      *__cur = *__first;
    if (__first == __last)
      _M_erase_at_end(__cur);
    else
      _M_range_insert_aux(end(), __first, __last,
              iterator_category(__first));
  }

  template <class _ForwardIterator>
  void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
                     forward_iterator_tag)
  {
    const size_type __len = tinySTL::distance(__first, __last);
    if (__len > size())
      {
        _ForwardIterator __mid = tinySTL::copy_n(__first, __len, begin()).first;
        _M_range_insert_aux(end(), __mid, __last, iterator_category(__first));
      }
    else
      _M_erase_at_end(tinySTL::copy(__first, __last, begin()));
  }

  // TODO: unfinished...
  template <class _ForwardIterator>
  void _M_range_insert_aux(iterator __pos,
        _ForwardIterator __first, _ForwardIterator __last,
        input_iterator_tag);

  template <class _ForwardIterator>
  void _M_range_insert_aux(iterator __pos,
        _ForwardIterator __first, _ForwardIterator __last,
        forward_iterator_tag)
  {
    const size_type __n = tinySTL::distance(__first, __last);
    if (__pos._M_cur == this->_M_start._M_cur) {
      // insert at begin.
      iterator __new_start = _M_reserve_elements_at_front(__n);
      try {
        tinySTL::uninitialized_copy(__first, __last, __new_start);
        this->_M_start = __new_start;
      } catch (...) {
        _M_destroy_nodes(__new_start._M_node, _M_start._M_node);
        throw;
      }
    } else if (__pos._M_cur == this->_M_finish._M_cur) {
      // insert at end.
      iterator __new_finish = _M_reserve_elements_at_back(__n);
      try {
        tinySTL::uninitialized_copy(__first, __last, _M_finish);
        this->_M_finish = __new_finish;
      } catch (...) {
        _M_destroy_nodes(_M_finish._M_node + 1, __new_finish._M_node + 1);
        throw;
      }
    } else {
      // insert not at begin and end.
      _M_insert_aux(__pos, __first, __last, __n);
    }
  }

  template <class _ForwardIterator>
  void _M_insert_aux(iterator __pos,
        _ForwardIterator __first, _ForwardIterator __last,
        size_type __n) {
    const difference_type __elemsbefore = __pos - _M_start;
    const size_type __length = size();
    if ((size_type)__elemsbefore < __length / 2) {
      // __pos is at the left of mid.
      iterator __new_start = _M_reserve_elements_at_front(__n);
      iterator __old_start = this->_M_start;
      // refresh __pos because _M_start may be realloc.
      __pos = this->_M_start + __elemsbefore;
      try {
        if (__elemsbefore >= difference_type(__n)) {
          iterator __start_n = _M_start + difference_type(__n);
          tinySTL::uninitialized_move(_M_start, __start_n, __new_start);
          _M_start = __new_start;
          tinySTL::move(__start_n, __pos, __old_start);
          tinySTL::copy(__first, __last, __pos - difference_type(__n));
        } else {
          _ForwardIterator __mid = __first;
          tinySTL::advance(__mid, difference_type(__n) - __elemsbefore);
          iterator __midm = tinySTL::uninitialized_move(
                                     _M_start, __pos, __new_start);
          tinySTL::uninitialized_copy(__first, __mid, __midm);
          _M_start = __new_start;
          tinySTL::copy(__mid, __last, __old_start);
        }
      } catch (...) {
        _M_destroy_nodes(__new_start._M_node, _M_start._M_node);
        throw;
      }
    } else {
      iterator __new_finish = _M_reserve_elements_at_back(__n);
      iterator __old_finish = this->_M_finish;
      const difference_type __elemsafter = difference_type(__length)
                                           - __elemsbefore;
      // refresh __pos because _M_finish may be realloc.
      __pos = this->_M_finish - __elemsafter;
      try {
        if (__elemsafter > difference_type(__n)) {
          iterator __finish_n = this->_M_finish - difference_type(__n);
          tinySTL::uninitialized_move(__finish_n, _M_finish, _M_finish);
          _M_finish = __new_finish;
          tinySTL::move_backward(__pos, __finish_n, __old_finish);
          tinySTL::copy(__first, __last, __pos);
        } else {
          _ForwardIterator __mid = __first;
          tinySTL::advance(__mid, __elemsafter);
          iterator __midm = tinySTL::uninitialized_copy(
                                     __mid, __last, _M_finish);
          tinySTL::uninitialized_move(__pos, _M_finish, __midm);
          _M_finish = __new_finish;
          tinySTL::copy(__first, __mid, __pos);
        }
      } catch (...) {
        _M_destroy_nodes(_M_finish._M_node + 1, __new_finish._M_node + 1);
        throw;
      }
    }
  }

  iterator 
  _M_reserve_elements_at_front(size_type __n)
  {
    const size_type __vacancies = _M_start._M_cur - _M_start._M_first;
    if (__n > __vacancies) 
	    _M_new_elements_at_front(__n - __vacancies);
    return _M_start - difference_type(__n);
  }

  iterator
  _M_reserve_elements_at_back(size_type __n)
  {
    const size_type __vacancies = _M_finish._M_last - _M_finish._M_cur - 1;
    if (__n > __vacancies)
      _M_new_elements_at_back(__n - __vacancies);
    return _M_finish + difference_type(__n);
  }

  void _M_new_elements_at_front(size_type __new_elems) {
    if (this->max_size() - this->size() < __new_elems)
      __tiny_throw_length_error("deque::_M_new_elements_at_front");
    
    const size_type __new_nodes = 
      (__new_elems + iterator::_S_buffer_size() - 1)
      / iterator::_S_buffer_size();
    _M_reserve_map_at_front(__new_nodes);
    size_type __i;
    try {
      for (__i = 1; __i <= __new_nodes; ++__i)
        *(_M_start._M_node - __i) = _Base::_M_allocate_node();
    } catch (...) {
      for (size_type __j = 1; __j < __i; ++__j)
        _M_deallocate_node(*(_M_start._M_node - __j));
      throw;
    }
  }

  void _M_new_elements_at_back(size_type __new_elems) {
    if (this->max_size() - this->size() < __new_elems)
      __tiny_throw_length_error("deque::_M_new_elements_at_back");

    const size_type __new_nodes = 
      (__new_elems + iterator::_S_buffer_size() - 1) 
      / iterator::_S_buffer_size();
    _M_reserve_map_at_back(__new_nodes);
    size_type __i;
    try {
      for (__i = 1; __i <= __new_nodes; ++__i)
        *(_M_finish._M_node + __i) = _Base::_M_allocate_node();
    } catch (...) {
      for (size_type __j = 1; __j < __i; ++__j)
        _M_deallocate_node(*(_M_finish._M_node + __j));
      throw;
    }
  }

  void _M_reserve_map_at_front(size_type __nodes_to_add = 1)
  {
    if (__nodes_to_add > size_type(_M_start._M_node - _M_map))
      _M_reallocate_map(__nodes_to_add, true);
  }

  void _M_reserve_map_at_back(size_type __nodes_to_add = 1)
  {
    if (__nodes_to_add + 1 
        > _M_map_size - (_M_finish._M_node - _M_map))
      _M_reallocate_map(__nodes_to_add, false);
  }

  // TODO: unfinished at 2023/12/10/23:23
  void
  _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front);
  
};

}