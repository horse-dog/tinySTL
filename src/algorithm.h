#pragma once

#include "tiny_heap.h"
#include "tiny_algobase.h"
#include "tiny_function.h"

namespace tinySTL
{

template <class _ForwardIterator, class _BinaryPredicate> _ForwardIterator
adjacent_find(_ForwardIterator __first, _ForwardIterator __last, _BinaryPredicate __pred) 
{
  if (__first == __last)
    return __first;
  _ForwardIterator __i = __first;
  while (++__i != __last) {
    if (__pred(*__first, *__i))
      return __first;
    __first = __i;
  }
  return __i;
}

template <class _ForwardIterator> _ForwardIterator
adjacent_find(_ForwardIterator __first, _ForwardIterator __last) 
{
  return tinySTL::adjacent_find(__first, __last, tinySTL::__equal_to{});
}

template <class _InputIterator, class _Tp>
typename tinySTL::iterator_traits<_InputIterator>::difference_type
count(_InputIterator __first, _InputIterator __last, const _Tp& __value) {
  typename tinySTL::iterator_traits<_InputIterator>::difference_type __r(0);
  for (; __first != __last; ++__first)
    if (*__first == __value)
      ++__r;
  return __r;
}

template <class _InputIterator, class _Predicate>
typename tinySTL::iterator_traits<_InputIterator>::difference_type
count_if(_InputIterator __first, _InputIterator __last, _Predicate __pred) {
  typename tinySTL::iterator_traits<_InputIterator>::difference_type __r(0);
  for (; __first != __last; ++__first)
    if (__pred(*__first))
      ++__r;
  return __r;
}

template <class _ForwardIterator, class _Tp, class _Compare>
_ForwardIterator lower_bound(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value, _Compare __comp) 
{
  auto __len = tinySTL::distance(__first, __last);
  while (__len != 0) {
    auto __l2 = (__len >> 1);
    _ForwardIterator __m = __first;
    tinySTL::advance(__m, __l2);
    if (__comp(*__m, __value)) {
      __first = ++__m;
      __len -= __l2 + 1;
    } else {
      __len = __l2;
    }
  }
  return __first;
}

template <class _ForwardIterator, class _Tp>
_ForwardIterator lower_bound(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) 
{
  return tinySTL::lower_bound(__first, __last, __value, tinySTL::__less{});
}

template <class _ForwardIterator, class _Tp, class _Compare>
_ForwardIterator upper_bound(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value, _Compare __comp) 
{
  auto __len = tinySTL::distance(__first, __last);
  while (__len != 0) {
    auto __l2 = (__len >> 1);
    _ForwardIterator __m = __first;
    tinySTL::advance(__m, __l2);
    if (!__comp(__value, *__m)) {
      __first = ++__m;
      __len -= __l2 + 1;
    } else {
      __len = __l2;
    }
  }
  return __first;
}

template <class _ForwardIterator, class _Tp>
_ForwardIterator upper_bound(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) 
{
  return tinySTL::upper_bound(__first, __last, __value, tinySTL::__less{});
}

template <class _ForwardIterator, class _Tp, class _Compare> bool
binary_search(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value, _Compare __comp) 
{
  __first = tinySTL::lower_bound(__first, __last, __value, __comp);
  return __first != __last && !__comp(__value, *__first);
}

template <class _ForwardIterator, class _Tp> bool
binary_search(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) 
{
  return tinySTL::binary_search(__first, __last, __value, tinySTL::__less{});
}

template <class _ForwardIterator, class _Tp, class _Compare>
tinySTL::pair<_ForwardIterator, _ForwardIterator>
equal_range(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value, _Compare __comp) 
{
  return {
    tinySTL::lower_bound(__first, __last, __value, __comp), 
    tinySTL::upper_bound(__first, __last, __value, __comp)
  };
}

template <class _ForwardIterator, class _Tp>
tinySTL::pair<_ForwardIterator, _ForwardIterator>
equal_range(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) 
{
  return equal_range(__first, __last, __value, tinySTL::__less{});
}

template <class _InputIterator, class _Tp> _InputIterator
find(_InputIterator __first, _InputIterator __last, const _Tp& __value) 
{
  for (; __first != __last; ++__first)
    if (*__first == __value)
      break;
  return __first;
}

template <class _InputIterator, class _Predicate> _InputIterator
find_if(_InputIterator __first, _InputIterator __last, _Predicate __pred) 
{
  for (; __first != __last; ++__first)
    if (__pred(*__first))
      break;
  return __first;
}

template <class _ForwardIterator1, class _ForwardIterator2, class _BinaryPredicate>
_ForwardIterator1 search(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                         _ForwardIterator2 __first2, _ForwardIterator2 __last2,
                         _BinaryPredicate __pred) 
{
  if (__first2 == __last2)
    return __first1; // Everything matches an empty sequence
  while (true) {
    // Find first element in sequence 1 that matchs *__first2, with a mininum of loop checks
    while (true) {
      if (__first1 == __last1) { // return __last1 if no element matches *__first2
        return __first1;
      }
      if (__pred(*__first1, *__first2))
        break;
      ++__first1;
    }
    // *__first1 matches *__first2, now match elements after here
    _ForwardIterator1 __m1 = __first1;
    _ForwardIterator2 __m2 = __first2;
    while (true) {
      if (++__m2 == __last2) // If pattern exhausted, __first1 is the answer (works for 1 element pattern)
        return __first1;
      if (++__m1 == __last1) { // Otherwise if source exhaused, pattern not found
        return __m1;
      }

      // if there is a mismatch, restart with a new __first1
      if (!__pred(*__m1, *__m2))
      {
        ++__first1;
        break;
      } // else there is a match, check next elements
    }
  }
}

template <class _ForwardIterator1, class _ForwardIterator2>
_ForwardIterator1 search(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
                         _ForwardIterator2 __first2, _ForwardIterator2 __last2) 
{
  return tinySTL::search(__first1, __last1, __first2, __last2, tinySTL::__equal_to{});
}

template <class _ForwardIterator, class _Size, class _Tp, class _BinaryPredicate>
_ForwardIterator search_n(_ForwardIterator __first, _ForwardIterator __last,
                          _Size __count, const _Tp& __value, _BinaryPredicate __pred) 
{
  if (__count <= 0)
    return __first;
  while (true) {
    // Find first element in sequence that matchs __value, with a mininum of loop checks
    while (true) {
      if (__first == __last) { // return __last if no element matches __value
        return __first;
      }
      if (__pred(*__first, __value))
        break;
      ++__first;
    }
    // *__first matches __value, now match elements after here
    _ForwardIterator __m = __first;
    size_t __c(0);
    while (true) {
      if (++__c == __count) // If pattern exhausted, __first is the answer (works for 1 element pattern)
        return __first;
      if (++__m == __last) { // Otherwise if source exhaused, pattern not found
        return __last;
      }

      // if there is a mismatch, restart with a new __first
      if (!__pred(*__m, __value))
      {
        __first = __m;
        ++__first;
        break;
      } // else there is a match, check next elements
    }
  }
}

template <class _ForwardIterator, class _Size, class _Tp>
_ForwardIterator search_n(_ForwardIterator __first, _ForwardIterator __last, _Size __count, const _Tp& __value) 
{
  return tinySTL::search_n(__first, __last, __count, __value, tinySTL::__equal_to{});
}


template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare>
_OutputIterator
merge(_InputIterator1 __first1, _InputIterator1 __last1,
      _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, _Compare __comp)
{
  for (; __first1 != __last1; ++__result) {
    if (__first2 == __last2) {
      return tinySTL::copy(__first1, __last1, __result);
    } if (__comp(*__first2, *__first1)) {
      *__result = *__first2;
      ++__first2;
    } else {
      *__result = *__first1;
      ++__first1;
    }
  }
  return tinySTL::copy(__first2, __last2, __result);
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator>
_OutputIterator
merge(_InputIterator1 __first1, _InputIterator1 __last1,
      _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result)
{
  return tinySTL::merge(__first1, __last1, __first2, __last2, __result, tinySTL::__less{});
}

template <class _ForwardIterator1, class _ForwardIterator2> 
void iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b) 
{
  tinySTL::swap(*__a, *__b);
}

template <class _BidirectionalIterator> void
reverse(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
  while (__first != __last)
  {
    if (__first == --__last)
      break;
    tinySTL::iter_swap(__first, __last);
    ++__first;
  }
}

template <RandomAccessIterator _BidirectionalIterator> void
reverse(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
  if (__first != __last) {
    for (; __first < --__last; ++__first) {
      tinySTL::iter_swap(__first, __last);
    }
  }
}


namespace __tiny_sort_inner {
const static int __threshold = 16;

template <class _RandomAccessIterator, class _Tp, class _Comp>
void __unguarded_linear_insert(_RandomAccessIterator __last, _Tp* __value, _Comp __comp) 
{
  _RandomAccessIterator __next = __last;
  --__next;
  while (__comp(*__value, *__next)) {
    *__last = tinySTL::move(*__next);
    __last = __next;
    --__next;
  }
  *__last = tinySTL::move(*__value);
} 

template <class _RandomAccessIterator, class _Comp>
void __linear_insert(_RandomAccessIterator __first, _RandomAccessIterator __last, _Comp __comp) 
{
  auto __val = tinySTL::move(*__last);
  if (__comp(__val, *__first)) {
    tinySTL::copy_backward(__first, __last, __last+1);
    *__first = tinySTL::move(__val);
  } else {
    __unguarded_linear_insert(__last, &__val, __comp);
  }
}

template <class _RandomAccessIterator, class _Comp>
void __insert_sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Comp __comp) 
{
  if (__first == __last) return; // zero elements.
  for (_RandomAccessIterator i = __first+1; i != __last; ++i) {
    __linear_insert(__first, i, __comp);
  }
}

template <class _Tp, class _Comp>
const _Tp& __median(const _Tp& a, const _Tp& b, const _Tp& c, _Comp __comp) {
  if (__comp(a, b)) {
    if (__comp(b, c)) return b;
    else if (__comp(a, c)) return c;
    else return a;
  } else if (__comp(a, c)) {
    return a;
  } else if (__comp(b, c)) {
    return c;
  } else {
    return b;
  }
}

template <class _RandomAccessIterator, class _Tp, class _Comp>
_RandomAccessIterator __unguarded_partition(_RandomAccessIterator __i, _RandomAccessIterator __j, 
                      const _Tp& __pivot, _Comp __comp)
{
  while (__i < __j) {
    do { ++__i; } while (__comp(*__i, __pivot));
    do { --__j; } while (__comp(__pivot, *__j));
    if (__i < __j) tinySTL::swap(*__i, *__j);
  }
  return __j + 1;
}

template <class _RandomAccessIterator, class _Size, class _Comp>
void __introsort_loop(_RandomAccessIterator __first, _RandomAccessIterator __last, _Size __depth_limit, _Comp __comp) 
{
  while (true) {
    auto __len = __last - __first;
    if (__len < 2) {
      return;
    }
    if (__len < __threshold) {
      __insert_sort(__first, __last, __comp);
      return;
    }
    if (__depth_limit == 0) {
      tinySTL::make_heap(__first, __last, __comp);
      tinySTL::sort_heap(__first, __last, __comp);
      return;
    } 
    --__depth_limit;
    using _Tp = iterator_traits<_RandomAccessIterator>::reference;
    _Tp __tmp = const_cast<_Tp>(__median(*__first, *(__first + (__last-1 - __first)/2), *(__last-1), __comp));
    tinySTL::swap(*__first, __tmp);
    _RandomAccessIterator __cut = __unguarded_partition(__first, __last, *__first, __comp);
    __introsort_loop(__cut, __last, __depth_limit, __comp);
    __last = __cut;
  }
}

template <class _Size>
_Size __lg(_Size __n) {
  _Size __k;
  for (__k = 0; __n > 1; __n >>= 1) ++__k;
  return __k;
} 
} // Namespace __tiny_sort_inner

template <class _RandomAccessIterator, class _Comp>
void sort(_RandomAccessIterator __first, _RandomAccessIterator __last, _Comp __comp) 
{
  if (__first != __last) {
    __tiny_sort_inner::__introsort_loop(__first, __last, __tiny_sort_inner::__lg(__last - __first)*2, __comp);
  }
}

template <RandomAccessIterator _RandomAccessIterator>
void sort(_RandomAccessIterator __first, _RandomAccessIterator __last) 
{
  tinySTL::sort(__first, __last, tinySTL::__less{});
}

template <class _ForwardIterator, class _Tp> _ForwardIterator
remove(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value)
{
  __first = tinySTL::find(__first, __last, __value);
  if (__first != __last)
  {
    _ForwardIterator __i = __first;
    while (++__i != __last)
    {
      if (!(*__i == __value))
      {
        *__first = tinySTL::move(*__i);
        ++__first;
      }
    }
  }
  return __first;
}

template <class _ForwardIterator, class _Predicate> _ForwardIterator
remove_if(_ForwardIterator __first, _ForwardIterator __last, _Predicate __pred)
{
  __first = tinySTL::find_if<_ForwardIterator, _Predicate&>(__first, __last, __pred);
  if (__first != __last)
  {
    _ForwardIterator __i = __first;
    while (++__i != __last)
    {
      if (!__pred(*__i))
      {
        *__first = tinySTL::move(*__i);
        ++__first;
      }
    }
  }
  return __first;
}


template <class _InputIterator, class _OutputIterator, class _Tp> _OutputIterator
remove_copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result, const _Tp& __value)
{
  for (; __first != __last; ++__first)
  {
    if (!(*__first == __value))
    {
      *__result = *__first;
      ++__result;
    }
  }
  return __result;
}


template <class _InputIterator, class _OutputIterator, class _Predicate> _OutputIterator
remove_copy_if(_InputIterator __first, _InputIterator __last, _OutputIterator __result, _Predicate __pred)
{
  for (; __first != __last; ++__first)
  {
    if (!__pred(*__first))
    {
      *__result = *__first;
      ++__result;
    }
  }
  return __result;
}

template <class _ForwardIterator, class _Tp> void
replace(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __old_value, const _Tp& __new_value)
{
  for (; __first != __last; ++__first)
    if (*__first == __old_value)
      *__first = __new_value;
}

template <class _InputIterator, class _OutputIterator, class _Tp> _OutputIterator
replace_copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result,
             const _Tp& __old_value, const _Tp& __new_value)
{
  for (; __first != __last; ++__first, (void) ++__result)
    if (*__first == __old_value)
      *__result = __new_value;
    else
      *__result = *__first;
  return __result;
}

template <class _ForwardIterator, class _Predicate, class _Tp> void
replace_if(_ForwardIterator __first, _ForwardIterator __last, _Predicate __pred, const _Tp& __new_value)
{
  for (; __first != __last; ++__first)
    if (__pred(*__first))
      *__first = __new_value;
}

template <class _InputIterator, class _OutputIterator, class _Predicate, class _Tp> 
_OutputIterator
replace_copy_if(_InputIterator __first, _InputIterator __last, _OutputIterator __result,
                _Predicate __pred, const _Tp& __new_value)
{
  for (; __first != __last; ++__first, (void) ++__result)
    if (__pred(*__first))
      *__result = __new_value;
    else
      *__result = *__first;
  return __result;
}

template <class _ForwardIterator, class _BinaryPredicate> _ForwardIterator
unique(_ForwardIterator __first, _ForwardIterator __last, _BinaryPredicate __pred) {
  __first = tinySTL::adjacent_find(__first, __last, __pred);
  if (__first != __last) {
    _ForwardIterator __i = __first;
    for (++__i; ++__i != __last;)
      if (!__pred(*__first, *__i))
        *++__first = tinySTL::move(*__i);
    ++__first;
  }
  return __first;
}

template <class _ForwardIterator> _ForwardIterator
unique(_ForwardIterator __first, _ForwardIterator __last) {
  return tinySTL::unique(__first, __last, tinySTL::__equal_to{});
}

template <class _InputIterator, class _OutputIterator, class _BinaryPredicate> _OutputIterator
unique_copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result, _BinaryPredicate __pred) 
{
  if (__first != __last) {
    *__result = *__first;
    while (++__first != __last)
      if (!__pred(*__result, *__first))
        *++__result = *__first;
    ++__result;
  }
  return __result;
}

template <class _InputIterator, class _OutputIterator> _OutputIterator
unique_copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result) {
  return tinySTL::unique_copy(tinySTL::move(__first), 
    tinySTL::move(__last), tinySTL::move(__result), tinySTL::__equal_to{});
}

template <class _InputIterator, class _Function> _Function 
for_each(_InputIterator __first, _InputIterator __last, _Function __f) 
{
  for (; __first != __last; ++__first)
    __f(*__first);
  return __f;
}

template <class _ForwardIterator, class _Generator> void
generate(_ForwardIterator __first, _ForwardIterator __last, _Generator __gen)
{
  for (; __first != __last; ++__first)
    *__first = __gen();
}

template <class _OutputIterator, class _Size, class _Generator> _OutputIterator
generate_n(_OutputIterator __first, _Size __n, _Generator __gen)
{
  for (; __n > 0; ++__first, (void) --__n)
      *__first = __gen();
  return __first;
}

template <class _InputIterator, class _OutputIterator, class _UnaryOperation> _OutputIterator
transform(_InputIterator __first, _InputIterator __last, _OutputIterator __result, _UnaryOperation __op)
{
  for (; __first != __last; ++__first, (void) ++__result)
    *__result = __op(*__first);
  return __result;
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _BinaryOperation>
_OutputIterator
transform(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2,
          _OutputIterator __result, _BinaryOperation __binary_op)
{
  for (; __first1 != __last1; ++__first1, (void) ++__first2, ++__result)
    *__result = __binary_op(*__first1, *__first2);
  return __result;
}

template <ForwardIterator _ForwardIterator, class _Compare> _ForwardIterator
max_element(_ForwardIterator __first, _ForwardIterator __last, _Compare __comp)
{
  if (__first != __last)
  {
    _ForwardIterator __i = __first;
    while (++__i != __last)
      if (__comp(*__first, *__i))
        __first = __i;
  }
  return __first;
}

template <ForwardIterator _ForwardIterator, class _Compare> _ForwardIterator
min_element(_ForwardIterator __first, _ForwardIterator __last, _Compare __comp)
{
  if (__first != __last)
  {
    _ForwardIterator __i = __first;
    while (++__i != __last)
      if (__comp(*__i, *__first))
        __first = __i;
  }
  return __first;
}

template <ForwardIterator _ForwardIterator> _ForwardIterator
max_element(_ForwardIterator __first, _ForwardIterator __last)
{
  return tinySTL::max_element(__first, __last, tinySTL::__less{});
}

template <ForwardIterator _ForwardIterator> _ForwardIterator
min_element(_ForwardIterator __first, _ForwardIterator __last)
{
  return tinySTL::min_element(__first, __last, tinySTL::__less{});
}

template <class _InputIterator1, class _InputIterator2, class _Compare> bool
includes(_InputIterator1 __first1, _InputIterator1 __last1, 
         _InputIterator2 __first2, _InputIterator2 __last2, _Compare __comp) 
{
  for (; __first2 != __last2; ++__first1) {
    if (__first1 == __last1 || __comp(*__first2, *__first1))
      return false;
    if (!__comp(*__first1, *__first2))
      ++__first2;
  }
  return true;
}

template <class _InputIterator1, class _InputIterator2> bool
includes(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _InputIterator2 __last2) 
{
  return tinySTL::includes(tinySTL::move(__first1), tinySTL::move(__last1), 
                           tinySTL::move(__first2), tinySTL::move(__last2), tinySTL::__less{});
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare>
_OutputIterator set_union(_InputIterator1 __first1, _InputIterator1 __last1,
    _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, _Compare __comp) 
{
  for (; __first1 != __last1; ++__result) {
    if (__first2 == __last2) {
      return tinySTL::copy(tinySTL::move(__first1), tinySTL::move(__last1), tinySTL::move(__result));
    }
    if (__comp(*__first2, *__first1)) {
      *__result = *__first2;
      ++__first2;
    } else {
      if (!__comp(*__first1, *__first2)) {
        ++__first2;
      }
      *__result = *__first1;
      ++__first1;
    }
  }
  return tinySTL::copy(tinySTL::move(__first2), tinySTL::move(__last2), tinySTL::move(__result));
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator>
_OutputIterator set_union(_InputIterator1 __first1, _InputIterator1 __last1,
    _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result) 
{
  return tinySTL::set_union(tinySTL::move(__first1), tinySTL::move(__last1),
      tinySTL::move(__first2), tinySTL::move(__last2), tinySTL::move(__result), tinySTL::__less());
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare>
_OutputIterator set_intersection(_InputIterator1 __first1, _InputIterator1 __last1,
    _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, _Compare __comp)
{
  while (__first1 != __last1 && __first2 != __last2) {
    if (__comp(*__first1, *__first2))
      ++__first1;
    else {
      if (!__comp(*__first2, *__first1)) {
        *__result = *__first1;
        ++__result;
        ++__first1;
      }
      ++__first2;
    }
  }
  return __result;
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator>
_OutputIterator set_intersection(_InputIterator1 __first1, _InputIterator1 __last1,
    _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result) 
{
  return tinySTL::set_intersection(tinySTL::move(__first1), tinySTL::move(__last1),
    tinySTL::move(__first2), tinySTL::move(__last2), tinySTL::move(__result), tinySTL::__less{});
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare>
_OutputIterator set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
    _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, _Compare __comp) 
{
  while (__first1 != __last1 && __first2 != __last2) {
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__first1;
      ++__result;
    } else if (__comp(*__first2, *__first1)) {
      ++__first2;
    } else {
      ++__first1;
      ++__first2;
    }
  }
  return tinySTL::copy(tinySTL::move(__first1), tinySTL::move(__last1), tinySTL::move(__result));
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator>
_OutputIterator set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
    _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result)  {
  return tinySTL::set_difference(__first1, __last1, __first2, __last2, __result, tinySTL::__less{});
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator, class _Compare>
_OutputIterator set_symmetric_difference(_InputIterator1 __first1, _InputIterator1 __last1,
    _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result, _Compare __comp)  {
  while (__first1 != __last1) {
    if (__first2 == __last2) {
      return tinySTL::copy(tinySTL::move(__first1), tinySTL::move(__last1), tinySTL::move(__result));
    }
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__result;
      ++__first1;
    } else {
      if (__comp(*__first2, *__first1)) {
        *__result = *__first2;
        ++__result;
      } else {
        ++__first1;
      }
      ++__first2;
    }
  }
  return tinySTL::copy(tinySTL::move(__first2), tinySTL::move(__last2), tinySTL::move(__result));
}

template <class _InputIterator1, class _InputIterator2, class _OutputIterator>
_OutputIterator set_symmetric_difference(_InputIterator1 __first1, _InputIterator1 __last1,
    _InputIterator2 __first2, _InputIterator2 __last2, _OutputIterator __result) {
  return tinySTL::set_symmetric_difference(tinySTL::move(__first1),
      tinySTL::move(__last1), tinySTL::move(__first2), tinySTL::move(__last2),
      tinySTL::move(__result), tinySTL::__less{});
}

}