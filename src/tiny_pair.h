#pragma once
#include <tuple>
#include "tiny_traits.h"

namespace tinySTL 
{

template <class _T1, class _T2>
struct pair {
  typedef _T1 first_type;
  typedef _T2 second_type;

  _T1 first;
  _T2 second;

  pair() : first(_T1()), second(_T2()) {}

  pair(const pair&) = default;

  pair(pair&&) = default;

  pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}

  template <class _U1, class _U2>
  pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}

  template <class _U1, class _U2>
  pair(pair<_U1, _U2>&& __p) : first(tinySTL::move(__p.first)), second(tinySTL::move(__p.second)) {}

  pair& operator=(const pair& __p) = default;
  pair& operator=(pair&& __p) = default;

  template <class _U1, class _U2>
  pair& operator=(const pair<_U1, _U2>& __p) {
    if (this != &__p) {
      first = __p.first;
      second = __p.second;
    }
    return *this;
  }

  template <class _U1, class _U2>
  pair& operator=(pair<_U1, _U2>&& __p) {
    if (this != &__p) {
      first = tinySTL::move(__p.first);
      second = tinySTL::move(__p.second);
    }
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const pair& __pair) 
  {
    return os << '{' << __pair.first << ", " << __pair.second << '}';
  }
};

template <class _T1, class _T2>
inline bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ 
  return __x.first == __y.first && __x.second == __y.second; 
}

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ 
  return __x.first < __y.first || 
         (!(__y.first < __x.first) && __x.second < __y.second); 
}

template <class _T1, class _T2>
inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y)
{
  return pair<_T1, _T2>(__x, __y);
}

template <std::size_t I, class _T1_, class _T2_>
decltype(auto) get(tinySTL::pair<_T1_, _T2_>& p) 
{
  if constexpr (I == 0) return (p.first);
  else if constexpr (I == 1) return (p.second);
}

template <std::size_t I, class _T1_, class _T2_>
decltype(auto) get(const tinySTL::pair<_T1_, _T2_>& p) 
{
  if constexpr (I == 0) return (p.first);
  else if constexpr (I == 1) return (p.second);
}

template <std::size_t I, class _T1_, class _T2_>
decltype(auto) get(tinySTL::pair<_T1_, _T2_>&& p) 
{
  if constexpr (I == 0) return tinySTL::move(p.first);
  else if constexpr (I == 1) return tinySTL::move(p.second);
}

}

template <class _T1, class _T2>
struct std::tuple_size<tinySTL::pair<_T1, _T2>> : tinySTL::integral_constant<size_t, 2> {};

template <class _T1, class _T2>
struct std::tuple_element<0, tinySTL::pair<_T1, _T2>> {
  using type = _T1;
};

template <class _T1, class _T2>
struct std::tuple_element<1, tinySTL::pair<_T1, _T2>> {
  using type = _T2;
};
