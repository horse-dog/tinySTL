// type traits.

#pragma once

namespace tinySTL 
{

#define TINY_TEMPLATE_NULL template <>

// type traits.
template <class T, T val>
struct integral_constant {
  static constexpr T value = val;

  using value_type = T;
  using type = integral_constant;

  constexpr operator value_type() const noexcept { return (value); }
  constexpr value_type operator()() const noexcept { return (value); }
};

template <bool val>
using bool_constant = integral_constant<bool, val>;

using true_type = bool_constant<true>;

using false_type = bool_constant<false>;


template <class _Tp>
struct type_traits {
  /* Do not remove this member. It informs a compiler which
    automatically specializes type_traits that this
    type_traits template is special. It just makes sure that
    things work if an implementation is using a template
    called type_traits for something unrelated. */
  typedef true_type     this_dummy_member_must_be_first;


  /* The following restrictions should be observed for the sake of
    compilers which automatically produce type specific specializations 
    of this class:
        - You may reorder the members below if you wish
        - You may remove any of the members below if you wish
        - You must not rename members without making the corresponding
          name change in the compiler
        - Members you add will be treated like regular members unless
          you add the appropriate support in the compiler. */

  typedef false_type    has_trivial_default_constructor;
  typedef false_type    has_trivial_copy_constructor;
  typedef false_type    has_trivial_move_constructor;
  typedef false_type    has_trivial_assignment_operator;
  typedef false_type    has_trivial_move_operator;
  typedef false_type    has_trivial_destructor;
  typedef false_type    is_POD_type;
};


TINY_TEMPLATE_NULL struct type_traits<bool> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};


TINY_TEMPLATE_NULL struct type_traits<char> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

TINY_TEMPLATE_NULL struct type_traits<signed char> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

TINY_TEMPLATE_NULL struct type_traits<unsigned char> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};


TINY_TEMPLATE_NULL struct type_traits<wchar_t> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};


TINY_TEMPLATE_NULL struct type_traits<short> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

TINY_TEMPLATE_NULL struct type_traits<unsigned short> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

TINY_TEMPLATE_NULL struct type_traits<int> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

TINY_TEMPLATE_NULL struct type_traits<unsigned int> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

TINY_TEMPLATE_NULL struct type_traits<long> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

TINY_TEMPLATE_NULL struct type_traits<unsigned long> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};


TINY_TEMPLATE_NULL struct type_traits<long long> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

TINY_TEMPLATE_NULL struct type_traits<unsigned long long> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};


TINY_TEMPLATE_NULL struct type_traits<float> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

TINY_TEMPLATE_NULL struct type_traits<double> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

TINY_TEMPLATE_NULL struct type_traits<long double> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};

// pointer.
template <class _Tp> struct type_traits<_Tp*> {
   typedef true_type    has_trivial_default_constructor;
   typedef true_type    has_trivial_copy_constructor;
   typedef true_type    has_trivial_move_constructor;
   typedef true_type    has_trivial_assignment_operator;
   typedef true_type    has_trivial_move_operator;
   typedef true_type    has_trivial_destructor;
   typedef true_type    is_POD_type;
};


// enable_if.
template <bool test, class T = void >
struct enable_if {};

template <class T>
struct enable_if<true, T> { using type = T; };

template <bool test, class T = void >
using enable_if_t = typename enable_if<test, T>::type;


// is_pointer.
template <class>
inline constexpr bool is_pointer_v = false;

template <class _Ty>
inline constexpr bool is_pointer_v<_Ty*> = true;

template <class _Ty>
inline constexpr bool is_pointer_v<_Ty* const> = true;

template <class _Ty>
inline constexpr bool is_pointer_v<_Ty* volatile> = true;

template <class _Ty>
inline constexpr bool is_pointer_v<_Ty* const volatile> = true;

template <class _Ty>
struct is_pointer : bool_constant<is_pointer_v<_Ty>> {};


// is_same.
template <class, class>
inline constexpr bool is_same_v = false;

template <class _Ty>
inline constexpr bool is_same_v<_Ty, _Ty> = true;

template <class _Tp, class _Ty>
struct is_same : bool_constant<is_same_v<_Tp, _Ty>> {};


// is_base_of
template <class _Base, class _Derived>
inline constexpr bool is_base_of_v = __is_base_of(_Base, _Derived);

template <class _Base, class _Derived>
struct is_base_of : bool_constant<__is_base_of(_Base, _Derived)> {};


// is_const
template <class>
inline constexpr bool is_const_v = false;

template <class _Ty>
inline constexpr bool is_const_v<const _Ty> = true;

template <class _Ty>
struct is_const : bool_constant<is_const_v<_Ty>> {};


// remove reference.
template <class _Ty>
struct remove_reference { using type = _Ty; };

template <class _Ty>
struct remove_reference<_Ty&> { using type = _Ty; };

template <class _Ty>
struct remove_reference<_Ty&&> { using type = _Ty; };

template <class _Ty>
using remove_reference_t = typename remove_reference<_Ty>::type;


// move.
template <typename _Ty>
typename remove_reference<_Ty>::type&& move(_Ty&& t) 
{
  return static_cast<typename remove_reference<_Ty>::type&&>(t);
}

// forward.
template <typename _Ty>
_Ty&& forward(typename std::remove_reference<_Ty>::type& t) {
  return static_cast<_Ty&&>(t);
}

template <typename _Ty>
_Ty &&forward(typename std::remove_reference<_Ty>::type&& t) 
{
  return static_cast<_Ty&&>(t);
}

}
