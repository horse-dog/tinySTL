#pragma once

#include "tiny_errors.h"
#include "tiny_algobase.h"
#include "tiny_iterator.h"

namespace tinySTL {

template <typename _Tp, size_t _Nm>
struct array 
{ _Tp _M_elems[_Nm];
  using value_type = _Tp;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reverse_iterator = tinySTL::reverse_iterator<iterator>;
  using const_reverse_iterator = tinySTL::reverse_iterator<const_iterator>;

  constexpr void 
  fill(const value_type& __u)
    { tinySTL::fill_n(begin(), _Nm, __u); }

  constexpr void
  swap(array& __other)
    { 
      for (size_type i = 0; i < _Nm; i++)
        tinySTL::swap(_M_elems[i], __other._M_elems[i]);
    }

  constexpr iterator
  begin() noexcept
    { return iterator(data()); }

  constexpr const_iterator
  begin() const noexcept
    { return const_iterator(data()); }

  constexpr iterator
  end() noexcept
    { return iterator(data() + _Nm); }

  constexpr const_iterator
  end() const noexcept
    { return const_iterator(data() + _Nm); }

  constexpr reverse_iterator
  rbegin() noexcept
    { return reverse_iterator(end()); }

  constexpr const_reverse_iterator
  rbegin() const noexcept
    { return const_reverse_iterator(end()); }

  constexpr reverse_iterator
  rend() noexcept
    { return reverse_iterator(begin()); }

  constexpr const_reverse_iterator
  rend() const noexcept
    { return const_reverse_iterator(begin()); }

  constexpr const_iterator
  cbegin() const noexcept
    { return const_iterator(data()); }

  constexpr const_iterator
  cend() const noexcept
    { return const_iterator(data() + _Nm); }

  constexpr const_reverse_iterator
  crbegin() const noexcept
    { return const_reverse_iterator(end()); }

  constexpr const_reverse_iterator
  crend() const noexcept
    { return const_reverse_iterator(begin()); }

  constexpr size_type
  size() const noexcept { return _Nm; }

  constexpr size_type
  max_size() const noexcept { return _Nm; }

  constexpr bool
  empty() const noexcept { return size() == 0; }

  constexpr reference
  operator[](size_type __n) noexcept
    { return _M_elems[__n]; }

  constexpr const_reference
  operator[](size_type __n) const noexcept
    { return _M_elems[__n]; }


  constexpr reference
  at(size_type __n)
    { 
      if (__n >= _Nm)
        __tiny_throw_range_error("array");
      return _M_elems[__n];
    }

  constexpr const_reference
  at(size_type __n) const
    { 
      if (__n >= _Nm)
        __tiny_throw_range_error("array");
      return _M_elems[__n];
    }

  constexpr reference
  front() noexcept 
    { 
      static_assert(_Nm > 0, "array<_Tp, 0> is meaningless.");
      return *begin(); 
    }

  constexpr const_reference
  front() const noexcept
    { 
      static_assert(_Nm > 0, "array<_Tp, 0> is meaningless.");
      return *begin(); 
    }

  constexpr reference
  back() noexcept 
    { 
      static_assert(_Nm > 0, "array<_Tp, 0> is meaningless.");
      return *(end() - 1); 
    }

  constexpr const_reference
  back() const noexcept
    { 
      static_assert(_Nm > 0, "array<_Tp, 0> is meaningless.");
      return *(end() - 1); 
    }

  constexpr pointer
  data() noexcept { return _M_elems; }

  constexpr const_pointer
  data() const noexcept { return _M_elems; }

};

}