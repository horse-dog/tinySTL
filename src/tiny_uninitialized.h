#pragma once

#include <string.h>

#include "tiny_pair.h"
#include "tiny_traits.h"
#include "tiny_algobase.h"
#include "tiny_construct.h"
#include "tiny_iterator_base.h"

namespace tinySTL 
{

////////////////////////////////////////////////////////////////////////////
// forward declaration.
////////////////////////////////////////////////////////////////////////////

template <class _InputIter, class _ForwardIter>
inline _ForwardIter
uninitialized_copy(_InputIter __first, _InputIter __last,
                   _ForwardIter __result);


////////////////////////////////////////////////////////////////////////////
// aux function.
////////////////////////////////////////////////////////////////////////////

template <class _InputIter, class _Size, class _ForwardIter>
pair<_InputIter, _ForwardIter>
__uninitialized_copy_n(_InputIter __first, _Size __count,
                       _ForwardIter __result,
                       input_iterator_tag)
{
  _ForwardIter __cur = __result;
  try {
    for ( ; __count > 0 ; --__count, ++__first, ++__cur) 
      tinySTL::_Construct(&*__cur, *__first);
    return pair<_InputIter, _ForwardIter>(__first, __cur);
  }
  catch (...) {
    tinySTL::_Destroy(__result, __cur);
    throw;
  }
}

template <class _RandomAccessIter, class _Size, class _ForwardIter>
inline pair<_RandomAccessIter, _ForwardIter>
__uninitialized_copy_n(_RandomAccessIter __first, _Size __count,
                       _ForwardIter __result,
                       random_access_iterator_tag) {
  _RandomAccessIter __last = __first + __count;
  return pair<_RandomAccessIter, _ForwardIter>(
                 __last,
                 tinySTL::uninitialized_copy(__first, __last, __result)
                );
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter 
__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                         _ForwardIter __result, true_type)
{
  return tinySTL::copy(__first, __last, __result);
}

template <class _InputIter, class _ForwardIter>
_ForwardIter 
__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                         _ForwardIter __result, false_type)
{
  _ForwardIter __cur = __result;
  try 
  {
    for ( ; __first != __last; ++__first, ++__cur)
      tinySTL::_Construct(&*__cur, *__first);
    return __cur;
  } 
  catch(...)
  { 
    tinySTL::_Destroy(__result, __cur);
    throw;
  }
}

template <class _ForwardIter1, class _ForwardIter2>
inline _ForwardIter2 
__uninitialized_move_aux(_ForwardIter1 __first, _ForwardIter1 __last,
                         _ForwardIter2 __result, true_type)
{
  return tinySTL::move(__first, __last, __result);
}

template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter2 
__uninitialized_move_aux(_ForwardIter1 __first, _ForwardIter1 __last,
                         _ForwardIter2 __result, false_type)
{
  _ForwardIter2 __cur = __result;
  try 
  {
    for ( ; __first != __last; ++__first, ++__cur)
      tinySTL::_Construct(&*__cur, tinySTL::move(*__first));
    return __cur;
  } 
  catch(...)
  { 
    tinySTL::_Destroy(__result, __cur);
    throw;
  }
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter
__uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
                           const _Tp& __x, true_type)
{
  return tinySTL::fill_n(__first, __n, __x);
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter
__uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
                           const _Tp& __x, false_type)
{
  _ForwardIter __cur = __first;
  try {
    for ( ; __n > 0; --__n, ++__cur)
      tinySTL::_Construct(&*__cur, __x);
    return __cur;
  }
  catch (...) {
    tinySTL::_Destroy(__first, __cur);
    throw;
  }
}

template <class _ForwardIter, class _Tp>
inline void
__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last, 
                         const _Tp& __x, true_type)
{
  tinySTL::fill(__first, __last, __x);
}

template <class _ForwardIter, class _Tp> void
__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last, 
                         const _Tp& __x, false_type)
{
  _ForwardIter __cur = __first;
  try {
    for ( ; __cur != __last; ++__cur)
      tinySTL::_Construct(&*__cur, __x);
  }
  catch (...) {
    tinySTL::_Destroy(__first, __cur);
    throw;
  }
}


////////////////////////////////////////////////////////////////////////////
// dispatcher.
////////////////////////////////////////////////////////////////////////////

template <class _InputIter, class _Size, class _ForwardIter>
inline pair<_InputIter, _ForwardIter>
__uninitialized_copy_n(_InputIter __first, _Size __count,
                       _ForwardIter __result) {
  return tinySTL::__uninitialized_copy_n(__first, __count, __result,
                                         iterator_category(__first));
}

template <class _InputIter, class _ForwardIter, class _Tp>
inline _ForwardIter
__uninitialized_copy(_InputIter __first, _InputIter __last,
                     _ForwardIter __result, _Tp*)
{
  typedef typename type_traits<_Tp>::is_POD_type _Is_POD;
  return tinySTL::__uninitialized_copy_aux(__first, __last, __result, _Is_POD());
}

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline _ForwardIter2
__uninitialized_move(_ForwardIter1 __first, _ForwardIter1 __last,
                     _ForwardIter2 __result, _Tp*)
{
  typedef typename type_traits<_Tp>::is_POD_type _Is_POD;
  return tinySTL::__uninitialized_move_aux(__first, __last, __result, _Is_POD());
}

template <class _ForwardIter, class _Size, class _Tp, class _Tp1>
inline _ForwardIter 
__uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, _Tp1*)
{
  typedef typename type_traits<_Tp1>::is_POD_type _Is_POD;
  return tinySTL::__uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
}

template <class _ForwardIter, class _Tp, class _Tp1>
inline void __uninitialized_fill(_ForwardIter __first, 
                                 _ForwardIter __last, const _Tp& __x, _Tp1*)
{
  typedef typename type_traits<_Tp1>::is_POD_type _Is_POD;
  tinySTL::__uninitialized_fill_aux(__first, __last, __x, _Is_POD());
                   
}


////////////////////////////////////////////////////////////////////////////
// basic interface.
////////////////////////////////////////////////////////////////////////////

template <class _InputIter, class _Size, class _ForwardIter>
inline pair<_InputIter, _ForwardIter>
uninitialized_copy_n(_InputIter __first, _Size __count,
                     _ForwardIter __result) {
  return tinySTL::__uninitialized_copy_n(__first, __count, __result,
                                         iterator_category(__first));
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter
uninitialized_copy(_InputIter __first, _InputIter __last,
                   _ForwardIter __result) 
{
  return tinySTL::__uninitialized_copy(__first, __last, __result,
                                       value_type(__result));
}

template <class _ForwardIter1, class _ForwardIter2>
inline _ForwardIter2
uninitialized_move(_ForwardIter1 __first, _ForwardIter1 __last,
                   _ForwardIter2 __result) 
{
  return tinySTL::__uninitialized_move(__first, __last, __result,
                                       value_type(__result));
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter 
uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
  return tinySTL::__uninitialized_fill_n(__first, __n, __x, value_type(__first));
}

template <class _ForwardIter, class _Tp>
inline void uninitialized_fill(_ForwardIter __first,
                               _ForwardIter __last, 
                               const _Tp& __x)
{
  tinySTL::__uninitialized_fill(__first, __last, __x, value_type(__first));
}


////////////////////////////////////////////////////////////////////////////
// specific version.
////////////////////////////////////////////////////////////////////////////

inline char* uninitialized_copy(const char* __first, const char* __last,
                                char* __result) {
  memmove(__result, __first, __last - __first);
  return __result + (__last - __first);
}

inline wchar_t* 
uninitialized_copy(const wchar_t* __first, const wchar_t* __last,
                   wchar_t* __result)
{
  memmove(__result, __first, sizeof(wchar_t) * (__last - __first));
  return __result + (__last - __first);
}

////////////////////////////////////////////////////////////////////////////
// extensions functions.
////////////////////////////////////////////////////////////////////////////

// __uninitialized_copy_copy
// copies [first1, last1) into [result, result + (last1 - first1)), and
// copies [first2, last2) into [result, result + (last1 - first1) + (last2 - first2)).
template <class _InputIter1, class _InputIter2, class _ForwardIter>
inline _ForwardIter
__uninitialized_copy_copy(_InputIter1 __first1, _InputIter1 __last1,
                          _InputIter2 __first2, _InputIter2 __last2,
                          _ForwardIter __result)
{
  _ForwardIter __mid = tinySTL::uninitialized_copy(__first1, __last1, __result);
  try {
    return tinySTL::uninitialized_copy(__first2, __last2, __mid);
  } catch (...) {
    tinySTL::_Destroy(__result, __mid);
    throw;
  }
}

// __uninitialized_fill_copy
// Fills [result, mid) with x, and copies [first, last) into
// [mid, mid + (last - first)).
template <class _ForwardIter, class _Tp, class _InputIter>
inline _ForwardIter 
__uninitialized_fill_copy(_ForwardIter __result, _ForwardIter __mid,
                          const _Tp& __x,
                          _InputIter __first, _InputIter __last)
{
  tinySTL::uninitialized_fill(__result, __mid, __x);
  try {
    return tinySTL::uninitialized_copy(__first, __last, __mid);
  } catch (...) {
    tinySTL::_Destroy(__result, __mid);
    throw;
  }
}

// __uninitialized_copy_fill
// Copies [first1, last1) into [first2, first2 + (last1 - first1)), and
//  fills [first2 + (last1 - first1), last2) with x.
template <class _InputIter, class _ForwardIter, class _Tp>
inline void
__uninitialized_copy_fill(_InputIter __first1, _InputIter __last1,
                          _ForwardIter __first2, _ForwardIter __last2,
                          const _Tp& __x)
{
  _ForwardIter __mid2 = tinySTL::uninitialized_copy(__first1, __last1, __first2);
  try {
    tinySTL::uninitialized_fill(__mid2, __last2, __x);
  } catch (...) {
    tinySTL::_Destroy(__first2, __mid2);
    throw;
  }
}

}