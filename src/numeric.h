#pragma once
#include "tiny_iterator.h"

namespace tinySTL
{

template <class _InputIterator, class _Tp> _Tp
accumulate(_InputIterator __first, _InputIterator __last, _Tp __init)
{
  for (; __first != __last; ++__first)
    __init = __init + *__first;
  return __init;
}

template <class _InputIterator, class _Tp, class _BinaryOperation> _Tp
accumulate(_InputIterator __first, _InputIterator __last, _Tp __init, _BinaryOperation __binary_op)
{
  for (; __first != __last; ++__first)
    __init = __binary_op(__init, *__first);
  return __init;
}

template <class _InputIterator, class _OutputIterator> _OutputIterator
partial_sum(_InputIterator __first, _InputIterator __last, _OutputIterator __result)
{
  if (__first != __last)
  {
    typename iterator_traits<_InputIterator>::value_type __t(*__first);
    *__result = __t;
    for (++__first, (void) ++__result; __first != __last; ++__first, (void) ++__result)
    {
      __t = __t + *__first;
      *__result = __t;
    }
  }
  return __result;
}

template <class _InputIterator, class _OutputIterator, class _BinaryOperation> _OutputIterator
partial_sum(_InputIterator __first, _InputIterator __last, _OutputIterator __result,
              _BinaryOperation __binary_op)
{
  if (__first != __last)
  {
    typename iterator_traits<_InputIterator>::value_type __t(*__first);
    *__result = __t;
    for (++__first, (void) ++__result; __first != __last; ++__first, (void) ++__result)
    {
      __t = __binary_op(__t, *__first);
      *__result = __t;
    }
  }
  return __result;
}


template <class _InputIterator1, class _InputIterator2, class _Tp> _Tp
inner_product(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2, _Tp __init)
{
  for (; __first1 != __last1; ++__first1, (void) ++__first2)
    __init = __init + *__first1 * *__first2;
  return __init;
}

template <class _InputIterator1, class _InputIterator2, class _Tp, class _BinaryOperation1, class _BinaryOperation2>
_Tp
inner_product(_InputIterator1 __first1, _InputIterator1 __last1, _InputIterator2 __first2,
              _Tp __init, _BinaryOperation1 __binary_op1, _BinaryOperation2 __binary_op2)
{
    for (; __first1 != __last1; ++__first1, (void) ++__first2)
        __init = __binary_op1(__init, __binary_op2(*__first1, *__first2));
    return __init;
}

template <class _InputIterator, class _OutputIterator> _OutputIterator
adjacent_difference(_InputIterator __first, _InputIterator __last, _OutputIterator __result)
{
  if (__first != __last)
  {
    typename iterator_traits<_InputIterator>::value_type __acc(*__first);
    *__result = __acc;
    for (++__first, (void) ++__result; __first != __last; ++__first, (void) ++__result)
    {
      typename iterator_traits<_InputIterator>::value_type __val(*__first);
      *__result = __val - __acc;
      __acc = tinySTL::move(__val);
    }
  }
  return __result;
}

template <class _InputIterator, class _OutputIterator, class _BinaryOperation>
_OutputIterator
adjacent_difference(_InputIterator __first, _InputIterator __last, _OutputIterator __result,
                    _BinaryOperation __binary_op)
{
  if (__first != __last)
  {
    typename iterator_traits<_InputIterator>::value_type __acc(*__first);
    *__result = __acc;
    for (++__first, (void) ++__result; __first != __last; ++__first, (void) ++__result)
    {
      typename iterator_traits<_InputIterator>::value_type __val(*__first);
      *__result = __binary_op(__val, __acc);
      __acc = tinySTL::move(__val);
    }
  }
  return __result;
}

}
