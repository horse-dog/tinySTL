// tinySTL: list.
#pragma once

#include "tiny_alloc.h"
#include "tiny_errors.h"
#include "tiny_iterator.h"
#include "tiny_algobase.h"
#include "tiny_construct.h"

namespace tinySTL
{

struct _List_node_base {
  _List_node_base* _M_prev = 0;
  _List_node_base* _M_next = 0;
};

struct _List_node_head : public _List_node_base
{
  using _List_node_base::_M_prev;
  using _List_node_base::_M_next;
  size_t _M_size = 0;

  _List_node_head() : _M_size(0) 
    { _M_prev = _M_next = this; }

 ~_List_node_head() 
    { 
      _M_prev = _M_next = this;
      _M_size = 0;
    }

  _List_node_head(const _List_node_head&) = delete;

  _List_node_head& operator=(const _List_node_head&) = delete;

  _List_node_head(_List_node_head&& __x)
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

  _List_node_head& operator=(_List_node_head&& __x) 
    {
      if (this != &__x) {
        this->~_List_node_head();
        tinySTL::construct(this, tinySTL::move(__x));
      }
      return *this;
    }
  
};

template <class _Tp>
struct _List_node : public _List_node_base
{
  using _List_node_base::_M_prev;
  using _List_node_base::_M_next;
  aligned_membuf<_Tp> _M_storage;

  template <class... _Args>
  _List_node(_Args&&... __args)
    { tinySTL::construct(_M_storage.ptr(), tinySTL::forward<_Args>(__args)...); }

 ~_List_node() 
    { tinySTL::destroy(_M_storage.ptr()); }
};

template <class _Tp, class _Alloc = alloc>
class list
{

 protected:
  _List_node_head _M_head;

 protected:
  class _List_iterator
  {
   friend class list;
   public:
    using iterator_category = tinySTL::bidirectional_iterator_tag;
    using value_type = _Tp;
    using difference_type = ptrdiff_t;
    using pointer = _Tp*;
    using reference = _Tp&;

   protected:
    _List_node_base* _M_node;

   public:
    constexpr _List_iterator() : _M_node(0) {}

   protected:
    // just invoke by class list inner.
    constexpr _List_iterator(const _List_node_base* __ptr)
      : _M_node((_List_node_base*)__ptr) { }

   public:
    constexpr reference operator*() const
      { return *((_List_node<_Tp>*)_M_node)->_M_storage.ptr(); }

    constexpr pointer operator->() const
      { return ((_List_node<_Tp>*)_M_node)->_M_storage.ptr(); }

    constexpr _List_iterator& operator++() 
      { 
        _M_node = _M_node->_M_next; 
        return *this; 
      }

    constexpr _List_iterator operator++(int) 
      {
        _List_iterator old(this->_M_node);
        ++(*this);
        return old;
      }

    constexpr _List_iterator& operator--() 
      { 
        _M_node = _M_node->_M_prev; 
        return *this; 
      }

    constexpr _List_iterator operator--(int) 
      {
        _List_iterator old(this->_M_node);
        --(*this);
        return old;
      }

    friend constexpr bool operator==(
      const _List_iterator& __x, 
      const _List_iterator& __y) 
      { 
        return __x._M_node == __y._M_node;
      }
  };

 public:
  typedef _Tp value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef _List_iterator iterator;
  typedef tinySTL::const_iterator<iterator> const_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Alloc allocator_type;
  typedef tinySTL::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef tinySTL::reverse_iterator<iterator> reverse_iterator;

 protected:
  typedef simple_alloc<_List_node<_Tp>, _Alloc> _M_node_allocator;
  typedef _List_node<_Tp> _Node;

  struct less 
  {
    bool operator()(const _Tp& a, const _Tp& b) { return a < b; }
  };
  struct equal 
  {
    bool operator()(const _Tp& a, const _Tp& b) { return a == b; }
  };

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

  void _M_range_check(size_type __n) const {
    if (__n >= this->size())
      __tiny_throw_range_error("list");
  }

 public:
  explicit list(const allocator_type& __a = allocator_type())
    : _M_head() {}

  explicit list(size_type __n) 
    : list(__n, _Tp()) {}

  list(size_type __n, const _Tp& __value,
       const allocator_type& __a = allocator_type())
    : _M_head()
    { 
      _List_node_base* __head = (_List_node_base*) this;
      _List_node_base* pre = __head;
      _List_node_base* s;
      _M_head._M_size = __n;
      for (size_type i = 0; i < __n; i++)
      {
        s = (_List_node_base*) _M_create_node(__value);
        pre->_M_next = s;
        s->_M_prev = pre;
        pre = s;
      }
      pre->_M_next = __head;
      __head->_M_prev = pre;
    }

  list(const list& __x) : _M_head()
    { _M_iter_construct(__x.begin(), __x.end()); }

  list(list&& __x) = default;

  list(std::initializer_list<_Tp> __l, 
       const allocator_type& __a = allocator_type())
    : _M_head()
    { _M_iter_construct(__l.begin(), __l.end()); }

  template <InputIterator Iterator>
  list(Iterator __first, Iterator __last,
       const allocator_type& __a = allocator_type())
    : _M_head()
    { _M_iter_construct(__first, __last); }

  ~list() { clear(); }

  list& operator=(std::initializer_list<_Tp> __l)
  {
    assign(__l);
    return *this;
  }

  list& operator=(const list& __x)
  {
    if (this != &__x) 
      assign(__x.begin(), __x.end());
    return *this;
  }

  list& operator=(list&& __x)
  {
    if (this != &__x)
    {
      this->~list();
      tinySTL::construct(this, tinySTL::move(__x));
    }
    return *this;
  }

  void assign(std::initializer_list<_Tp> __l)
    { assign(__l.begin(), __l.end()); }

  void assign(size_type __n, const _Tp& __val)
  {
    iterator __i = begin();
    for ( ; __i != end() && __n > 0; ++__i, --__n)
      *__i = __val;
    if (__n > 0)
      insert(end(), __n, __val);
    else
      erase(__i, end());
  }

  template <InputIterator Iterator>
  void assign(Iterator __first, Iterator __last)
  {
    iterator __first1 = begin();
    iterator __last1 = end();
    for ( ; __first1 != __last1 && __first != __last; ++__first1, ++__first)
      *__first1 = *__first;
    if (__first == __last)
      erase(__first1, __last1);
    else
      insert(__last1, __first, __last);
  }

 public:
  allocator_type get_allocator() const { return allocator_type(); }

  iterator begin() 
    { return iterator(_M_head._M_next); }

  const_iterator begin() const 
    { return const_iterator(_M_head._M_next); }

  const_iterator cbegin() const 
    { return const_iterator(_M_head._M_next); }

  iterator end() 
    { return iterator(&_M_head); }

  const_iterator end() const 
    { return const_iterator(&_M_head); }

  const_iterator cend() const 
    { return const_iterator(&_M_head); }

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

  bool empty() const { return size() == 0; }
  
  size_type size() const { return _M_head._M_size; }

  size_type max_size() const { return size_type(-1); }

  reference front() 
    { _M_range_check(0); return *begin(); }

  const_reference front() const
    { _M_range_check(0); return *begin(); }
  
  reference back()
    { _M_range_check(0); return *(--end()); }

  const_reference back() const
    { _M_range_check(0); return *(--end()); }

  void swap(list<_Tp, _Alloc>& __x) 
    { tinySTL::swap(_M_head, __x._M_head); }

  iterator insert(const_iterator __position, _Tp&& __x) 
    { return emplace(__position, std::move(__x)); }

  iterator insert(const_iterator __position, const _Tp& __x) 
    { return emplace(__position, __x); }

  iterator insert(const_iterator __position, std::initializer_list<_Tp> __l)
    { return insert(__position, __l.begin(), __l.end()); }

  iterator insert(const_iterator __position, size_type __n, const _Tp& __x)
    {
      if (__n == 0) return __position.base();
      list __tmp(__n, __x);
      iterator __it = __tmp.begin();
      splice(__position, __tmp);
      return __it;
    }

  template <InputIterator Iterator>
  iterator insert(const_iterator __position, Iterator __first, Iterator __last)
    {
      list __tmp(__first, __last);
      if (__tmp.empty()) return __position.base();
      iterator __it = __tmp.begin();
      splice(__position, __tmp);
      return __it;
    }
  
  void push_front(_Tp&& __x) 
    { emplace(begin(), tinySTL::forward<_Tp>(__x)); }

  void push_front(const _Tp& __x) { emplace(begin(), __x); }

  void push_back(_Tp&& __x) 
    { emplace(end(), tinySTL::forward<_Tp>(__x)); }

  void push_back(const _Tp& __x) { emplace(end(), __x); }

  iterator erase(const_iterator __pos) 
    {
      iterator __position = __pos.base();
      _List_node_base* __next_node = __position._M_node->_M_next;
      _List_node_base* __prev_node = __position._M_node->_M_prev;
      _Node* __n = (_Node*) __position._M_node;
      __prev_node->_M_next = __next_node;
      __next_node->_M_prev = __prev_node;
      _M_destroy_node(__n);
      _M_head._M_size -= 1;
      return iterator(__next_node);
    }

  iterator erase(const_iterator __first, const_iterator __last)
    {
      if (__first == begin() && __last == end()) 
      {
        clear();
        return end();
      } 
      while (__first != __last)
        erase(__first++);
      return __last.base();
    }

  void clear()
    {
      _Node* __cur  = (_Node*) _M_head._M_next;
      _Node* __head = (_Node*) &_M_head;
      while (__cur != __head) {
        _Node* __tmp = __cur;
        __cur = (_Node*) __cur->_M_next;
        _M_destroy_node(__tmp);
      }
      _M_head._M_next = &_M_head;
      _M_head._M_prev = &_M_head;
      _M_head._M_size = 0;
    }

  void resize(size_type __new_size, const _Tp& __x)
    {
      iterator __i = begin();
      size_type __len = 0;
      for ( ; __i != end() && __len < __new_size; ++__i, ++__len)
        ;
      if (__len == __new_size)
        erase(__i, end());
      else  // __i == end()
        insert(end(), __new_size - __len, __x);
    }

  void resize(size_type __new_size) { this->resize(__new_size, _Tp()); }

  void pop_front() { erase(begin()); }

  void pop_back() 
    { 
      iterator __tmp = end();
      erase(--__tmp);
    }

  // TODO: add comment of splices.
  void splice(const_iterator __position, list&& __x)
    { return splice(__position, __x); }

  void splice(const_iterator __position, list& __x)
    {
      if (!__x.empty()) {
        this->_M_transfer(__position.base(), __x.begin(), __x.end());
        this->_M_head._M_size += __x.size();
        __x._M_head._M_size = 0;
      }
    }

  void splice(const_iterator __position, list&& __x, const_iterator __i)
    { return splice(__position, __x, __i); }

  void splice(const_iterator __position, list& __x, const_iterator __i)
    {
      const_iterator __j = __i;
      ++__j;
      if (__position == __i || __position == __j) return;
      this->_M_transfer(__position.base(), __i.base(), __j.base());
      this->_M_head._M_size += 1;
      __x._M_head._M_size -= 1;
    }

  void splice(const_iterator __position, list&& __x, const_iterator __first,
              const_iterator __last)
    { return splice(__position, __x, __first, __last); }

  void splice(const_iterator __position, list& __x, const_iterator __first,
              const_iterator __last)
    {
      if (__first == __x.begin() && __last == __x.end()) 
      {
        splice(__position, __x);
        return;
      }
      size_type __n = 0;
      iterator __end = __x.end();
      for (auto it = __first; it != __end && it != __last; ++it, ++__n)
        ;
      this->_M_transfer(__position.base(), __first.base(), __last.base());
      this->_M_head._M_size += __n;
      __x._M_head._M_size -= __n;
    }

  size_type remove(const _Tp& __value)
    { 
      size_type __old_len = size();
      iterator __extra = end();
      for (auto it = begin(); it != end();)
        if (*it == __value)
          if (tinySTL::addressof(*it) != tinySTL::addressof(__value))
            it = erase(it);
          else
            __extra = it++;
        else
          ++it;
      if (__extra != end())
        erase(__extra);
      return __old_len - size();
    }

  template <class _Predicate>
  size_type remove_if(_Predicate __pred)
    { 
      size_type __old_len = size();
      for (auto it = begin(); it != end();)
        __pred(*it) ? (it = erase(it)) : (++it);
      return __old_len - size();
    }

  /**
   * @brief remove all consecutive repeating elements from the list.
   */
  size_type unique()
    { return unique(equal()); }

  /**
   * @brief remove all consecutive repeating elements from the list.
   * @param __pred expression
   */
  template <class _BinaryPredicate>
  size_type unique(_BinaryPredicate __pred)
    { 
      size_type __old_len = size();
      for (iterator pre = begin(), p = ++begin(); p != end();)
        __pred(*pre, *p) ? (p = erase(p)) : (pre = p, ++p);
      return __old_len - size();
    }

  /**
   * @brief merge this list with another list, these two lists
   * should be ordered before use this function.
   * @param __x another list
   */
  void merge(list& __x)
    { merge(__x, less()); }

  /**
   * @brief merge this list with another list, these two lists
   * should be ordered before use this function.
   * @param __x another list
   */
  void merge(list&& __x)
    { merge(__x, less()); }

  /**
   * @brief merge this list with another list, these two lists
   * should be ordered before use this function.
   * @param __x another list
   * @param __comp sort method
   */
  template <class _StrictWeakOrdering>
  void merge(list& __x, _StrictWeakOrdering __comp) 
    { 
      iterator __first1 = begin(), __last1 = end();    
      iterator __first2 = __x.begin(), __last2 = __x.end();
      while (__first1 != __last1 && __first2 != __last2) 
      { 
        if (__comp(*__first2, *__first1)) 
        { 
          iterator __old = __first2;
          _M_transfer(__first1, __old, ++__first2);
        } 
        else
          ++__first1;
      }
      if (__first2 != __last2)
        _M_transfer(__last1, __first2, __last2);
      _M_head._M_size += __x._M_head._M_size;
      __x._M_head._M_size = 0;
    }

  /**
   * @brief merge this list with another list, these two lists \n
   * should be ordered before use this function
   * @param __x another list
   * @param __comp sort method
   */
  template <class _StrictWeakOrdering>
  void merge(list&& __x, _StrictWeakOrdering __comp)
    { merge(__x, __comp); }

  /**
   * @brief reverse list. 
   */
  void reverse()
    { 
      _List_node_base* __head = (_List_node_base*)(this);
      _List_node_base* __cur = __head;
      while (true) 
      { 
        _List_node_base* __next = __cur->_M_next;
        __cur->_M_next = __cur->_M_prev;
        __cur->_M_prev = __next;
        if (__next == __head) break;
        __cur = __next;
      }
    }

  void sort() { sort(less()); }

  // TODO: why it work?.
  template <class _StrictWeakOrdering>
  void sort(_StrictWeakOrdering __comp)
  { 
    // Do nothing if the list has length 0 or 1.
    if (size() < 2) return;

    list __carry;
    list __tmp[64];
    list* __fill = __tmp;
    list* __counter;
    try { 
      do { 
        // move front element to __carry.
        __carry.splice(__carry.begin(), *this, begin());

        for(__counter = __tmp;
            __counter != __fill && !__counter->empty(); 
            ++__counter) 
          { 
            __counter->merge(__carry, __comp);
            __carry.swap(*__counter);
          }
        
        __carry.swap(*__counter);
        if (__counter == __fill)
          ++__fill;
      } while ( !empty() );

      for (__counter = __tmp + 1; __counter != __fill; ++__counter)
        __counter->merge(*(__counter - 1), __comp);
      
      swap(*(__fill - 1));
    } catch(...) { 
      this->splice(this->end(), __carry);
      for (int __i = 0; __i < sizeof(__tmp)/sizeof(__tmp[0]); ++__i)
        this->splice(this->end(), __tmp[__i]);
      throw;
    }
  }

  template <class... _Args>
  iterator emplace(const_iterator __pos, _Args &&...__args) 
  {
    iterator __position = __pos.base();
    _Node* __tmp = _M_create_node(tinySTL::forward<_Args>(__args)...);
    __tmp->_M_next = __position._M_node;
    __tmp->_M_prev = __position._M_node->_M_prev;
    __position._M_node->_M_prev->_M_next = __tmp;
    __position._M_node->_M_prev = __tmp;
    ++_M_head._M_size;
    return __tmp;
  }

  template <class... _Args>
  reference emplace_back(_Args &&...__args) 
  {
    return *emplace(end(), tinySTL::forward<_Args>(__args)...);
  }

  template <class... _Args>
  reference emplace_front(_Args &&...__args) 
  {
    return *emplace(begin(), tinySTL::forward<_Args>(__args)...);
  }

 protected:
  void _M_transfer(iterator __position, iterator __first, iterator __last)
  {
    if (__position != __last) {
      // Remove [first, last) from its old position.
      __last._M_node->_M_prev->_M_next     = __position._M_node;
      __first._M_node->_M_prev->_M_next    = __last._M_node;
      __position._M_node->_M_prev->_M_next = __first._M_node;

      // Splice [first, last) into its new position.
      _List_node_base* __tmp      = __position._M_node->_M_prev;
      __position._M_node->_M_prev = __last._M_node->_M_prev;
      __last._M_node->_M_prev     = __first._M_node->_M_prev; 
      __first._M_node->_M_prev    = __tmp;
    }
  }

  template <InputIterator Iterator>
  void _M_iter_construct(Iterator __first, Iterator __last) 
  {
    _List_node_base* head = (_List_node_base*) this;
    _List_node_base* pre = head, *s;
    size_type __n = 0;
    for (auto it = __first; it != __last; ++it, ++__n) {
      s = (_List_node_base*) _M_create_node(*it);
      pre->_M_next = s;
      s->_M_prev = pre;
      pre = s;
    }
    pre->_M_next = head;
    head->_M_prev = pre;
    _M_head._M_size = __n;
  }

};

}