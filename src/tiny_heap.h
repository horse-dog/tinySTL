#pragma once

#include "tiny_iterator_base.h"

namespace tinySTL
{

/**
 *  @note 
 *  This function requires that 
 *  the subsequence [__first, __first + __index) is already heaped.
 *  after shift up, [__first, __first + __index] will be heaped. 
 * 
 *  @param __first Start of sequence.
 *  @param __index Shift point.
 *  @param __comp  Comparison functor to use.
 */
template 
<typename _RandomAccessIterator, typename _Distance, typename _Compare> 
void __shift_up(_RandomAccessIterator __first, 
      _Distance __index, _Compare __comp)
{
  _Distance __parent = (__index - 1) / 2;
  auto __tmp = tinySTL::move(*(__first + __index));
  while (__index > 0 && __comp(*(__first + __parent), __tmp)) {
    *(__first + __index) = tinySTL::move(*(__first + __parent));
    __index = __parent;
    __parent = (__index - 1) / 2;
  }
  *(__first + __index) = tinySTL::move(__tmp);
}

/**
 *  @note 
 *  This function requires that 
 *  the left and right subtrees of @p __index are both heaped.
 * 
 *  @param __first Start of sequence.
 *  @param __index Shift point.
 *  @param __len   Length of sequence.
 *  @param __comp  Comparison functor to use.
 * 
 *  @return 
 *   true: __shfit_down the element at @p __index.
 *  false: do nothing at @p __index.
 */
template
<typename _RandomAccessIterator, typename _Distance, typename _Compare>
bool __shift_down(_RandomAccessIterator __first, 
      _Distance __index, _Distance __len, _Compare __comp)
{
  _Distance __holeIndex = __index;
  _Distance __bestChild = __holeIndex * 2 + 1;
  auto __tmp = tinySTL::move(*(__first + __index));

  while (__bestChild < __len) {
    // if rChild exists and rChild > lChild (__comp = less).
    if (__bestChild + 1 < __len 
     && __comp(*(__first + __bestChild), *(__first + __bestChild + 1)))
      ++__bestChild;
    if (*(__first + __bestChild) < __tmp) break;
    *(__first + __holeIndex) = tinySTL::move(*(__first + __bestChild));
    __holeIndex = __bestChild;
    __bestChild = __holeIndex * 2 + 1;
  }

  *(__first + __holeIndex) = tinySTL::move(__tmp);
  return __holeIndex > __index;
}

/**
 *  @brief  Construct a heap over a range using comparison functor.
 *  @param  __first  Start of heap.
 *  @param  __last   End of heap.
 *  @param  __comp   Comparison functor to use.
 *  @note parent(i) = (i - 1) / 2.
 *  @note lchild(i) = 2 * i + 1.
 *  @note rchild(i) = 2 * i + 2.
 *
 *  This operation makes the elements in [__first,__last) into a heap.
 *  Comparisons are made using __comp.
 */
template<typename _RandomAccessIterator, typename _Compare>
inline void
make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
          _Compare __comp)
{
  typedef typename iterator_traits
  <_RandomAccessIterator>::difference_type _DistanceType;
  const _DistanceType __len = __last - __first;
  
  // __last -1 <==> __len - 1.
  // parent(__last - 1) <==> [(__len - 1) - 1] / 2. 
  _DistanceType __parent = (__len - 2) / 2;
  
  while (true) {
    // After __shift_down, 
    // subtree with __parent as root node will be a heap.
    __shift_down(__first, __parent, __len, __comp);
    
    // all subtrees make heap down, return.
    if (__parent == 0) return;
    --__parent; // continue make_heap.
  }
}

/**
 *  @brief  Construct a heap over a range.
 *  @param  __first  Start of heap.
 *  @param  __last   End of heap.
 *  @note parent(i) = (i - 1) / 2.
 *  @note lchild(i) = 2 * i + 1.
 *  @note rchild(i) = 2 * i + 2.
 *
 *  This operation makes the elements in [__first,__last) into a heap.
 */
template<typename _RandomAccessIterator>
inline void
make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  using _Value_type = typename iterator_traits<_RandomAccessIterator>::value_type;
  tinySTL::make_heap(
    __first, __last,
    [](const _Value_type& __a, const _Value_type& __b)
    { return __a < __b; }
  );
}

/**
 *  @brief  Push an element onto a heap using comparison functor.
 *  @param  __first  Start of heap.
 *  @param  __last   End of heap + element.
 *  @param  __comp   Comparison functor to use.
 *
 *  This operation pushes the element at last-1 onto the valid heap
 *  over the range [__first,__last-1).  After completion,
 *  [__first,__last) is a valid heap.
 */
template<typename _RandomAccessIterator, typename _Compare>
inline void
push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
          _Compare __comp)
{
  __shift_up(__first, __last - __first - 1, __comp);
}

/**
 *  @brief  Push an element onto a heap.
 *  @param  __first  Start of heap.
 *  @param  __last   End of heap + element.
 *
 *  This operation pushes the element at last-1 onto the valid heap
 *  over the range [__first,__last-1).  After completion,
 *  [__first,__last) is a valid heap.
 */
template<typename _RandomAccessIterator>
inline void
push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  using _Value_type = typename iterator_traits<_RandomAccessIterator>::value_type;
  tinySTL::push_heap(
    __first, __last,
    [](const _Value_type& __a, const _Value_type& __b)
    { return __a < __b; }
  );
}

/**
 *  @brief  Delete an element off a heap using comparison functor.
 *  @param  __first  Start of heap.
 *  @param  __last   End of heap.
 *  @param  __index  Delete point.
 *  @param  __comp   Comparison functor to use.
 *
 *  This operation pops the top of the heap.  The elements at __index
 *  and __last - 1 are swapped and [__first, __last - 1) is made into
 *  a heap.  Comparisons are made using comp.
*/
template
<typename _RandomAccessIterator, typename _Distance, typename _Compare> 
inline void
del_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
         _Distance __index, _Compare __comp)
{
  _Distance __len = __last - __first;
  if (__index < 0 || __index >= __len - 1) return;
  auto __tmp = tinySTL::move(*(__first + __index));
  *(__first + __index) = tinySTL::move(*(__last - 1));
  *(__last - 1) = tinySTL::move(__tmp);
  if (!__shift_down(__first, __index, __len - 1, __comp))
    __shift_up(__first, __index, __comp);
  return;
}

/**
 *  @brief  Delete an element off a heap.
 *  @param  __first  Start of heap.
 *  @param  __last   End of heap.
 *  @param  __index  Delete point.
 *
 *  This operation pops the top of the heap.  The elements at __index
 *  and __last - 1 are swapped and [__first, __last - 1) is made into
 *  a heap.
*/
template
<typename _RandomAccessIterator, typename _Distance> 
inline void
del_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
         _Distance __index)
{
  using _Value_type = typename iterator_traits<_RandomAccessIterator>::value_type;
  tinySTL::del_heap(
    __first, __last, __index,
    [](const _Value_type& __a, const _Value_type& __b)
    { return __a < __b; }
  );
}

/**
 *  @brief  Pop an element off a heap using comparison functor.
 *  @param  __first  Start of heap.
 *  @param  __last   End of heap.
 *  @param  __comp   Comparison functor to use.
 *
 *  This operation pops the top of the heap.  The elements __first
 *  and __last-1 are swapped and [__first,__last-1) is made into a
 *  heap.  Comparisons are made using comp.
*/
template<typename _RandomAccessIterator, typename _Compare>
inline void
pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
         _Compare __comp)
{
  tinySTL::del_heap(__first, __last, 0, __comp);
}

/**
 *  @brief  Pop an element off a heap.
 *  @param  __first  Start of heap.
 *  @param  __last   End of heap.
 *
 *  This operation pops the top of the heap.  The elements __first
 *  and __last-1 are swapped and [__first,__last-1) is made into a
 *  heap.
*/
template<typename _RandomAccessIterator>
inline void
pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  using _Value_type = typename iterator_traits<_RandomAccessIterator>::value_type;
  tinySTL::pop_heap(
    __first, __last,
    [](const _Value_type& __a, const _Value_type& __b)
    { return __a < __b; }
  );
}

/**
 *  @brief  Sort a heap using comparison functor.
 *  @param  __first  Start of heap.
 *  @param  __last   End of heap.
 *  @param  __comp   Comparison functor to use.
 *
 *  This operation sorts the valid heap in the range [__first,__last).
 *  Comparisons are made using __comp.
 */
template<typename _RandomAccessIterator, typename _Compare>
inline void
sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
         _Compare __comp)
{
  while (__last - __first > 1)
    tinySTL::pop_heap(__first, __last--, __comp);
}

/**
 *  @brief  Sort a heap.
 *  @param  __first  Start of heap.
 *  @param  __last   End of heap.
 *
 *  This operation sorts the valid heap in the range [__first,__last).
 */
template<typename _RandomAccessIterator>
inline void
sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  using _Value_type = typename iterator_traits<_RandomAccessIterator>::value_type;
  tinySTL::sort_heap(
    __first, __last,
    [](const _Value_type& __a, const _Value_type& __b)
    { return __a < __b; }
  );
}

}