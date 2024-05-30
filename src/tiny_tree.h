#pragma once

#include "tiny_pair.h"
#include "tiny_alloc.h"
#include "tiny_algobase.h"
#include "tiny_iterator.h"
#include "tiny_construct.h"

namespace tinySTL
{

enum class _Rb_tree_color { _S_red = false, _S_black = true };

struct _Rb_tree_node_base 
{
  typedef _Rb_tree_node_base* _Base_ptr;
  typedef _Rb_tree_node_base const* _Const_Base_ptr;

  _Rb_tree_color _M_color;
  _Base_ptr _M_parent;
  _Base_ptr _M_left;
  _Base_ptr _M_right;

  static _Base_ptr
  _S_minimum(_Base_ptr __x) noexcept
  {
    while (__x->_M_left != 0) __x = __x->_M_left;
    return __x;
  }

  static _Const_Base_ptr
  _S_minimum(_Const_Base_ptr __x) noexcept
  {
    while (__x->_M_left != 0) __x = __x->_M_left;
    return __x;
  }

  static _Base_ptr
  _S_maximum(_Base_ptr __x) noexcept
  {
    while (__x->_M_right != 0) __x = __x->_M_right;
    return __x;
  }

  static _Const_Base_ptr
  _S_maximum(_Const_Base_ptr __x) noexcept
  {
    while (__x->_M_right != 0) __x = __x->_M_right;
    return __x;
  }

  static _Base_ptr _Rb_tree_increment(_Base_ptr __x)
  {
    if (__x->_M_color == _Rb_tree_color::_S_red
     && __x->_M_parent->_M_parent == __x)
    {
      /**
       * There are two nodes that satisfy the condition 
       * __x-> _M_parent-> _M_parent == __x, one is the 
       * head node and the another is the root node. but
       * the head node is red while the root node is black.
       * 
       * so this if statement check __x is head or not.
       * if __x is head, then the next is __x->_M_left.
       */
      __x = __x->_M_left;
    }
    else if (__x->_M_right != 0)
    {
      __x = _S_minimum(__x->_M_right);
    }
    else 
    {
      _Base_ptr __y = __x->_M_parent;
      while (__x == __y->_M_right) {
        __x = __y;
        __y = __y->_M_parent;
      }

      /**
       * when the tree only contains head node and root
       * node, and __x is root node, then this if state
       * -ment will be __x->_M_right == __y. 
       */
      if (__x->_M_right != __y)
        __x = __y;
    }
    return __x;
  }

  static _Const_Base_ptr _Rb_tree_increment(_Const_Base_ptr __x)
  {
    return _Const_Base_ptr(_Rb_tree_increment(_Base_ptr(__x)));
  }

  static _Base_ptr _Rb_tree_decrement(_Base_ptr __x) 
  { // Symmetric functions of _Rb_tree_increment.
    if (__x->_M_color == _Rb_tree_color::_S_red
     && __x->_M_parent->_M_parent == __x)
    {
      __x = __x->_M_right;
    }
    else if (__x->_M_left != 0) 
    {
      __x = _S_maximum(__x->_M_left);
    }
    else 
    {
      _Base_ptr __y = __x->_M_parent;
      while (__x == __y->_M_left) {
        __x = __y;
        __y = __y->_M_parent;
      }

      if (__x->_M_left != __y)
        __x = __y;
    }
    return __x;
  }

  static _Const_Base_ptr _Rb_tree_decrement(_Const_Base_ptr __x) 
  {
    return _Const_Base_ptr(_Rb_tree_decrement(_Base_ptr(__x)));
  }
};


template <class _Val>
struct _Rb_tree_node : public _Rb_tree_node_base
{
  typedef _Rb_tree_node<_Val>* _Link_type;
  typedef const _Rb_tree_node<_Val>* _Const_Link_type;
  typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
  typedef _Rb_tree_node_base::_Const_Base_ptr _Const_Base_ptr;

  aligned_membuf<_Val> _M_storage;

  _Val* _M_valptr() { return _M_storage.ptr(); }

  const _Val* _M_valptr() const 
  { return _M_storage.ptr(); }

  _Link_type& _M_brother()
  {
    if (this == _M_parent->_M_left)
      return (_Link_type&)_M_parent->_M_right;
    else 
      return (_Link_type&)_M_parent->_M_left;
  }

  _Link_type _M_brother() const
  {
    if (this == _M_parent->_M_left)
      return _Link_type(_M_parent->_M_right);
    else 
      return _Link_type(_M_parent->_M_left);    
  }

  _Link_type& _M_gparent() 
  { 
    return (_Link_type&)_M_parent->_M_parent; 
  }
  
  _Link_type _M_gparent() const 
  {
    return _Link_type(_M_parent->_M_parent);
  }

  _Link_type& _M_uncle() 
  {
    _Base_ptr __gparent = _M_parent->_M_parent;
    if (_M_parent == __gparent->_M_left)
      return (_Link_type&)__gparent->_M_right;
    else 
      return (_Link_type&)__gparent->_M_left;
  }

  _Link_type _M_uncle() const 
  {
    _Base_ptr __gparent = _M_parent->_M_parent;
    if (_M_parent == __gparent->_M_left)
      return _Link_type(__gparent->_M_right);
    else 
      return _Link_type(__gparent->_M_left);
  }

  bool _M_isRed() const 
  { return _M_color == _Rb_tree_color::_S_red; }

  bool _M_isBlk() const 
  { return _M_color == _Rb_tree_color::_S_black; }

  void _M_setRed() 
  { _M_color = _Rb_tree_color::_S_red; }

  void _M_setBlk()
  { _M_color = _Rb_tree_color::_S_black; }

};

template <class _Tp>
struct _Rb_tree_iterator
{
  typedef _Tp  value_type;
  typedef _Tp& reference;
  typedef _Tp* pointer;

  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t       difference_type;

  typedef _Rb_tree_iterator<_Tp>    _Self;
  typedef _Rb_tree_node_base::_Base_ptr  _Base_ptr;
  typedef _Rb_tree_node<_Tp>*    _Link_type;

  constexpr
  _Rb_tree_iterator(_Base_ptr __x) noexcept
  : _M_node(__x) { }

  _Rb_tree_iterator() noexcept
  : _M_node() { }

  reference
  operator*() const noexcept
  { return *_Link_type(_M_node)->_M_valptr(); }

  pointer 
  operator->() const noexcept
  { return _Link_type(_M_node)->_M_valptr(); }

  _Self& operator++() noexcept
  {
    _M_node = _Rb_tree_node_base::_Rb_tree_increment(_M_node);
    return *this;
  }

  _Self operator++(int) noexcept
  {
    _Self __tmp = *this;
    _M_node = _Rb_tree_node_base::_Rb_tree_increment(_M_node);
    return __tmp;
  }

  _Self& operator--() noexcept
  {
    _M_node = _Rb_tree_node_base::_Rb_tree_decrement(_M_node);
    return *this;
  }

  _Self operator--(int) noexcept
  {
    _Self __tmp = *this;
    _M_node = _Rb_tree_node_base::_Rb_tree_decrement(_M_node);
    return __tmp;    
  }

  friend bool
  operator==(const _Self& __x, const _Self& __y) noexcept
  { return __x._M_node == __y._M_node; }

  friend bool
  operator!=(const _Self& __x, const _Self& __y) noexcept
  { return __x._M_node != __y._M_node; }

  _Base_ptr _M_node;
};

template<typename _Key, typename _Val, typename _KeyOfValue,
     typename _Compare, typename _Alloc = tinySTL::allocator<_Val> >
class _Rb_tree 
{

 protected: 
  typedef typename _Alloc_rebind<_Alloc, _Rb_tree_node<_Val>>
            ::type _Node_allocator;
  typedef typename _Alloc_rebind<_Alloc, _Rb_tree_node_base>
            ::type _Head_allocator;
  typedef _Rb_tree_node_base* _Base_ptr;
  typedef const _Rb_tree_node_base* _Const_Base_ptr;
  typedef _Rb_tree_node<_Val>* _Link_type;
  typedef const _Rb_tree_node<_Val>* _Const_Link_type;

 public:
  typedef _Key key_type;
  typedef _Val value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Alloc allocator_type;
  typedef _Rb_tree_iterator<value_type> iterator;
  typedef tinySTL::const_iterator<iterator> const_iterator;
  typedef tinySTL::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef tinySTL::reverse_iterator<iterator> reverse_iterator;

 protected:
  _Rb_tree_node_base* _M_header;
  size_t _M_node_count;
  _Node_allocator _M_node_allocator;
  _Head_allocator _M_head_allocator;
  _Compare _M_compare;
  
  _Link_type _M_get_node()
  { return _M_node_allocator.allocate(1); }

  void _M_put_node(_Link_type __p)
  { return _M_node_allocator.deallocate(__p, 1); }

  /**
   *  If the red-black tree is empty, we set the parent node
   *  of the head node to nil, the left child of the head 
   *  node to the head node itself, and the right child to 
   *  the head node itself.
   */
  void _M_reset()
  {
    if (_M_header != nullptr) {
      _M_header->_M_parent = 0;
      _M_header->_M_left = _M_header;
      _M_header->_M_right = _M_header;
      _M_header->_M_color = _Rb_tree_color::_S_red;
    }
    _M_node_count = 0;
  }
  
  void _M_copy(const _Rb_tree& __x)
  {
    if (__x._M_header->_M_parent != nullptr) {
      if (_M_header == nullptr) {
        _M_header = _M_head_allocator.allocate(1);
      }
      _Link_type __tmp = _M_copy((_Link_type)__x._M_header->_M_parent, (_Link_type)_M_header);
      _M_header->_M_parent = __tmp;
      _M_header->_M_left = _Rb_tree::_S_minimum(__tmp);
      _M_header->_M_right = _Rb_tree::_S_maximum(__tmp);
      _M_node_count = __x._M_node_count;
    }
  }

  _Link_type _M_copy(_Link_type __x, _Link_type __p)
  {
    _Link_type __top = _M_clone_node(__x);
    __top->_M_parent = __p;

    try {
      if (__x->_M_right)
        __top->_M_right = _M_copy(_S_right(__x), __top);
      __p = __top;
      __x = _S_left(__x);
      while (__x != 0) {
        _Link_type __y = _M_clone_node(__x);
        __p->_M_left = __y;
        __y->_M_parent = __p;
        if (__x->_M_right)
          __y->_M_right = _M_copy(_S_right(__x), __y);
        __p = __y;
        __x = _S_left(__x);
      }
    } catch (...) {
      _M_erase(__top);
    }
    return __top;
  }

  _Link_type _M_clone_node(_Link_type __x)
  {
    _Link_type __tmp = _M_create_node(*__x->_M_valptr());
    __tmp->_M_color = __x->_M_color;
    __tmp->_M_left = 0;
    __tmp->_M_right = 0;
    return __tmp;
  }

  template <class... _Args>
  void _M_construct_node(_Link_type __node, _Args&&... __args)
  {
    try {
      tinySTL::construct(__node->_M_valptr(),
        tinySTL::forward<_Args>(__args)...);
    } catch (...) {
      _M_put_node(__node);
      throw;
    }
  }

  void _M_destroy_node(_Link_type __p) noexcept
  {
    tinySTL::destroy(__p->_M_valptr());
  }

  template <class... _Args>
  _Link_type _M_create_node(_Args&&... __args)
  {
    _Link_type __tmp = _M_get_node();
    _M_construct_node(__tmp, tinySTL::forward<_Args>(__args)...);
    return __tmp;
  }

  void _M_drop_node(_Link_type __p) noexcept
  {
    _M_destroy_node(__p);
    _M_put_node(__p);
  }

  _Base_ptr& _M_root() noexcept
  { return _M_header->_M_parent; }

  _Const_Base_ptr _M_root() const noexcept
  { return _M_header->_M_parent; }

  _Base_ptr& _M_leftmost() noexcept
  { return _M_header->_M_left; }

  _Const_Base_ptr _M_leftmost() const noexcept
  { return _M_header->_M_left; }

  _Base_ptr& _M_rightmost() noexcept
  { return _M_header->_M_right; }

  _Const_Base_ptr _M_rightmost() const noexcept
  { return _M_header->_M_right; }

  _Base_ptr _M_head() noexcept 
  { return _M_header; }

  _Const_Base_ptr _M_head() const noexcept 
  { return _M_header; }

  static const_reference
  _S_value(_Const_Link_type __x)
  { return *__x->_M_valptr(); }

  static const _Key&
  _S_key(_Const_Link_type __x)
  { return _KeyOfValue()(*__x->_M_valptr()); }

  static _Link_type _S_parent(_Base_ptr __x) noexcept 
  { return _Link_type(__x->_M_parent); }

  static _Const_Link_type _S_parent(_Const_Base_ptr __x) noexcept 
  { return _Const_Link_type(__x->_M_parent); }

  static _Link_type _S_left(_Base_ptr __x) noexcept
  { return _Link_type(__x->_M_left); }

  static _Const_Link_type _S_left(_Const_Base_ptr __x) noexcept
  { return _Const_Link_type(__x->_M_left); }

  static _Link_type _S_right(_Base_ptr __x) noexcept
  { return _Link_type(__x->_M_right); }

  static _Const_Link_type _S_right(_Const_Base_ptr __x) noexcept
  { return _Const_Link_type(__x->_M_right); }

  static const_reference _S_value(_Const_Base_ptr __x)
  { return *_Const_Link_type(__x)->_M_valptr(); }

  static const _Key& _S_key(_Const_Base_ptr __x)
  { return _S_key(_Const_Link_type(__x)); }

  static _Base_ptr _S_minimum(_Base_ptr __x) noexcept
  { return _Rb_tree_node_base::_S_minimum(__x); }

  static _Const_Base_ptr
  _S_minimum(_Const_Base_ptr __x) noexcept
  { return _Rb_tree_node_base::_S_minimum(__x); }

  static _Base_ptr _S_maximum(_Base_ptr __x) noexcept
  { return _Rb_tree_node_base::_S_maximum(__x); }

  static _Const_Base_ptr
  _S_maximum(_Const_Base_ptr __x) noexcept
  { return _Rb_tree_node_base::_S_maximum(__x); }

 public:
  _Rb_tree() 
  : _M_header(0), _M_node_count(0), 
    _M_node_allocator(), _M_head_allocator(), _M_compare() 
  { }

  _Rb_tree(const _Compare& __comp, 
     const allocator_type& __a = allocator_type())
  : _M_header(0), _M_node_count(0), 
    _M_node_allocator(__a), _M_head_allocator(), _M_compare(__comp)
  { }

  _Rb_tree(const _Rb_tree& __x)
  : _M_node_allocator(__x._M_node_allocator), 
    _M_head_allocator(__x._M_head_allocator), 
    _M_compare(__x._M_compare) 
  { 
    _M_header = nullptr;
    _M_node_count = 0;
    if (!__x.empty()) {
      _M_copy(__x);
    } 
  }

  _Rb_tree(_Rb_tree&& __x) 
  : _M_node_allocator(tinySTL::move(__x._M_node_allocator)), 
    _M_head_allocator(tinySTL::move(__x._M_head_allocator)), 
    _M_compare(tinySTL::move(__x._M_compare)) 
  { 
    _M_header = __x._M_header;
    _M_node_count = __x._M_node_count;
    __x._M_header = 0;
    __x._M_node_count = 0;
  }

  ~_Rb_tree() 
  { 
    clear();
    if (_M_header != nullptr) {
      _M_head_allocator.deallocate(_M_header);
    }
  }

  _Rb_tree& operator=(const _Rb_tree& __x)
  {
    if (this != &__x) {
      clear();
      if (!__x.empty()) {
        _M_copy(__x);
      } else {
        _M_reset();
      }
    }
    return *this;
  }

  _Rb_tree& operator=(_Rb_tree&& __x) 
  {
    if (this != &__x) {
      clear();
      if (_M_header != nullptr) {
        _M_head_allocator.deallocate(_M_header);
      }
      _M_node_allocator = tinySTL::move(_M_node_allocator);
      _M_head_allocator = tinySTL::move(_M_head_allocator);
      _M_compare = tinySTL::move(_M_compare);
      _M_header = __x._M_header;
      _M_node_count = __x._M_node_count;
      __x._M_header = 0;
      __x._M_node_count = 0;
    }
    return *this;
  }

 public:
  _Compare key_comp() const 
  { return _M_compare; }

  iterator begin() noexcept
  { return iterator(_M_header ? _M_header->_M_left : 0); }

  const_iterator begin() const noexcept
  { return const_iterator(_M_header ? _M_header->_M_left : 0); }

  iterator end() noexcept 
  { return iterator(_M_header); }

  const_iterator end() const noexcept 
  { return const_iterator(_Base_ptr(_M_header)); }

  reverse_iterator rbegin() noexcept 
  { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const noexcept
  { return const_reverse_iterator(end()); }

  reverse_iterator rend() noexcept
  { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const noexcept 
  { return const_reverse_iterator(begin()); }

  bool empty() const noexcept 
  { return _M_node_count == 0; }

  size_type size() const noexcept 
  { return _M_node_count; }

  size_type max_size() const noexcept 
  { return size_type(-1); }

  void swap(_Rb_tree& __t) 
  {
    tinySTL::swap(_M_header, __t._M_header);
    tinySTL::swap(_M_node_count, __t._M_node_count);
    tinySTL::swap(_M_node_allocator, __t._M_node_allocator);
    tinySTL::swap(_M_head_allocator, __t._M_head_allocator);
    tinySTL::swap(_M_compare, __t._M_compare);
  }

  void clear() 
  {
    if (_M_header != nullptr) {
      _M_erase(_Link_type(_M_root()));
    }
    _M_reset();
  }

  template <class _Arg>
  tinySTL::pair<iterator, bool>
  _M_insert_unique(_Arg&& __x) 
  {
    if (_M_header == nullptr) {
      _M_header = _M_head_allocator.allocate(1);
      _M_reset();
    }
    _Base_ptr pos = _M_root();
    _Base_ptr pos_parent = _M_head();
    bool __comp = true;
    while (pos != nullptr) {
      pos_parent = pos;
      __comp = key_comp()(_KeyOfValue()(__x), _S_key(pos));
      pos = __comp ? pos->_M_left : pos->_M_right;
    }
    iterator j = iterator(pos_parent);
    if (__comp) {
      if (j == begin()) {
        return {_M_insert(0, pos_parent, tinySTL::forward<_Arg>(__x)), true};
      } else {
        --j;
      }
    }
    if (key_comp()(_S_key(j._M_node), _KeyOfValue()(__x))) {
      return {_M_insert(0, pos_parent, tinySTL::forward<_Arg>(__x)), true};
    }
    return {j, false};
  }

  tinySTL::pair<iterator, bool>
  _M_insert_node_unique(_Link_type __x) 
  {
    if (_M_header == nullptr) {
      _M_header = _M_head_allocator.allocate(1);
      _M_reset();
    }
    _Base_ptr pos = _M_root();
    _Base_ptr pos_parent = _M_head();
    bool __comp = true;
    while (pos != nullptr) {
      pos_parent = pos;
      __comp = key_comp()(_S_key(__x), _S_key(pos));
      pos = __comp ? pos->_M_left : pos->_M_right;
    }
    iterator j = iterator(pos_parent);
    if (__comp) {
      if (j == begin()) {
        return {_M_insert_node(0, pos_parent, __x), true};
      } else {
        --j;
      }
    }
    if (key_comp()(_S_key(j._M_node), _S_key(__x))) {
      return {_M_insert_node(0, pos_parent, __x), true};
    }
    return {j, false};
  }

  template <class _Arg> iterator
  _M_insert_equal(_Arg&& __x) 
  {
    if (_M_header == nullptr) {
      _M_header = _M_head_allocator.allocate(1);
      _M_reset();
    }
    _Base_ptr pos = _M_root();
    _Base_ptr pos_parent = _M_head();
    while (pos != nullptr) {
      pos_parent = pos;
      if (key_comp()(_KeyOfValue()(__x), _S_key(pos))) {
        pos = pos->_M_left;
      } else {
        pos = pos->_M_right;
      }
    }
    return _M_insert(0, pos_parent, tinySTL::forward<_Arg>(__x));
  }

  iterator _M_insert_node_equal(_Link_type __x) 
  {
    if (_M_header == nullptr) {
      _M_header = _M_head_allocator.allocate(1);
      _M_reset();
    }
    _Base_ptr pos = _M_root();
    _Base_ptr pos_parent = _M_head();
    while (pos != nullptr) {
      pos_parent = pos;
      if (key_comp()(_S_key(__x), _S_key(pos))) {
        pos = pos->_M_left;
      } else {
        pos = pos->_M_right;
      }
    }
    return _M_insert_node(0, pos_parent, __x);
  }

  iterator _M_insert_unique(iterator __position, const value_type& __x) 
  {
    if (_M_header == nullptr) {
      _M_header = _M_head_allocator.allocate(1);
      _M_reset();
      if (__position._M_node == nullptr) {
        __position = begin();
      }
    }
    if (__position._M_node == _M_header->_M_left) { // begin()
      if (size() > 0 && key_comp()(_KeyOfValue()(__x), _S_key(__position._M_node))) {
        return _M_insert(__position._M_node, __position._M_node, __x);
      } else {
        return _M_insert_unique(__x).first;
      }
    } else if (__position._M_node == _M_header) {
      if (key_comp()(_S_key(_M_rightmost()), _KeyOfValue()(__x))) {
        return _M_insert(0, _M_rightmost(), __x);
      } else {
        return _M_insert_unique(__x).first;
      }
    } else {
      iterator __before = __position;
      --__before;
      if (key_comp()(_S_key(__before._M_node), _KeyOfValue()(__x))
          && key_comp()(_KeyOfValue()(__x), _S_key(__position._M_node))) {
        if (__before._M_node->_M_right == 0) {
          return _M_insert(0, __before._M_node, __x);
        } else {
          return _M_insert(__position._M_node, __position._M_node, __x);
        }
      } else {
        return _M_insert_unique(__x).first;
      }
    }
  }

  iterator _M_insert_unique(iterator __position, value_type&& __x) 
  {
    if (_M_header == nullptr) {
      _M_header = _M_head_allocator.allocate(1);
      _M_reset();
      if (__position._M_node == nullptr) {
        __position = begin();
      }
    }
    if (__position._M_node == _M_header->_M_left) { // begin()
      if (size() > 0 && key_comp()(_KeyOfValue()(__x), _S_key(__position._M_node))) {
        return _M_insert(__position._M_node, __position._M_node, tinySTL::move(__x));
      } else {
        return _M_insert_unique(tinySTL::move(__x)).first;
      }
    } else if (__position._M_node == _M_header) {
      if (key_comp()(_S_key(_M_rightmost()), _KeyOfValue()(__x))) {
        return _M_insert(0, _M_rightmost(), tinySTL::move(__x));
      } else {
        return _M_insert_unique(tinySTL::move(__x)).first;
      }
    } else {
      iterator __before = __position;
      --__before;
      if (key_comp()(_S_key(__before._M_node), _KeyOfValue()(__x))
          && key_comp()(_KeyOfValue()(__x), _S_key(__position._M_node))) {
        if (__before._M_node->_M_right == 0) {
          return _M_insert(0, __before._M_node, tinySTL::move(__x));
        } else {
          return _M_insert(__position._M_node, __position._M_node, tinySTL::move(__x));
        }
      } else {
        return _M_insert_unique(tinySTL::move(__x)).first;
      }
    }
  }

  tinySTL::pair<iterator, bool>
  _M_insert_unique(iterator __position, _Link_type __x) 
  {
    if (_M_header == nullptr) {
      _M_header = _M_head_allocator.allocate(1);
      _M_reset();
      if (__position._M_node == nullptr) {
        __position = begin();
      }
    }
    if (__position._M_node == _M_header->_M_left) { // begin()
      if (size() > 0 && key_comp()(_S_key(__x), _S_key(__position._M_node))) {
        return {_M_insert_node(__position._M_node, __position._M_node, __x), true};
      } else {
        return _M_insert_node_unique(__x);
      }
    } else if (__position._M_node == _M_header) {
      if (key_comp()(_S_key(_M_rightmost()), _S_key(__x))) {
        return {_M_insert_node(0, _M_rightmost(), __x), true};
      } else {
        return _M_insert_node_unique(__x);
      }
    } else {
      iterator __before = __position;
      --__before;
      if (key_comp()(_S_key(__before._M_node), _S_key(__x))
          && key_comp()(_S_key(__x), _S_key(__position._M_node))) {
        if (__before._M_node->_M_right == 0) {
          return {_M_insert_node(0, __before._M_node, __x), true};
        } else {
          return {_M_insert_node(__position._M_node, __position._M_node, __x), true};
        }
      } else {
        return _M_insert_node_unique(__x);
      }
    }
  }

  iterator _M_insert_equal(iterator __position, const value_type& __x) 
  {
    if (_M_header == nullptr) {
      _M_header = _M_head_allocator.allocate(1);
      _M_reset();
      if (__position._M_node == nullptr) {
        __position = begin();
      }
    }
    if (__position._M_node == _M_header->_M_left) { // begin()
      if (size() > 0 && !key_comp()(_S_key(__position._M_node), _KeyOfValue()(__x))) {
        return _M_insert(__position._M_node, __position._M_node, __x);
      } else {
        return _M_insert_equal(__x);
      }
    } else if (__position._M_node == _M_header) {
      if (!key_comp()(_KeyOfValue()(__x), _S_key(_M_rightmost()))) {
        return _M_insert(0, _M_rightmost(), __x);
      } else {
        return _M_insert_equal(__x);
      }
    } else {
      iterator __before = __position;
      --__before;
      if (!key_comp()(_KeyOfValue()(__x), _S_key(__before._M_node))
          && !key_comp()(_S_key(__position._M_node), _KeyOfValue()(__x))) {
        if (__before._M_node->_M_right == 0) {
          return _M_insert(0, __before._M_node, __x);
        } else {
          return _M_insert(__position._M_node, __position._M_node, __x);
        }
      } else {
        return _M_insert_equal(__x);
      }
    }
  }

  iterator _M_insert_equal(iterator __position, value_type&& __x) 
  {
    if (_M_header == nullptr) {
      _M_header = _M_head_allocator.allocate(1);
      _M_reset();
      if (__position._M_node == nullptr) {
        __position = begin();
      }
    }
    if (__position._M_node == _M_header->_M_left) { // begin()
      if (size() > 0 && !key_comp()(_S_key(__position._M_node), _KeyOfValue()(__x))) {
        return _M_insert(__position._M_node, __position._M_node, tinySTL::move(__x));
      } else {
        return _M_insert_equal(tinySTL::move(__x));
      }
    } else if (__position._M_node == _M_header) {
      if (!key_comp()(_KeyOfValue()(__x), _S_key(_M_rightmost()))) {
        return _M_insert(0, _M_rightmost(), tinySTL::move(__x));
      } else {
        return _M_insert_equal(tinySTL::move(__x));
      }
    } else {
      iterator __before = __position;
      --__before;
      if (!key_comp()(_KeyOfValue()(__x), _S_key(__before._M_node))
          && !key_comp()(_S_key(__position._M_node), _KeyOfValue()(__x))) {
        if (__before._M_node->_M_right == 0) {
          return _M_insert(0, __before._M_node, tinySTL::move(__x));
        } else {
          return _M_insert(__position._M_node, __position._M_node, tinySTL::move(__x));
        }
      } else {
        return _M_insert_equal(tinySTL::move(__x));
      }
    }
  }

  iterator _M_insert_equal(iterator __position, _Link_type __x) 
  {
    if (_M_header == nullptr) {
      _M_header = _M_head_allocator.allocate(1);
      _M_reset();
      if (__position._M_node == nullptr) {
        __position = begin();
      }
    }
    if (__position._M_node == _M_header->_M_left) { // begin()
      if (size() > 0 && !key_comp()(_S_key(__position._M_node), _S_key(__x))) {
        return _M_insert_node(__position._M_node, __position._M_node, __x);
      } else {
        return _M_insert_node_equal(__x);
      }
    } else if (__position._M_node == _M_header) {
      if (!key_comp()(_S_key(__x), _S_key(_M_rightmost()))) {
        return _M_insert_node(0, _M_rightmost(), __x);
      } else {
        return _M_insert_node_equal(__x);
      }
    } else {
      iterator __before = __position;
      --__before;
      if (!key_comp()(_S_key(__x), _S_key(__before._M_node))
          && !key_comp()(_S_key(__position._M_node), _S_key(__x))) {
        if (__before._M_node->_M_right == 0) {
          return _M_insert_node(0, __before._M_node, __x);
        } else {
          return _M_insert_node(__position._M_node, __position._M_node, __x);
        }
      } else {
        return _M_insert_node_equal(__x);
      }
    }
  }
  
  /// @p __hint
  ///  if not null: new_node will be insert at left of __pos_parent at first.
  ///  if null: decide by compare.
  template <class... _Args> iterator
  _M_insert(_Base_ptr __hint, _Base_ptr __pos_parent, _Args&&... __args)
  {
    _Link_type new_node = _M_create_node(tinySTL::forward<_Args>(__args)...);
    return _M_insert_node(__hint, __pos_parent, new_node);
  }

  /// @p __hint
  ///  if not null: new_node will be insert at left of __pos_parent at first.
  ///  if null: decide by compare.
  iterator
  _M_insert_node(_Base_ptr __hint, _Base_ptr __pos_parent, _Link_type new_node)
  {
    if (__pos_parent == _M_head()) {
      _M_leftmost() = _M_rightmost() = _M_root() = new_node;
      new_node->_M_setBlk();
      new_node->_M_parent = __pos_parent;
      new_node->_M_left = new_node->_M_right = 0;
      ++_M_node_count;
      return iterator(new_node);
    } 

    if (__hint || key_comp()(_S_key(new_node), _S_key(__pos_parent))) {
      __pos_parent->_M_left = new_node;
      if (__pos_parent == _M_leftmost()) {
        _M_leftmost() = new_node;
      } 
    } else {
      __pos_parent->_M_right = new_node;
      if (__pos_parent == _M_rightmost()) {
        _M_rightmost() = new_node;
      }
    }
    new_node->_M_parent = __pos_parent;
    new_node->_M_left = new_node->_M_right = 0;
    _M_pre_fix_insert(new_node);
    ++_M_node_count;
    return iterator(new_node);
  }

  void _M_pre_fix_insert(_Link_type new_node) {
    new_node->_M_setRed();
    if (_S_parent(new_node)->_M_isRed()) {
      _M_fix_insert(new_node);
    }
  }

  void _M_fix_insert(_Link_type new_node) {
    _Link_type parent = _S_parent(new_node);
    _Link_type gparent = new_node->_M_gparent();
    _Link_type uncle = new_node->_M_uncle();
    while (true) {
      if (parent == gparent->_M_left) {
        if (uncle == nullptr || uncle->_M_isBlk()) {
          if (new_node == parent->_M_right) {
            _M_left_rotate(parent);
            new_node = parent;
            parent = _S_parent(new_node);
          }
          parent->_M_setBlk();
          gparent->_M_setRed();
          _M_right_rotate(gparent);
          return;
        } else {
          parent->_M_setBlk();
          uncle->_M_setBlk();
          if (gparent == _M_root()) {
            return;
          }
          gparent->_M_setRed();
          if (_S_parent(gparent)->_M_isBlk()) {
            return;
          }
          new_node = gparent;
          parent = _S_parent(new_node);
          gparent = new_node->_M_gparent();
          uncle = new_node->_M_uncle();
          continue;
        }
      } else {
        if (uncle == nullptr || uncle->_M_isBlk()) {
          if (new_node == parent->_M_left) {
            _M_right_rotate(parent);
            new_node = parent;
            parent = _S_parent(new_node);
          }
          parent->_M_setBlk();
          gparent->_M_setRed();
          _M_left_rotate(gparent);
          return;
        } else {
          parent->_M_setBlk();
          uncle->_M_setBlk();
          if (gparent == _M_root()) {
            return;
          }
          gparent->_M_setRed();
          if (_S_parent(gparent)->_M_isBlk()) {
            return;
          }
          new_node = gparent;
          parent = _S_parent(new_node);
          gparent = new_node->_M_gparent();
          uncle = new_node->_M_uncle();
          continue;
        }
      }
    }
    return; // Never reach.
  }

  void _M_left_rotate(_Base_ptr x) 
  {
    _Base_ptr rchild = x->_M_right;
    x->_M_right = rchild->_M_left;
    if (rchild->_M_left != nullptr) {
      rchild->_M_left->_M_parent = x;
    }
    if (x == _M_root()) {
      _M_root() = rchild;
    } else if (x == x->_M_parent->_M_left) {
      x->_M_parent->_M_left = rchild;
    } else {
      x->_M_parent->_M_right = rchild;
    }
    rchild->_M_parent = x->_M_parent;
    rchild->_M_left = x;
    x->_M_parent = rchild;
  }

  void _M_right_rotate(_Base_ptr x) 
  {
    _Base_ptr lchild = x->_M_left;
    x->_M_left = lchild->_M_right;
    if (lchild->_M_right != nullptr) {
      lchild->_M_right->_M_parent = x;
    }
    if (x == _M_root()) {
      _M_root() = lchild;
    } else if (x == x->_M_parent->_M_left) {
      x->_M_parent->_M_left = lchild;
    } else {
      x->_M_parent->_M_right = lchild;
    }
    lchild->_M_parent = x->_M_parent;
    lchild->_M_right = x;
    x->_M_parent = lchild;
  }

  template <class... _Args>
  tinySTL::pair<iterator, bool>
  _M_emplace_unique(_Args&&... __args) 
  {
    return _M_insert_unique(value_type(tinySTL::forward<_Args>(__args)...));
  }

  template <class... _Args> iterator
  _M_emplace_equal(_Args&&... __args) 
  {
    return _M_insert_equal(value_type(tinySTL::forward<_Args>(__args)...));
  }

  template <class... _Args> iterator
  _M_emplace_hint_unique(iterator __position, _Args&&... __args) 
  {
    return _M_insert_unique(__position, value_type(tinySTL::forward<_Args>(__args)...));
  }

  template <class... _Args> iterator
  _M_emplace_hint_equal(iterator __position, _Args&&... __args) 
  {
    return _M_insert_equal(__position, value_type(tinySTL::forward<_Args>(__args)...));
  }

  iterator 
  erase(iterator __pos) 
  {
    _Base_ptr pos = __pos._M_node;
    iterator it(pos);
    ++it;
    _Base_ptr y = pos, x = nullptr, x_parent = nullptr;
    if (y->_M_left == nullptr) {
      x = y->_M_right;
    } else if (y->_M_right == nullptr) {
      x = y->_M_left;
    } else {
      y = y->_M_right;
      while (y->_M_left != nullptr) {
        y = y->_M_left;
      }
      x = y->_M_right;
    }
    if (y != pos) {
      y->_M_left = pos->_M_left;
      pos->_M_left->_M_parent = y;
      if (y != pos->_M_right) {
        x_parent = y->_M_parent;
        if (x != nullptr) {
          x->_M_parent = y->_M_parent;
        }
        y->_M_parent->_M_left = x;
        y->_M_right = pos->_M_right;
        pos->_M_right->_M_parent = y;
      } else {
        x_parent = y;
      }

      if (pos == _M_root()) {
        _M_root() = y;
      } else if (pos->_M_parent->_M_left == pos) {
        pos->_M_parent->_M_left = y;
      } else {
        pos->_M_parent->_M_right = y;
      }

      y->_M_parent = pos->_M_parent;
      _Rb_tree_color tmp = y->_M_color;
      y->_M_color = pos->_M_color;
      pos->_M_color = tmp;
      y = pos;
    } else {
      x_parent = y->_M_parent;
      if (x != nullptr) {
        x->_M_parent = y->_M_parent;
      }
      
      if (pos == _M_root()) {
        _M_root() = x;
      } else if (pos->_M_parent->_M_left == pos) {
        pos->_M_parent->_M_left = x;
      } else {
        pos->_M_parent->_M_right = x;
      }

      if (pos == _M_leftmost()) {
        if (pos->_M_right == nullptr) {
          _M_leftmost() = pos->_M_parent;
        } else {
          _M_leftmost() = x;
        }
      }

      if (pos == _M_rightmost()) {
        if (pos->_M_left == nullptr) {
          _M_rightmost() = pos->_M_parent;
        } else {
          _M_rightmost() = x;
        }
      }
    }

    if (x == _M_root()) {
      if (x != nullptr) {
        x->_M_color = _Rb_tree_color::_S_black;
      }
    } else if (y->_M_color == _Rb_tree_color::_S_black) {
      _M_pre_fix_erase((_Link_type)x, (_Link_type)x_parent);
    }

    _M_put_node((_Link_type)y);
    --_M_node_count;
    return it;
  }

  size_type 
  erase(const key_type& __k) 
  {
    tinySTL::pair<iterator,iterator> __p = equal_range(__k);
    size_type __n = tinySTL::distance(__p.first, __p.second);
    erase(__p.first, __p.second);
    return __n;
  }

  iterator
  erase(iterator __first, iterator __last) 
  {
    if (__first == begin() && __last == end()) {
      clear();
    } else {
      while (__first != __last) erase(__first++);
    }
    return __last;
  }

  void 
  erase(const key_type* __first, const key_type* __last) 
  {
    while (__first != __last) erase(*__first++);
  }

  void _M_pre_fix_erase(_Link_type successor, _Link_type suc_parent) 
  {
    if (successor != nullptr && successor->_M_isRed()) {
      successor->_M_setBlk();
    } else {
      _M_fix_erase(successor, suc_parent);
    }
  }

  void _M_fix_erase(_Link_type successor, _Link_type suc_parent) 
  {
    _Link_type parent = suc_parent, brother;
    while (true) {
      if (successor == _S_left(parent)) {
        brother = _S_right(parent);
        if (brother->_M_isRed()) {
          parent->_M_setRed();
          brother->_M_setBlk();
          _M_left_rotate(parent);
          brother = _S_right(parent);
        }
        if ((_S_left(brother) == 0 || _S_left(brother)->_M_isBlk())
         && (_S_right(brother) == 0 || _S_right(brother)->_M_isBlk())) {
          brother->_M_setRed();
          if (parent->_M_isRed()) {
            parent->_M_setBlk();
            return;
          }
          if (parent == _M_root()) {
            return;
          }
          successor = parent;
          parent = _S_parent(successor);
          brother = successor->_M_brother();
          continue;
        } else {
          if (_S_right(brother) == 0 || _S_right(brother)->_M_isBlk()) {
            brother->_M_setRed();
            _S_left(brother)->_M_setBlk();
            _M_right_rotate(brother);
            brother = _S_right(parent);
          }
          brother->_M_color = parent->_M_color;
          parent->_M_setBlk();
          _S_right(brother)->_M_setBlk();
          _M_left_rotate(parent);
          return;
        }
      } else {
        brother = _S_left(parent);
        if (brother->_M_isRed()) {
          parent->_M_setRed();
          brother->_M_setBlk();
          _M_right_rotate(parent);
          brother = _S_left(parent);
        }
        if ((_S_right(brother) == 0 || _S_right(brother)->_M_isBlk())
         && (_S_left(brother) == 0 || _S_left(brother)->_M_isBlk())) {
          brother->_M_setRed();
          if (parent->_M_isRed()) {
            parent->_M_setBlk();
            return;
          }
          if (parent == _M_root()) {
            return;
          }
          successor = parent;
          parent = _S_parent(successor);
          brother = successor->_M_brother();
          continue;
        } else {
          if (_S_left(brother) == 0 || _S_left(brother)->_M_isBlk()) {
            brother->_M_setRed();
            _S_right(brother)->_M_setBlk();
            _M_left_rotate(brother);
            brother = _S_left(parent);
          }
          brother->_M_color = parent->_M_color;
          parent->_M_setBlk();
          _S_left(brother)->_M_setBlk();
          _M_right_rotate(parent);
          return;
        }
      }
    }
    return; // Never reach.
  }

  friend std::ostream& operator<<(std::ostream& os, const _Rb_tree& tree) {
    os << '{';
    auto __from = tree.begin(), __to = tree.end();
    if (__from != __to) {
      os << *__from;
      ++__from;
      while (__from != __to) {
        os << ", " << *__from;
        ++__from;
      }
    }
    return os << '}';
  }

  void disp(std::ostream& os) const {
    if (!empty())
      __disp(os, (_Link_type)_M_root());
    os << '\n';
  }

  void __disp(std::ostream& os, _Link_type rt) const {
    if (rt->_M_isRed()) {
      os << "\033[1;31m" << *rt->_M_valptr();
    } else {
      os << "\033[1;34m" << *rt->_M_valptr();
    }
    os << "\033[0m";
    if (rt->_M_left != 0 || rt->_M_right != 0) {
      os << '(';
      if (rt->_M_left != 0) {
        __disp(os, (_Link_type)rt->_M_left);
      }
      if (rt->_M_right != 0) {
        os << ", ";
        __disp(os, (_Link_type)rt->_M_right);
      }
      os << ')';
    }
  }

  template <class _Compare2>
  void _M_merge_unique(_Rb_tree<_Key, _Val, _KeyOfValue, _Compare2, _Alloc>&& __tree) 
  {
    if (_M_header == nullptr && __tree._M_header == nullptr) {
      return;
    }
    if (_M_header == nullptr) {
      *this = tinySTL::move(__tree);
      return;
    }
    iterator pos = end();
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare2, _Alloc> tmp;
    _M_merge_unique_aux((_Link_type)__tree._M_root(), pos, tmp);
    __tree._M_reset();
    __tree = tinySTL::move(tmp);
  }

  template <class _Compare2>
  void _M_merge_equal(_Rb_tree<_Key, _Val, _KeyOfValue, _Compare2, _Alloc>&& __tree) 
  {
    if (_M_header == nullptr && __tree._M_header == nullptr) {
      return;
    }
    if (_M_header == nullptr) {
      *this = tinySTL::move(__tree);
      return;
    }
    iterator pos = end();
    _M_merge_equal_aux((_Link_type)__tree._M_root(), pos);
    __tree._M_reset();
  }

  template <class _Compare2>
  void _M_merge_unique_aux(_Link_type __x, iterator& pos, 
      _Rb_tree<_Key, _Val, _KeyOfValue, _Compare2, _Alloc>& tmp) 
  {
    while (__x != 0) {
      _M_merge_unique_aux(_S_right(__x), pos, tmp);
      _Link_type __y = _S_left(__x);
      auto __pair = _M_insert_unique(pos, __x);
      pos = __pair.first;
      if (__pair.second == false) {
        tmp._M_insert_node_equal(__x);
      }
      __x = __y;
    }
  }

  void _M_merge_equal_aux(_Link_type __x, iterator& pos) 
  {
    while (__x != 0) {
      _M_merge_equal_aux(_S_right(__x), pos);
      _Link_type __y = _S_left(__x);
      pos = _M_insert_equal(pos, __x);
      __x = __y;
    }
  }

  iterator
  find(const key_type& __k) 
  {
    iterator j = lower_bound(__k);
    return (j == end() || key_comp()(__k, _S_key(j._M_node))) ? end() : j;
  }

  const_iterator
  find(const key_type& __k) const 
  {
    const_iterator j = lower_bound(__k);
    return (j == end() || key_comp()(__k, _S_key(j.base()._M_node))) ? end() : j;
  }

  size_type
  count_unique(const key_type& __k) const 
  {
    if (_M_header == nullptr) {
      return 0;
    }
    _Const_Base_ptr x = _M_root();
    while (x != nullptr) {
      if (key_comp()(__k, _S_key(x))) {
        x = x->_M_left;
      } else if (key_comp()(_S_key(x), __k)) {
        x = x->_M_right;
      } else {
        return 1;
      }
    }
    return 0;
  }

  size_type
  count_multi(const key_type& __k) const 
  {
    if (_M_header == nullptr) {
      return 0;
    }
    _Const_Base_ptr y = _M_head();
    _Const_Base_ptr x = _M_root();
    while (x != nullptr) {
      if (key_comp()(__k, _S_key(x))) {
        y = x;
        x = x->_M_left;
      } else if (key_comp()(_S_key(x), __k)) {
        x = x->_M_right;
      } else {
        return tinySTL::distance(
          __lower_bound(__k, x->_M_left, x), 
          __upper_bound(__k, x->_M_right, y)
        );
      }
    }
    return 0;
  }

  iterator
  lower_bound(const key_type& __k) 
  {
    if (_M_header == nullptr) {
      return end();
    }
    _Base_ptr y = _M_head();
    _Base_ptr x = _M_root();
    return __lower_bound(__k, x, y);
  }

  iterator
  __lower_bound(const key_type& __k, _Base_ptr x, _Base_ptr y) 
  {
    while (x != nullptr) {
      if (!key_comp()(_S_key(x), __k)) {
        y = x;
        x = x->_M_left;
      } else {
        x = x->_M_right;
      }
    }
    return iterator(y);
  }

  const_iterator
  lower_bound(const key_type& __k) const 
  {
    if (_M_header == nullptr) {
      return end();
    }
    _Const_Base_ptr y = _M_head();
    _Const_Base_ptr x = _M_root();
    return __lower_bound(__k, x, y);
  }

  const_iterator
  __lower_bound(const key_type& __k, _Const_Base_ptr x, _Const_Base_ptr y) const 
  {
    while (x != nullptr) {
      if (!key_comp()(_S_key(x), __k)) {
        y = x;
        x = x->_M_left;
      } else {
        x = x->_M_right;
      }
    }
    return const_iterator((_Base_ptr)y);
  }

  iterator
  upper_bound(const key_type& __k) 
  {
    if (_M_header == nullptr) {
      return end();
    }
    _Base_ptr y = _M_head();
    _Base_ptr x = _M_root();
    return __upper_bound(__k, x, y);
  }

  iterator
  __upper_bound(const key_type& __k, _Base_ptr x, _Base_ptr y) 
  {
    while (x != nullptr) {
      if (key_comp()(__k, _S_key(x))) {
        y = x;
        x = x->_M_left;
      } else {
        x = x->_M_right;
      }
    }
    return iterator(y);
  }

  const_iterator
  upper_bound(const key_type& __k) const 
  {
    if (_M_header == nullptr) {
      return end();
    }
    _Const_Base_ptr y = _M_head();
    _Const_Base_ptr x = _M_root();
    return __upper_bound(__k, x, y);
  }

  const_iterator
  __upper_bound(const key_type& __k, _Const_Base_ptr x, _Const_Base_ptr y) const 
  {
    while (x != nullptr) {
      if (key_comp()(__k, _S_key(x))) {
        y = x;
        x = x->_M_left;
      } else {
        x = x->_M_right;
      }
    }
    return const_iterator((_Base_ptr)y);
  }

  tinySTL::pair<iterator, iterator>
  equal_range(const key_type& __k) 
  { return {lower_bound(__k), upper_bound(__k)}; }

  tinySTL::pair<const_iterator, const_iterator>
  equal_range(const key_type& __k) const
  { return {lower_bound(__k), upper_bound(__k)}; }

 protected:
  void _M_erase(_Link_type __x)
  {
    while (__x != 0) {
      _M_erase(_S_right(__x));
      _Link_type __y = _S_left(__x);
      _M_drop_node(__x);
      __x = __y;
    }
  }

};

}