#pragma once

namespace tinySTL
{

template<typename _Arg, typename _Result>
struct unary_function
{
  /// @c argument_type is the type of the argument
  typedef _Arg    argument_type;   

  /// @c result_type is the return type
  typedef _Result result_type;  
};


template<typename _Arg1, typename _Arg2, typename _Result>
struct binary_function
{
  /// @c first_argument_type is the type of the first argument
  typedef _Arg1   first_argument_type; 

  /// @c second_argument_type is the type of the second argument
  typedef _Arg2   second_argument_type;

  /// @c result_type is the return type
  typedef _Result result_type;
};


template<typename _Tp>
struct _Identity
: public unary_function<_Tp, _Tp>
{
  _Tp&
  operator()(_Tp& __x) const
  { return __x; }

  const _Tp&
  operator()(const _Tp& __x) const
  { return __x; }
};

template<typename _Tp> struct _Identity<const _Tp> : _Identity<_Tp> { };


template<typename _Pair>
struct _Select1st
: public unary_function<_Pair, typename _Pair::first_type>
{
  typename _Pair::first_type&
  operator()(_Pair& __x) const
  { return __x.first; }

  const typename _Pair::first_type&
  operator()(const _Pair& __x) const
  { return __x.first; }

  template<typename _Pair2>
  typename _Pair2::first_type&
  operator()(_Pair2& __x) const
  { return __x.first; }

  template<typename _Pair2>
  const typename _Pair2::first_type&
  operator()(const _Pair2& __x) const
  { return __x.first; }
};

template <class _Tp>
struct less : binary_function<_Tp, _Tp, bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const
  { return __x < __y; }
};

template <class _Tp>
struct greater : binary_function<_Tp, _Tp, bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const
  { return __x > __y; }
};

template <class _Tp>
struct greater_equal : binary_function<_Tp, _Tp, bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const
  { return __x >= __y; }
};

template <class _Tp>
struct less_equal : binary_function<_Tp, _Tp, bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const
  { return __x <= __y; }
};

template <class _Tp>
struct equal_to : binary_function<_Tp, _Tp, bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const
  { return __x == __y; }
};

}