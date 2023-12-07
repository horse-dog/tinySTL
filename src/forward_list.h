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
};

struct _Slist_node_head : public _Slist_node_base
{
  using _Slist_node_base::_M_next;

  _Slist_node_head() {}
 
 ~_Slist_node_head() { _M_next = 0; }

  _Slist_node_head(const _Slist_node_head&) = default;

  // TODO: default ok ? .
  _Slist_node_head(_Slist_node_head&&) = default;
};

template <class _Tp>
struct _Slist_node : public _Slist_node_base
{
  using _Slist_node_base::_M_next;
  aligned_membuf<_Tp>  _M_storage;

  template <class... _Args>
  _Slist_node(_Args&&... __args)
    { tinySTL::construct(_M_storage.ptr(), tinySTL::forward<_Args>(__args)...); }

 ~_Slist_node() 
    { tinySTL::destroy(_M_storage.ptr()); }
};

template <class _Tp, class _Alloc = alloc>
class forward_list
{

 protected:
  _Slist_node_head _M_head;

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
  typedef simple_alloc<_Slist_node<_Tp>, _Alloc> _M_node_allocator;
  typedef _Slist_node<_Tp> _Node;

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
    { return _M_node_allocator::allocate(1); }

  void _M_put_node(_Node* __p) 
    { _M_node_allocator::deallocate(__p, 1); }

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
  explicit forward_list(const allocator_type& __a = allocator_type())
    : _M_head() {}

  explicit forward_list(size_type __n)
    : forward_list(__n, _Tp()) {}

  forward_list(size_type __n, const _Tp& __value,
    const allocator_type& __a = allocator_type())
    : _M_head()
    {
      _Slist_node_base* __head = (_Slist_node_base*) this;
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

  forward_list(const forward_list& __x) : _M_head()
    { _M_iter_construct(__x.begin(), __x.end()); }

  forward_list(forward_list&& __x) = default;

  forward_list(std::initializer_list<_Tp> __l, 
    const allocator_type& __a = allocator_type())
    : _M_head()
    { _M_iter_construct(__l.begin(), __l.end()); }

  template <InputIterator Iterator>
  forward_list(Iterator __first, Iterator __last,
       const allocator_type& __a = allocator_type())
    : _M_head()
    { _M_iter_construct(__first, __last); }

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
  allocator_type get_allocator() const { return allocator_type(); }

  iterator begin()
    { return iterator(_M_head._M_next); }

  const_iterator begin() const
    { return const_iterator(_M_head._M_next); }

  const_iterator cbegin() const
    { return const_iterator(_M_head._M_next); }

  iterator before_begin()
    { return iterator(this); }

  const_iterator before_begin() const
    { return const_iterator(this); }

  const_iterator cbefore_begin() const
    { return const_iterator(this); }

  iterator end()
    { return iterator(); }

  const_iterator end() const
    { return const_iterator(); }

  const_iterator cend() const
    { return const_iterator(); }

  bool empty() const
    { return _M_head._M_next == 0; }

  void clear()
  {
    _Node* __cur  = (_Node*) _M_head._M_next;
    _Node* __head = (_Node*) &_M_head;

    while (__cur != 0) {
      _Node* __tmp = __cur;
      __cur = (_Node*) __cur->_M_next;
      _M_destroy_node(__tmp);
    }

    _M_head._M_next = 0;
  }

  size_type max_size() const { return size_type(-1); }

  reference front()
    { return *begin(); }

  const_reference front() const
    { return *begin(); }

  void swap(forward_list<_Tp, _Alloc>& __x)
    { tinySTL::swap(_M_head, __x._M_head); }

  iterator insert_after(const_iterator __pos, _Tp&& __val)
    { return emplace_after(__pos, std::move(__val)); }

  iterator insert_after(const_iterator __pos, const _Tp& __val)
    { return emplace(__pos, __val); }

  iterator insert_after(const_iterator __pos, std::initializer_list<_Tp> __il)
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
        _M_transfer(__pre1, __pre2, ++__first2);
        __pre1 = __old;
      } else {
        ++__pre1;
        ++__first1;
      }
    }
    if (__first2 != __last2)
      _M_transfer(__pre1, __pre2, __last2);
  }

  size_type remove(const _Tp& __val);

  template <class _Pred>
  size_type remove_if(_Pred __pred);

  void resize(size_type __sz);

  void resize(size_type __sz, const _Tp& __val);

  void reverse();

  void sort();

  template <class _Comp>
  void sort(_Comp __comp);

  size_type unique();

  template <class _BinPred>
  size_type unique(_BinPred __binary_pred);

  void splice_after(const_iterator __pos, forward_list&& __list);

  void splice_after(const_iterator __pos, forward_list& __list);

  void splice_after(const_iterator __pos, forward_list&& __list,
                    const_iterator __i);

  void splice_after(const_iterator __pos, forward_list& __list,
                    const_iterator __i);

  void splice_after(const_iterator __pos, forward_list&& __list,
                    const_iterator __before, const_iterator __last);

  void splice_after(const_iterator __pos, forward_list& __list,
                    const_iterator __before, const_iterator __last);

};

}