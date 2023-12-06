// construct & destroy.

#pragma once
#include "tiny_traits.h"
#include "tiny_iterator_base.h"

namespace tinySTL 
{

template <class _T1, class... _Args>
inline void _Construct(_T1* __p, _Args&& ...__args) {
  ::new ((void*) __p) _T1(tinySTL::forward<_Args>(__args)...);
}

// Single destory.
template <class _Tp>
inline void _Destroy(_Tp* __pointer) {
  __pointer->~_Tp();
}

template <class _ForwardIterator>
void
__destroy_aux(_ForwardIterator __first, _ForwardIterator __last, false_type)
{
  for ( ; __first != __last; ++__first)
    tinySTL::_Destroy(&*__first);
}

template <class _ForwardIterator> 
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, true_type) {}

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
  typedef typename type_traits<_Tp>::has_trivial_destructor
          _Trivial_destructor;
  // _Trivial_destructor is true_type or false_type.
  tinySTL::__destroy_aux(__first, __last, _Trivial_destructor());
}

// Range destory [first, last).
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) {
  tinySTL::__destroy(__first, __last, value_type(__first));
}

template <class _ForwardIterator>
requires tinySTL::is_arithmetic_v<_ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) {}


template <class _T1, class... _Args>
inline void construct(_T1* __p, _Args&& ...__args) {
  tinySTL::_Construct(__p, tinySTL::forward<_Args>(__args)...);
}

template <class _Tp>
inline void destroy(_Tp* __pointer) {
  tinySTL::_Destroy(__pointer);
}

template <class _ForwardIterator>
inline void destroy(_ForwardIterator __first, _ForwardIterator __last) {
  tinySTL::_Destroy(__first, __last);
}

}