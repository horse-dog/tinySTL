#pragma once

#include <string>
#include <sstream>
#include <string.h>

#include "tiny_pair.h"
#include "tiny_concepts.h"

namespace tinySTL
{

////////////////////////////////////////////////////////////////////////////
// to_string.
////////////////////////////////////////////////////////////////////////////

template <class container>
std::string to_string(const container &__x) {
  std::string result = "[";
  for (const auto &item : __x) {
    std::ostringstream os;
    os << item;
    result += std::string(os.str()) + ", ";
  }
  result = result.substr(0, result.size() - 2) + ']';
  return result;
}


////////////////////////////////////////////////////////////////////////////
// swap.
////////////////////////////////////////////////////////////////////////////

template <class _Tp>
inline void swap(_Tp& __a, _Tp& __b) {
  _Tp __tmp = __a;
  __a = __b;
  __b = __tmp;
}

////////////////////////////////////////////////////////////////////////////
// fill.
////////////////////////////////////////////////////////////////////////////

template <Iterable _ForwardIter, class _Tp>
void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __value) {
  for ( ; __first != __last; ++__first)
    *__first = __value;
}

inline void fill(unsigned char* __first, unsigned char* __last,
                 const unsigned char& __c) {
  unsigned char __tmp = __c;
  memset(__first, __tmp, __last - __first);
}

inline void fill(signed char* __first, signed char* __last,
                 const signed char& __c) {
  signed char __tmp = __c;
  memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}

inline void fill(char* __first, char* __last, const char& __c) {
  char __tmp = __c;
  memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}

////////////////////////////////////////////////////////////////////////////
// fill_n.
////////////////////////////////////////////////////////////////////////////

template <Iterable _OutputIterator, class _Size, class _Tp>
_OutputIterator fill_n(_OutputIterator __first, _Size __n, const _Tp& __value) {
  for ( ; __n > 0; --__n, ++__first)
    *__first = __value;
  return __first;
}

template <class _Size>
inline unsigned char* fill_n(unsigned char* __first, _Size __n,
                             const unsigned char& __c) {
  fill(__first, __first + __n, __c);
  return __first + __n;
}

template <class _Size>
inline signed char* fill_n(char* __first, _Size __n,
                           const signed char& __c) {
  fill(__first, __first + __n, __c);
  return __first + __n;
}

template <class _Size>
inline char* fill_n(char* __first, _Size __n, const char& __c) {
  fill(__first, __first + __n, __c);
  return __first + __n;
}


////////////////////////////////////////////////////////////////////////////
// copy.
////////////////////////////////////////////////////////////////////////////

template <class _InputIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result,
                          input_iterator_tag, _Distance*)
{
  for ( ; __first != __last; ++__result, ++__first)
    *__result = *__first;
  return __result;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter
__copy(_RandomAccessIter __first, _RandomAccessIter __last,
       _OutputIter __result, random_access_iterator_tag, _Distance*)
{
  for (_Distance __n = __last - __first; __n > 0; --__n) {
    *__result = *__first;
    ++__first;
    ++__result;
  }
  return __result;
}

template <class _Tp>
inline _Tp*
__copy_trivial(const _Tp* __first, const _Tp* __last, _Tp* __result) {
  memmove(__result, __first, sizeof(_Tp) * (__last - __first));
  return __result + (__last - __first);
}

template <class _InputIter, class _OutputIter, class _BoolType>
struct __copy_dispatch {
  static _OutputIter copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result) {
    typedef typename iterator_traits<_InputIter>::iterator_category _Category;
    typedef typename iterator_traits<_InputIter>::difference_type _Distance;
    return tinySTL::__copy(__first, __last, __result, _Category(), (_Distance*) 0);
  }
};

template <class _Tp>
struct __copy_dispatch<_Tp*, _Tp*, true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    return tinySTL::__copy_trivial(__first, __last, __result);
  }
};

template <class _Tp>
struct __copy_dispatch<const _Tp*, _Tp*, true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    return tinySTL::__copy_trivial(__first, __last, __result);
  }
};

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last,
                        _OutputIter __result) {
  typedef typename iterator_traits<_OutputIter>::value_type _Tp;
  typedef typename type_traits<_Tp>::has_trivial_assignment_operator
                   _Trivial;
  return __copy_dispatch<_InputIter, _OutputIter, _Trivial>
    ::copy(__first, __last, __result);
}


////////////////////////////////////////////////////////////////////////////
// move.
////////////////////////////////////////////////////////////////////////////

template <class _ForwardIter1, class _ForwardIter2, class _Distance>
inline _ForwardIter2 __move(_ForwardIter1 __first, _ForwardIter1 __last,
                            _ForwardIter2 __result,
                            input_iterator_tag, _Distance*)
{
  for ( ; __first != __last; ++__result, ++__first)
    *__result = tinySTL::move(*__first);
  return __result;
}

template <class _RandomAccessIter, class _ForwardIter, class _Distance>
inline _ForwardIter
__move(_RandomAccessIter __first, _RandomAccessIter __last,
       _ForwardIter __result, random_access_iterator_tag, _Distance*)
{
  for (_Distance __n = __last - __first; __n > 0; --__n) {
    *__result = tinySTL::move(*__first);
    ++__first;
    ++__result;
  }
  return __result;
}

template <class _ForwardIter1, class _ForwardIter2, class _BoolType>
struct __move_dispatch {
  static _ForwardIter2 move(_ForwardIter1 __first, _ForwardIter1 __last,
                            _ForwardIter2 __result) {
    typedef typename iterator_traits<_ForwardIter1>::iterator_category _Category;
    typedef typename iterator_traits<_ForwardIter1>::difference_type _Distance;
    return tinySTL::__move(__first, __last, __result, _Category(), (_Distance*) 0);
  }
};

template <class _Tp>
struct __move_dispatch<_Tp*, _Tp*, true_type>
{
  static _Tp* move(_Tp* __first, _Tp* __last, _Tp* __result) {
    return tinySTL::__copy_trivial(__first, __last, __result);
  }
};

template <class _ForwardIter1, class _ForwardIter2>
inline _ForwardIter2 move(_ForwardIter1 __first, _ForwardIter1 __last,
                          _ForwardIter2 __result) {
  typedef typename iterator_traits<_ForwardIter2>::value_type _Tp;
  typedef typename type_traits<_Tp>::has_trivial_move_operator
                   _Trivial;
  return __move_dispatch<_ForwardIter1, _ForwardIter2, _Trivial>
    ::move(__first, __last, __result);
}


////////////////////////////////////////////////////////////////////////////
// copy_n.
////////////////////////////////////////////////////////////////////////////


template <class _InputIter, class _Size, class _OutputIter>
pair<_InputIter, _OutputIter> __copy_n(_InputIter __first, _Size __count,
                                       _OutputIter __result,
                                       input_iterator_tag) {
  for ( ; __count > 0; --__count) {
    *__result = *__first;
    ++__first;
    ++__result;
  }
  return pair<_InputIter, _OutputIter>(__first, __result);
}

template <class _RAIter, class _Size, class _OutputIter>
inline pair<_RAIter, _OutputIter>
__copy_n(_RAIter __first, _Size __count,
         _OutputIter __result,
         random_access_iterator_tag) {
  _RAIter __last = __first + __count;
  return pair<_RAIter, _OutputIter>(__last, tinySTL::copy(__first, __last, __result));
}

template <class _InputIter, class _Size, class _OutputIter>
inline pair<_InputIter, _OutputIter>
__copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
  return tinySTL::__copy_n(__first, __count, __result,
                           iterator_category(__first));
}

template <class _InputIter, class _Size, class _OutputIter>
inline pair<_InputIter, _OutputIter>
copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
  return tinySTL::__copy_n(__first, __count, __result);
}


////////////////////////////////////////////////////////////////////////////
// copy_backward.
////////////////////////////////////////////////////////////////////////////

template <class _BidirectionalIter1, class _BidirectionalIter2, 
          class _Distance>
inline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, 
                                           _BidirectionalIter1 __last, 
                                           _BidirectionalIter2 __result,
                                           bidirectional_iterator_tag,
                                           _Distance*)
{
  while (__first != __last)
    *--__result = *--__last;
  return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, 
                                          _RandomAccessIter __last, 
                                          _BidirectionalIter __result,
                                          random_access_iterator_tag,
                                          _Distance*)
{
  for (_Distance __n = __last - __first; __n > 0; --__n)
    *--__result = *--__last;
  return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BoolType>
struct __copy_backward_dispatch
{
  typedef typename iterator_traits<_BidirectionalIter1>::iterator_category 
          _Cat;
  typedef typename iterator_traits<_BidirectionalIter1>::difference_type
          _Distance;

  static _BidirectionalIter2 copy(_BidirectionalIter1 __first, 
                                  _BidirectionalIter1 __last, 
                                  _BidirectionalIter2 __result) 
  {
    return tinySTL::__copy_backward(__first, __last, __result, _Cat(), (_Distance*) 0);
  }
};

template <class _Tp>
struct __copy_backward_dispatch<_Tp*, _Tp*, true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    const ptrdiff_t _Num = __last - __first;
    memmove(__result - _Num, __first, sizeof(_Tp) * _Num);
    return __result - _Num;
  }
};

template <class _Tp>
struct __copy_backward_dispatch<const _Tp*, _Tp*, true_type>
{
  static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    return  __copy_backward_dispatch<_Tp*, _Tp*, true_type>
      ::copy(__first, __last, __result);
  }
};

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result) {
  typedef typename type_traits<typename iterator_traits<_BI2>::value_type>
                   ::has_trivial_assignment_operator
                   _Trivial;
  return __copy_backward_dispatch<_BI1, _BI2, _Trivial>
              ::copy(__first, __last, __result);
}


////////////////////////////////////////////////////////////////////////////
// move_backward.
////////////////////////////////////////////////////////////////////////////

template <class _BidirectionalIter1, class _BidirectionalIter2, 
          class _Distance>
inline _BidirectionalIter2 __move_backward(_BidirectionalIter1 __first, 
                                           _BidirectionalIter1 __last, 
                                           _BidirectionalIter2 __result,
                                           bidirectional_iterator_tag,
                                           _Distance*)
{
  while (__first != __last)
    *--__result = tinySTL::move(*--__last);
  return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __move_backward(_RandomAccessIter __first, 
                                          _RandomAccessIter __last, 
                                          _BidirectionalIter __result,
                                          random_access_iterator_tag,
                                          _Distance*)
{
  for (_Distance __n = __last - __first; __n > 0; --__n)
    *--__result = tinySTL::move(*--__last);
  return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BoolType>
struct __move_backward_dispatch
{
  typedef typename iterator_traits<_BidirectionalIter1>::iterator_category 
          _Cat;
  typedef typename iterator_traits<_BidirectionalIter1>::difference_type
          _Distance;

  static _BidirectionalIter2 move(_BidirectionalIter1 __first, 
                                  _BidirectionalIter1 __last, 
                                  _BidirectionalIter2 __result) 
  {
    return ::tinySTL::__move_backward (
      __first, __last, __result, _Cat(), (_Distance*) 0
    );
  }
};

template <class _Tp>
struct __move_backward_dispatch<_Tp*, _Tp*, true_type>
{
  static _Tp* move(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    const ptrdiff_t _Num = __last - __first;
    memmove(__result - _Num, __first, sizeof(_Tp) * _Num);
    return __result - _Num;
  }
};

template <class _BI1, class _BI2>
inline _BI2 move_backward(_BI1 __first, _BI1 __last, _BI2 __result) {
  typedef typename type_traits<typename iterator_traits<_BI2>::value_type>
                   ::has_trivial_move_operator
                   _Trivial;
  return __move_backward_dispatch<_BI1, _BI2, _Trivial>
              ::move(__first, __last, __result);
}


////////////////////////////////////////////////////////////////////////////
// min max.
////////////////////////////////////////////////////////////////////////////

template <class _Tp>
inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
  return __b < __a ? __b : __a;
}

template <class _Tp>
inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
  return  __a < __b ? __b : __a;
}

template <class _Tp, class _Compare>
inline const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __comp) {
  return __comp(__b, __a) ? __b : __a;
}

template <class _Tp, class _Compare>
inline const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __comp) {
  return __comp(__a, __b) ? __b : __a;
}


////////////////////////////////////////////////////////////////////////////
// equal.
////////////////////////////////////////////////////////////////////////////

template <Iterable _InputIter1, Iterable _InputIter2>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2) {
  for ( ; __first1 != __last1; ++__first1, ++__first2)
    if (*__first1 != *__first2)
      return false;
  return true;
}

template <Iterable _InputIter1, Iterable _InputIter2, class _BinaryPredicate>
inline bool equal(_InputIter1 __first1, _InputIter1 __last1,
                  _InputIter2 __first2, _BinaryPredicate __binary_pred) {
  for ( ; __first1 != __last1; ++__first1, ++__first2)
    if (!__binary_pred(*__first1, *__first2))
      return false;
  return true;
}


////////////////////////////////////////////////////////////////////////////
// lexicographical_compare.
////////////////////////////////////////////////////////////////////////////

template <Iterable _InputIter1, Iterable _InputIter2>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2) {
  for ( ; __first1 != __last1 && __first2 != __last2
        ; ++__first1, ++__first2) {
    if (*__first1 < *__first2)
      return true;
    if (*__first2 < *__first1)
      return false;
  }
  return __first1 == __last1 && __first2 != __last2;
}


template <Iterable _InputIter1, Iterable _InputIter2, class _Compare>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _Compare __comp) {
  for ( ; __first1 != __last1 && __first2 != __last2
        ; ++__first1, ++__first2) {
    if (__comp(*__first1, *__first2))
      return true;
    if (__comp(*__first2, *__first1))
      return false;
  }
  return __first1 == __last1 && __first2 != __last2;
}


inline bool 
lexicographical_compare(const unsigned char* __first1,
                        const unsigned char* __last1,
                        const unsigned char* __first2,
                        const unsigned char* __last2)
{
  const size_t __len1 = __last1 - __first1;
  const size_t __len2 = __last2 - __first2;
  const int __result = memcmp(__first1, __first2, min(__len1, __len2));
  return __result != 0 ? __result < 0 : __len1 < __len2;
}

inline bool lexicographical_compare(const char* __first1, const char* __last1,
                                    const char* __first2, const char* __last2)
{
// TODO: why #if?.
#if CHAR_MAX == SCHAR_MAX
  return lexicographical_compare((const signed char*) __first1,
                                 (const signed char*) __last1,
                                 (const signed char*) __first2,
                                 (const signed char*) __last2);
#else /* CHAR_MAX == SCHAR_MAX */
  return lexicographical_compare((const unsigned char*) __first1,
                                 (const unsigned char*) __last1,
                                 (const unsigned char*) __first2,
                                 (const unsigned char*) __last2);
#endif /* CHAR_MAX == SCHAR_MAX */
}

}