#pragma once

#include "tiny_traits.h"
#include "tiny_iterator_base.h"

namespace tinySTL
{

// same_as.
template <class _Tp, class _Ty>
concept same_as = is_same_v<_Tp, _Ty> && is_same_v<_Ty, _Tp>;


// iterator concept.
template <class _Tp>
concept __Iterable = requires(_Tp iter) {
  { is_pointer_v<typename _Tp::iterator_category> };
  { is_pointer_v<typename _Tp::value_type> };
  { is_pointer_v<typename _Tp::difference_type> };
  { is_pointer_v<typename _Tp::pointer> };
  { is_pointer_v<typename _Tp::reference> };

  { iter.operator* () } -> same_as<typename _Tp::reference>;
  { iter.operator->() } -> same_as<typename _Tp::pointer>;
};


template <class _Tp>
concept __InputIterator = 
  is_base_of_v<input_iterator_tag, typename _Tp::iterator_category> 
  && __Iterable<_Tp>;



template <class _Tp>
concept __OutputIterator = 
  is_base_of_v<output_iterator_tag, typename _Tp::iterator_category> 
  && __Iterable<_Tp>;


template <class _Tp>
concept __ForwardIterator = requires(_Tp iter) {
  { (++iter)++ };
} && is_base_of_v<forward_iterator_tag, typename _Tp::iterator_category> 
  && __Iterable<_Tp>;


template <class _Tp>
concept __BidirectionalIterator = requires(_Tp iter) {
  { (--iter)-- };
} && is_base_of_v<bidirectional_iterator_tag, typename _Tp::iterator_category> 
  && __ForwardIterator<_Tp>;


template <class _Tp>
concept __RandomAccessIterator = requires(_Tp iter) {
  { iter +  0  };
  { iter -  0  };
  { iter += 0  };
  { iter -= 0  };
  { iter   [0] };
} && is_base_of_v<random_access_iterator_tag, typename _Tp::iterator_category>
  && __BidirectionalIterator<_Tp>;


template <class _Tp>
concept Iterable = __Iterable<_Tp> || is_pointer<_Tp>::value;

template <class _Tp>
concept InputIterator = __InputIterator<_Tp> || is_pointer<_Tp>::value;

template <class _Tp>
concept OutputIterator = __OutputIterator<_Tp> 
  || (is_pointer<_Tp>::value && !is_const<_Tp>::value);

template <class _Tp>
concept ForwardIterator = __ForwardIterator<_Tp>
  || (is_pointer<_Tp>::value && !is_const<_Tp>::value);

template <class _Tp>
concept BidirectionalIterator = __BidirectionalIterator<_Tp>
  || (is_pointer<_Tp>::value && !is_const<_Tp>::value);

template <class _Tp>
concept RandomAccessIterator = __RandomAccessIterator<_Tp>
  || (is_pointer<_Tp>::value && !is_const<_Tp>::value);

}