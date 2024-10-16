#pragma once

#include "tiny_alloc.h"
#include "tiny_errors.h"
#include "tiny_iterator.h"
#include "tiny_algobase.h"
#include "tiny_construct.h"

namespace tinySTL
{

struct _Slist_node_base {
  _Slist_node_base* _M_next = 0;

  _Slist_node_base* _M_transfer_after(
                     _Slist_node_base*__begin,
                     _Slist_node_base*__end) noexcept 
  {
    _Slist_node_base* __keep = __begin->_M_next;
    if (__end) {
      __begin->_M_next = __end->_M_next;
      __end->_M_next = this->_M_next;
    } else
      __begin->_M_next = nullptr;
    _M_next = __keep;
    return __end;
  }
};

struct _SList_node_header : public _Slist_node_base
{
  using _Slist_node_base::_M_next;

  _SList_node_header() {}
 
 ~_SList_node_header() { _M_next = 0; }

  _SList_node_header(const _SList_node_header&) = delete;

  _SList_node_header(_SList_node_header&& __x)
    {
      _M_next = __x._M_next;
      __x._M_next = 0;
    }

  _SList_node_header& operator=(const _SList_node_header&) = delete;
  
  _SList_node_header& operator=(_SList_node_header&& __x)
    {
      if (this != &__x) {
        _M_next = __x._M_next;
        __x._M_next = 0;
      }
      return *this;
    }
};

template <class _Tp>
struct _Slist_node : public _Slist_node_base
{
  using _Slist_node_base::_M_next;
  aligned_membuf<_Tp>  _M_storage;

  template <class... _Args>
  _Slist_node(_Args&&... __args)
    { 
      tinySTL::construct(
        _M_storage.ptr(), 
        tinySTL::forward<_Args>(__args)...
      ); 
    }

 ~_Slist_node() 
    { 
      tinySTL::destroy(_M_storage.ptr()); 
    }
};

template <class _Tp, class _Alloc = tinySTL::allocator<_Tp>>
class forward_list
{

 protected:
  class _Slist_iterator
  {
   friend class forward_list;
   public:
    using iterator_category = tinySTL::forward_iterator_tag;
    using value_type = _Tp;
    using difference_type = ptrdiff_t;
    using pointer = _Tp*;
    using reference = _Tp&;

   protected:
    _Slist_node_base* _M_node;
  
   public:
    constexpr _Slist_iterator() : _M_node(0) {}

   protected:
    constexpr _Slist_iterator(const _Slist_node_base* __ptr)
      : _M_node((_Slist_node_base*)__ptr) { }

   public:
    constexpr reference operator*() const
      { return *((_Slist_node<_Tp>*)_M_node)->_M_storage.ptr(); }

    constexpr pointer operator->() const
      { return ((_Slist_node<_Tp>*)_M_node)->_M_storage.ptr(); }

    constexpr _Slist_iterator& operator++() 
      { 
        _M_node = _M_node->_M_next; 
        return *this; 
      }

    constexpr _Slist_iterator operator++(int) 
      {
        _Slist_iterator old(this->_M_node);
        ++(*this);
        return old;
      }

    friend constexpr bool operator==(
      const _Slist_iterator& __x, 
      const _Slist_iterator& __y) 
      { 
        return __x._M_node == __y._M_node;
      }
  };

 public:
  typedef _Tp value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef _Slist_iterator iterator;
  typedef tinySTL::const_iterator<iterator> const_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Alloc allocator_type;
  typedef tinySTL::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef tinySTL::reverse_iterator<iterator> reverse_iterator;

 protected:
  typedef typename _Alloc_rebind<_Alloc, _Slist_node<_Tp>>
            ::type _Node_alloc_type;
  typedef _Slist_node<_Tp> _Node;

  struct _Slist_impl
  : public _Node_alloc_type
  {
    _Slist_impl(const _Node_alloc_type& __a)
    : _Node_alloc_type(__a), _M_header()
      { }

    _Slist_impl(_Slist_impl&& __x) = default;

    _SList_node_header _M_header;
  };

  _Slist_impl _M_impl;

  struct less 
  {
    bool operator()(const _Tp& a, const _Tp& b) { return a < b; }
  };
  struct equal 
  {
    bool operator()(const _Tp& a, const _Tp& b) { return a == b; }
  };

 protected:
  _Node* _M_get_node()
    { return _M_impl.allocate(1); }

  void _M_put_node(_Node* __p) 
    { _M_impl.deallocate(__p, 1); }

  template <class... _Args>
  _Node* _M_create_node(_Args&&... __args)
    {
      _Node* __p = _M_get_node();
      try {
        tinySTL::construct(__p, tinySTL::forward<_Args>(__args)...);
      } catch (...) {
        _M_put_node(__p);
        throw;
      }
      return __p;
    }

  void _M_destroy_node(_Node* __p)
    {
      tinySTL::destroy(__p);
      _M_put_node(__p);
    }

 public:
  explicit forward_list(
      const allocator_type& __a = allocator_type()
    ) 
  : _M_impl(_Node_alloc_type(__a)) { }

  explicit forward_list(
      size_type __n,
      const allocator_type& __a = allocator_type()
    )
  : forward_list(__n, _Tp()) { }

  forward_list(
    size_type __n, const _Tp& __value,
    const allocator_type& __a = allocator_type()
  ) : _M_impl(_Node_alloc_type(__a))
  {
    _Slist_node_base* __head = (_Slist_node_base*)(&_M_impl._M_header);
    _Slist_node_base* __pren = __head;
    _Slist_node_base* __node;
    for (size_type i = 0; i < __n; ++i)
    {
      __node = _M_create_node(__value);
      __pren->_M_next = __node;
      __pren = __node; 
    }
    __pren->_M_next = 0;
  }

  forward_list(const forward_list& __x) 
    : _M_impl(_Node_alloc_type(__x._M_impl))
  { 
    _M_iter_construct(__x.begin(), __x.end());
  }

  forward_list(forward_list&& __x) = default;

  forward_list(
    std::initializer_list<_Tp> __l, 
    const allocator_type& __a = allocator_type()
  ) : _M_impl(_Node_alloc_type(__a))
  { 
    _M_iter_construct(__l.begin(), __l.end()); 
  }

  template <InputIterator Iterator>
  forward_list(
    Iterator __first, Iterator __last,
    const allocator_type& __a = allocator_type()
  ) : _M_impl(_Node_alloc_type(__a))
  { 
    _M_iter_construct(__first, __last);
  }

  ~forward_list() { clear(); }

  forward_list& operator=(std::initializer_list<_Tp> __l)
  {
    assign(__l);
    return *this;
  }

  forward_list& operator=(const forward_list& __x)
  {
    if (this != &__x) 
      assign(__x.begin(), __x.end());
    return *this;
  }

  forward_list& operator=(forward_list&& __x)
  {
    if (this != &__x)
    {
      this->~forward_list();
      tinySTL::construct(this, tinySTL::move(__x));
    }
    return *this;
  }

  void assign(std::initializer_list<_Tp> __l)
    { assign(__l.begin(), __l.end()); }

  void assign(size_type __n, const _Tp& __val)
  {
    iterator __i = begin();
    iterator __prei = before_begin();
    size_type i;
    
    for ( ; __i != end() && __n > 0; ++__i, --__n) 
    {
      *__i = __val;
      __prei = __i;
    }

    if (__n > 0)
      insert_after(__prei, __n, __val);
    else
      erase_after(__prei, end());    
  }

  template <InputIterator Iterator>
  void assign(Iterator __first, Iterator __last)
  {
    iterator __first1 = begin();
    iterator __last1 = end();
    iterator __before1 = before_begin();

    for (; __first1 != __last1 && __first != __last
         ; ++__first1, ++__first)
      {
        *__first1 = *__first;
        __before1 = __first1;
      }

    if (__first == __last)
      erase_after(__before1, __last1);
    else
      insert_after(__before1, __first, __last);
  }

 public:
  allocator_type get_allocator() const 
  { return allocator_type(_M_impl); }

  iterator begin()
    { return iterator(_M_impl._M_header._M_next); }

  const_iterator begin() const
    { return const_iterator(_M_impl._M_header._M_next); }

  const_iterator cbegin() const
    { return const_iterator(_M_impl._M_header._M_next); }

  iterator before_begin()
    { return iterator((_Slist_node_base*)(&_M_impl._M_header)); }

  const_iterator before_begin() const
    { return const_iterator((_Slist_node_base*)(&_M_impl._M_header)); }

  const_iterator cbefore_begin() const
    { return const_iterator((_Slist_node_base*)(&_M_impl._M_header)); }

  iterator end()
    { return iterator(); }

  const_iterator end() const
    { return const_iterator(); }

  const_iterator cend() const
    { return const_iterator(); }

  bool empty() const
    { return _M_impl._M_header._M_next == 0; }

  void clear()
  {
    _Node* __cur  = (_Node*) _M_impl._M_header._M_next;
    _Node* __head = (_Node*) &_M_impl._M_header;

    while (__cur != 0) {
      _Node* __tmp = __cur;
      __cur = (_Node*) __cur->_M_next;
      _M_destroy_node(__tmp);
    }

    _M_impl._M_header._M_next = 0;
  }

  size_type max_size() const { return size_type(-1); }

  reference front()
    { return *begin(); }

  const_reference front() const
    { return *begin(); }

  void swap(forward_list<_Tp, _Alloc>& __x)
    { tinySTL::swap(_M_impl._M_header, __x._M_impl._M_header); }

  iterator insert_after(const_iterator __pos, _Tp&& __val)
    { return emplace_after(__pos, tinySTL::move(__val)); }

  iterator insert_after(const_iterator __pos, const _Tp& __val)
    { return emplace_after(__pos, __val); }

  iterator insert_after(const_iterator __pos, std::initializer_list<_Tp> __l)
    { return insert_after(__pos, __l.begin(), __l.end()); }

  iterator insert_after(const_iterator __pos, size_type __n, const _Tp& __val)
    {
      if (__n == 0) return __pos.base();
      forward_list __tmp(__n, __val);
      iterator __it = __tmp.begin();
      splice_after(__pos, __tmp);
      return __it;
    }

  template <InputIterator Iterator>
  iterator insert_after(const_iterator __pos, Iterator __first,
                        Iterator __last)
    {
      forward_list __tmp(__first, __last);
      if (__tmp.empty()) return __pos.base();
      iterator __it = __tmp.begin();
      splice_after(__pos, __tmp);
      return __it;
    }

  template <class... _Args>
  iterator emplace_after(const_iterator __pos, _Args&& ...__args)
  {
    iterator __position = __pos.base();
    _Node* __tmp = _M_create_node(tinySTL::forward<_Args>(__args)...);
    __tmp->_M_next = __position._M_node->_M_next;
    __position._M_node->_M_next = __tmp;
    return __tmp;
  }

  template <typename... _Args>
  reference emplace_front(_Args&& ...__args)
  {
    return *emplace_after(before_begin(), tinySTL::forward<_Args>(__args)...);
  }

  iterator erase_after(const_iterator __pos)
  {
    iterator __position = __pos.base();
    _Node* __target = (_Node*) __position._M_node->_M_next;
    __position._M_node->_M_next = __target->_M_next;
    _M_destroy_node(__target);
    return ++__position;
  }

  iterator erase_after(const_iterator __pos, const_iterator __last)
  {
    if (__pos == before_begin() && __last == end()) {
      clear();
      return end();
    }
    while (__pos.base()._M_node->_M_next
        != __last.base()._M_node) 
      erase_after(__pos);
    return __last.base();
  }

  void push_front(_Tp&& __val)
    { emplace_after(before_begin(), tinySTL::forward<_Tp>(__val)); }

  void push_front(const _Tp& __val)
    { emplace_after(before_begin(), __val); }

  void pop_front()
    { erase_after(before_begin()); }

  /**
   * @brief merge this forward_list with another forward_list,
   * these two forward_lists should be ordered before use this function
   * @param __list another list
   */
  void merge(forward_list&& __list)
    { merge(__list, less()); }

  /**
   * @brief merge this forward_list with another forward_list,
   * these two forward_lists should be ordered before use this function
   * @param __list another list
   */
  void merge(forward_list& __list)
    { merge(__list, less()); }

  /**
   * @brief merge this forward_list with another forward_list,
   * these two forward_lists should be ordered before use this function
   * @param __list another forward_list
   * @param __comp sort method
   */
  template <class _Comp>
  void merge(forward_list&& __list, _Comp __comp)
    { merge(__list, __comp); }

  /**
   * @brief merge this forward_list with another forward_list,
   * these two forward_lists should be ordered before use this function
   * @param __list another forward_list
   * @param __comp sort method
   */
  template <class _Comp>
  void merge(forward_list& __list, _Comp __comp)
  {
    iterator __first1 = begin(), __last1 = end();
    iterator __first2 = __list.begin(), __last2 = __list.end();
    iterator __pre1 = before_begin(), __pre2 = __list.before_begin();

    while (__first1 != __last1 && __first2 != __last2) {
      if (__comp(*__first2, *__first1)) {
        iterator __old = __first2;
        _M_splice_after(__pre1, __pre2, ++__first2);
        __pre1 = __old;
      } else {
        ++__pre1;
        ++__first1;
      }
    }
    if (__first2 != __last2)
      _M_splice_after(__pre1, __pre2, __last2);
  }

  size_type remove(const _Tp& __value) {
    size_type __cnt = 0;
    iterator __extra_before = end();
    iterator __before = before_begin();
    for (auto __cur = begin(); __cur != end();) {
      if (*__cur == __value) {
        if (tinySTL::addressof(*__cur) 
         != tinySTL::addressof(__value)) {
          __cur = erase_after(__before);
          ++__cnt;
        } else {
          __extra_before = __before;
          ++__before;
          ++__cur;
        }
      } else {
        ++__before;
        ++__cur;
      }
    }
    if (__extra_before != end()) {
      erase_after(__extra_before);
      ++__cnt;
    }
    return __cnt;
  }

  template <class _Pred>
  size_type remove_if(_Pred __pred) {
    size_type __cnt = 0;
    _Slist_node_base* __cur = (_Slist_node_base*)(&_M_impl._M_header);

#define __SLIST_UNWRAP__(x) \
    *((_Node*) x->_M_next)->_M_storage.ptr()

    while (__cur->_M_next) {
      if (__pred(__SLIST_UNWRAP__(__cur))) {
        erase_after(iterator(__cur));
        ++__cnt;
      } else {
        __cur = __cur->_M_next;
      }
    }
#undef __SLIST_UNWRAP__
    return __cnt;
  }

  void resize(size_type __sz) { resize(__sz, _Tp()); }

  void resize(size_type __sz, const _Tp& __val)
  {
    _Slist_node_base* __cur = (_Slist_node_base*)(&_M_impl._M_header);
    while (__cur->_M_next != 0 && __sz > 0) {
      --__sz;
      __cur = __cur->_M_next;
    }
    if (__cur->_M_next)
      erase_after(iterator(__cur), end()); 
    else
      insert_after(iterator(__cur), __sz, __val);
  }

  void reverse()
  {
    if (this->empty()) return;
    _Slist_node_base* __node = _M_impl._M_header._M_next;
    _Slist_node_base* __result = __node;
    __node = __node->_M_next;
    __result->_M_next = 0;
    while (__node) {
      _Slist_node_base* __next = __node->_M_next;
      __node->_M_next = __result;
      __result = __node;
      __node = __next;
    }
    _M_impl._M_header._M_next = __result;
  }

  void sort() { sort(less()); }

  template <class _Comp>
  void sort(_Comp __comp)
  {
    if (_M_impl._M_header._M_next == 0 
     || _M_impl._M_header._M_next->_M_next == 0)
      return;
    
    forward_list __carry;
    forward_list __counter[64];
    int __fill = 0;
    while (!empty()) {
      __carry.splice_after(__carry.before_begin(),
                           *this, this->before_begin());
      int __i = 0;
      while (__i < __fill && !__counter[__i].empty()) {
        __counter[__i].merge(__carry, __comp);
        __carry.swap(__counter[__i]);
        ++__i;
      }
      __carry.swap(__counter[__i]);
      if (__i == __fill)
        ++__fill;
    }

    for (int __i = 1; __i < __fill; ++__i)
      __counter[__i].merge(__counter[__i - 1], __comp);
    this->swap(__counter[__fill - 1]);
  }

  size_type unique() { return unique(equal()); }

  template <class _BinPred>
  size_type unique(_BinPred __binary_pred)
  {
    size_type __cnt = 0;
    for (iterator pre = begin(), p = ++begin(); p != end();)
      if (__binary_pred(*pre, *p)) {
        p = erase_after(pre);
        ++__cnt;
      } else
        pre = p++;
    return __cnt;
  }

  /**
   * @brief splice @p __list to current forward_list after @p __pos .
   *  @p __list will be empty after splice.
   * 
   * @param __pos 
   * @param __list 
   * 
   * @attention @p __list should not be *this.
   */
  void splice_after(const_iterator __pos, forward_list&& __list)
    { splice_after(__pos, __list); }

  /**
   * @brief splice @p __list to current forward_list after @p __pos .
   *  @p __list will be empty after splice.
   * 
   * @param __pos 
   * @param __list 
   * 
   * @attention @p __list should not be *this.
   */
  void splice_after(const_iterator __pos, forward_list& __list)
    {
      if (__list.empty()) return;
      _M_splice_after(__pos.base(), __list.before_begin(), __list.end());
    }

  /**
   * @brief move element after @p __i from @p __list to current forward_list after @p __pos .
   * 
   * @param __pos 
   * @param __list 
   * @param __i 
   * 
   * @attention if @p __list is *this, @p __i +1 should not be @p __pos .
   */
  void splice_after(const_iterator __pos, forward_list&& __list,
                    const_iterator __i)
    { splice_after(__pos, __list, __i); }

  /**
   * @brief move element after @p __i from @p __list to current forward_list after @p __pos .
   * 
   * @param __pos 
   * @param __list 
   * @param __i 
   * 
   * @attention if @p __list is *this, @p __i +1 should not be @p __pos .
   */
  void splice_after(const_iterator __pos, forward_list& __list,
                    const_iterator __i)
    {
      _Slist_node_base* __before_splice = __pos.base()._M_node; 
      _Slist_node_base* __before_first  = __i.base()._M_node;
      _Slist_node_base* __before_last   = __before_first->_M_next;
      if (__before_splice == __before_first 
       || __before_splice == __before_last) return;
      __before_splice->_M_transfer_after(__before_first, __before_last);
    }

  /**
   * @brief move [ @p __before +1, @p __last ) from @p __list to current list after @p __pos .
   * 
   * @param __pos 
   * @param __list 
   * @param __before 
   * @param __last 
   * 
   * @attention if @p __list is *this, @p __pos should not in ( @p __before , @p __last ) .
   */
  void splice_after(const_iterator __pos, forward_list&& __list,
                    const_iterator __before, const_iterator __last)
    { splice_after(__pos, __list, __before, __last); }

  /**
   * @brief move [ @p __before +1, @p __last ) from @p __list to current list after @p __pos .
   * 
   * @param __pos 
   * @param __list 
   * @param __before 
   * @param __last 
   * 
   * @attention if @p __list is *this, @p __pos should not in ( @p __before , @p __last ) .
   */
  void splice_after(const_iterator __pos, forward_list& __list,
                    const_iterator __before, const_iterator __last)
    { _M_splice_after(__pos.base(), __before.base(), __last.base()); }

 protected:
  void _M_splice_after(iterator __pos, iterator __before,
                       iterator __last)
    {
      _Slist_node_base* __before_splice = __pos._M_node;
      _Slist_node_base* __before_first  = __before._M_node;
      _Slist_node_base* __before_last   = __before_first;
      _Slist_node_base* __last_ptr      = __last._M_node;

      while (__before_last && __before_last->_M_next != __last_ptr)
        __before_last = __before_last->_M_next;

      if (__before_last == 0)
        __tiny_throw_range_error("forward_list");

      if (__before_last != __before_first)
        __before_splice->_M_transfer_after(__before_first, __before_last);
    }

  template <class _Iterator>
  void _M_iter_construct(_Iterator __first, _Iterator __last) {
    _Slist_node_base* __head = (_Slist_node_base*)(&_M_impl._M_header);
    _Slist_node_base* __before = __head;
    for (auto it = __first; it != __last; ++it) {
      _Node* __node = _M_create_node(*it);
      __before->_M_next = __node;
      __before = __node;
    }
    __before->_M_next = 0;
  }
};

}