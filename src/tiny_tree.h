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


struct _Rb_tree_header
{
  _Rb_tree_node_base  _M_header;
  size_t _M_node_count;

  _Rb_tree_header() noexcept
  {
    _M_header._M_color = _Rb_tree_color::_S_red;
    _M_reset();
  }

  _Rb_tree_header(_Rb_tree_header&& __x) noexcept
  {
    /**
     * when __x._M_header._M_parent == 0, __x is
     * an empty tree (just has head node). 
     */
    if (__x._M_header._M_parent != nullptr)
      _M_move_data(__x);
    else {
      _M_header._M_color = _Rb_tree_color::_S_red;
      _M_reset();
    }
  }

  _Rb_tree_header& operator=(_Rb_tree_header&& __x) noexcept 
  {
    if (this != &__x) {
      this->_M_reset(); // TODO: 内存未释放？
      tinySTL::construct(this, tinySTL::move(__x));
    }
    return *this;
  }

  void _M_move_data(_Rb_tree_header& __from)
  {
    _M_header._M_color = __from._M_header._M_color;
    _M_header._M_parent = __from._M_header._M_parent;
    _M_header._M_left = __from._M_header._M_left;
    _M_header._M_right = __from._M_header._M_right;
    _M_node_count = __from._M_node_count;
    __from._M_reset();
  }

  /**
   *  If the red-black tree is empty, we set the parent node
   *  of the head node to nil, the left child of the head 
   *  node to the head node itself, and the right child to 
   *  the head node itself.
   */
  void _M_reset()
  {
    _M_header._M_parent = 0;
    _M_header._M_left = &_M_header;
    _M_header._M_right = &_M_header;
    _M_node_count = 0;
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
  struct _Rb_tree_impl
  : public _Node_allocator, public _Rb_tree_header
  {
    _Compare _M_key_compare;

    _Rb_tree_impl()
    : _Node_allocator(), _M_key_compare(), _Rb_tree_header()
    { }

    _Rb_tree_impl(const _Rb_tree_impl& __x)
    : _Node_allocator(__x), 
      _M_key_compare(__x._M_key_compare), _Rb_tree_header()
    { }

    _Rb_tree_impl(_Rb_tree_impl&& __x) = default;

    explicit
    _Rb_tree_impl(_Node_allocator&& __a)
    : _Node_allocator(tinySTL::move(__a)), 
      _M_key_compare(), _Rb_tree_header()
    { }

    _Rb_tree_impl(const _Compare& __comp, _Node_allocator&& __a)
    : _Node_allocator(tinySTL::move(__a)), _M_key_compare(__comp),
      _Rb_tree_header()
    { }

  };

  _Rb_tree_impl _M_impl;
  
  _Link_type _M_get_node()
  { return _M_impl.allocate(1); }

  void _M_put_node(_Link_type __p)
  { return _M_impl.deallocate(__p, 1); }

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

  template <class... _Args>
  _Link_type _M_create_node(_Args&&... __args)
  {
    _Link_type __tmp = _M_get_node();
    _M_construct_node(__tmp, tinySTL::forward<_Args>(__args)...);
    return __tmp;
  }

  void _M_destroy_node(_Link_type __p) noexcept
  {
    tinySTL::destroy(__p->_M_valptr());
  }

  void _M_drop_node(_Link_type __p) noexcept
  {
    _M_destroy_node(__p);
    _M_put_node(__p);
  }

  _Link_type _M_clone_node(_Link_type __x)
  {
    _Link_type __tmp = _M_create_node(*__x->_M_valptr());
    __tmp->_M_color = __x->_M_color;
    __tmp->_M_left = 0;
    __tmp->_M_right = 0;
    return __tmp;
  }

  _Base_ptr& _M_root() noexcept
  { return _M_impl._M_header._M_parent; }

  _Const_Base_ptr _M_root() const noexcept
  { return _M_impl._M_header._M_parent; }

  _Base_ptr& _M_leftmost() noexcept
  { return _M_impl._M_header._M_left; }

  _Const_Base_ptr _M_leftmost() const noexcept
  { return _M_impl._M_header._M_left; }

  _Base_ptr& _M_rightmost() noexcept
  { return _M_impl._M_header._M_right; }

  _Const_Base_ptr _M_rightmost() const noexcept
  { return _M_impl._M_header._M_right; }

  _Base_ptr _M_head()  
  { return &_M_impl._M_header; }

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
  _Rb_tree() = default;

  _Rb_tree(const _Compare& __comp, 
     const allocator_type& __a = allocator_type())
  : _M_impl(__comp, _Node_allocator(__a)) { }

  _Rb_tree(const _Rb_tree& __x)
  : _M_impl(__x._M_impl)
  { _M_copy(__x); }

  _Rb_tree(_Rb_tree&& __x) = default;

  ~_Rb_tree() { clear(); }

  _Rb_tree& operator=(const _Rb_tree& __x)
  {
    if (this != &__x) {
      clear();
      _M_impl._M_key_compare = __x._M_impl._M_key_compare;
      _M_copy(__x);
    }
    return *this;
  }

  _Rb_tree& operator=(_Rb_tree&& __x) 
  {
    if (this != &__x) {
      clear();
      _M_impl._M_key_compare = 
        tinySTL::move(__x._M_impl._M_key_compare);
      if (__x._M_root())
        _M_impl._M_move_data(__x._M_impl);
    }
  }

 public:
  _Compare key_comp() const 
  { return _M_impl._M_key_compare; }

  iterator begin() noexcept
  { return iterator(_M_impl._M_header._M_left); }

  const_iterator begin() const noexcept
  { return const_iterator(_M_impl._M_header._M_left); }

  iterator end() noexcept 
  { return iterator(&_M_impl._M_header); }

  const_iterator end() const noexcept 
  { return const_iterator(const_cast<_Base_ptr>(&_M_impl._M_header)); }

  reverse_iterator rbegin() noexcept 
  { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const noexcept
  { return const_reverse_iterator(end()); }

  reverse_iterator rend() noexcept
  { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const noexcept 
  { return const_reverse_iterator(begin()); }

  bool empty() const noexcept 
  { return _M_impl._M_node_count == 0; }

  size_type size() const noexcept 
  { return _M_impl._M_node_count; }

  size_type max_size() const noexcept 
  { return size_type(-1); }

  void swap(_Rb_tree& __t) 
  {
    tinySTL::swap(_M_impl._M_key_compare, __t._M_impl._M_key_compare);
    // tinySTL::swap(_Rb_tree_header(_M_impl), _Rb_tree_header(__t._M_impl)); // TODO.
    tinySTL::swap(_M_impl, __t._M_impl);
  }

  void clear() 
  {
    _M_erase(_Link_type(_M_root()));
    _M_impl._M_reset();
  }

  template <class _Arg>
  tinySTL::pair<iterator, bool>
  _M_insert_unique(_Arg&& __x) 
  {
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
        return {_M_insert(pos_parent, tinySTL::forward<_Arg>(__x)), true};
      } else {
        --j;
      }
    }
    if (key_comp()(_S_key(j._M_node), _KeyOfValue()(__x))) {
      return {_M_insert(pos_parent, tinySTL::forward<_Arg>(__x)), true};
    }
    return {j, false};
  }

  template <class _Arg> iterator
  _M_insert_equal(_Arg&& __x) 
  {
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
    return _M_insert(pos_parent, tinySTL::forward<_Arg>(__x));
  }
  
  template <class _Arg> iterator
  _M_insert(_Base_ptr __pos_parent, _Arg&& __x)
  {
    _Link_type new_node = _M_create_node(tinySTL::forward<_Arg>(__x));
    if (__pos_parent == _M_head()) {
      _M_leftmost() = _M_rightmost() = _M_root() = new_node;
      new_node->_M_setBlk();
      new_node->_M_parent = __pos_parent;
      new_node->_M_left = new_node->_M_right = 0;
      ++_M_impl._M_node_count;
      return iterator(new_node);
    } 

    if (key_comp()(_S_key(new_node), _S_key(__pos_parent))) {
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
    ++_M_impl._M_node_count;
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
          _M_right_rotate(parent);
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

  iterator 
  erase(const_iterator __pos);

  size_type 
  erase(const key_type& __x);

  iterator
  erase(const_iterator __first, const_iterator __last);

  void 
  erase(const key_type* __first, const key_type* __last);

  iterator
  find(const key_type& __k);

  const_iterator
  find(const key_type& __k) const;

  size_type
  count(const key_type& __k) const;

  iterator
  lower_bound(const key_type& __k);

  const_iterator
  lower_bound(const key_type& __k) const;

  iterator
  upper_bound(const key_type& __k);

  const_iterator
  upper_bound(const key_type& __k) const;

  tinySTL::pair<iterator, iterator>
  equal_range(const key_type& __k);

  tinySTL::pair<const_iterator, const_iterator>
  equal_range(const key_type& __k) const;

 protected:
  void _M_copy(const _Rb_tree& __x)
  {
    if (__x._M_root()) {
      _Link_type __tmp = _M_copy(__x._M_root(), &_M_impl._M_header);
      _M_root() = __tmp;
      _M_leftmost() = _S_minimum(__tmp);
      _M_rightmost() = _S_maximum(__tmp);
      _M_impl._M_node_count = __x._M_impl._M_node_count;
    }
  }

  _Link_type _M_copy(_Link_type __x, _Link_type __p)
  {
    _Link_type __top = _M_clone_node(__x);
    __top->_M_parent = __p;

    try {
      if (__x->_M_right)
        __top->_M_right = _M_copy(_S_right(__x, __top));
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
  }

  void _M_erase(_Link_type __x)
  {
    while (__x != 0) {
      _M_erase(_S_right(__x));
      _Link_type __y = _S_left(__x);
      _M_put_node(__x);
      __x = __y;
    }
  }

};

}