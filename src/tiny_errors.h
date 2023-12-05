#pragma once

#include <stdexcept>

namespace tinySTL
{

inline void __tiny_throw_range_error(const char* __msg) 
  { throw std::range_error(__msg); }

inline void __tiny_throw_length_error(const char* __msg) 
  { throw std::length_error(__msg); }

}