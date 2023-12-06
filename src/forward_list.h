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
  class _Slist_iterator;

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

 public:
  forward_list(const allocator_type& __a = allocator_type()) : _M_head() {}

  explicit forward_list(size_type __n);

  forward_list(size_type __n, const _Tp& __value,
    const allocator_type& __a = allocator_type());

  forward_list(const forward_list& __x);

  forward_list(forward_list&& __x);

  forward_list(std::initializer_list<_Tp> __l, 
    const allocator_type& __a = allocator_type());

  template <InputIterator Iterator>
  forward_list(Iterator __first, Iterator __last,
       const allocator_type& __a = allocator_type());

  ~forward_list();

  forward_list& operator=(std::initializer_list<_Tp> __l);

  forward_list& operator=(const forward_list& __x);

  forward_list& operator=(forward_list&& __x);

  void assign(std::initializer_list<_Tp> __l);

  void assign(size_type __n, const _Tp& __val);

  template <InputIterator Iterator>
  void assign(Iterator __first, Iterator __last);

 public:
  allocator_type get_allocator() const { return allocator_type(); }

  iterator begin();

  const_iterator begin() const;

  const_iterator cbegin() const;

  iterator before_begin();

  const_iterator before_begin() const;

  const_iterator cbefore_begin() const;

  iterator end();

  const_iterator end() const;

  const_iterator cend() const;

  bool empty() const;

  void clear();

  size_type max_size() const;

  reference front();

  const_reference front() const;

  void swap(forward_list<_Tp, _Alloc>& __x);

  iterator insert_after(const_iterator __pos, std::initializer_list<T> __il);

  iterator insert_after(const_iterator __pos, _Tp&& __val);

  iterator insert_after(const_iterator __pos, const _Tp& __val);

  iterator insert_after(const_iterator __pos, size_type __n, const _Tp& __val);

  template <InputIterator Iterator>
  iterator insert_after(const_iterator __pos, Iterator __first,
                        Iterator __last);

  template <class... _Args>
  iterator emplace_after(const_iterator __pos, _Args&& ...__args);

  template <typename... _Args>
  reference emplace_front(_Args&& ...__args);

  iterator erase_after(const_iterator __pos);

  iterator erase_after(const_iterator __pos, const_iterator __last);

  void push_front(_Tp&& __val);

  void push_front(const _Tp& __val);

  void pop_front();

  /**
   * @brief merge this forward_list with another forward_list,
   * these two forward_lists should be ordered before use this function
   * @param __list another list
   */
  void merge(forward_list&& __list);

  /**
   * @brief merge this forward_list with another forward_list,
   * these two forward_lists should be ordered before use this function
   * @param __list another list
   */
  void merge(forward_list& __list);

  /**
   * @brief merge this forward_list with another forward_list,
   * these two forward_lists should be ordered before use this function
   * @param __list another forward_list
   * @param __comp sort method
   */
  template <class _Comp>
  void merge(forward_list&& __list, _Comp __comp);

  /**
   * @brief merge this forward_list with another forward_list,
   * these two forward_lists should be ordered before use this function
   * @param __list another forward_list
   * @param __comp sort method
   */
  template <class _Comp>
  void merge(forward_list& __list, _Comp __comp);

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