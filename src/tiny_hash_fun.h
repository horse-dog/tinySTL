#pragma once
#include <cstddef>
#include <string>

namespace tinySTL 
{

template <class _Tp> struct hash {};

inline size_t __stl_hash_string(const char* __s)
{
  uint64_t __hash = 0;
  for (int i = 0; *__s; i++) {
    if (i & 1) {
      __hash ^= ~((__hash << 11) ^ (*__s++) ^ (__hash >> 5));
    } else {
      __hash ^= ((__hash << 7) ^ (*__s++) ^ (__hash >> 3));
    }
  }
  return size_t(__hash & 0x7FFFFFFF);
}

template <> struct hash<char*>
{
  size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
};
template <> struct hash<const char*>
{
  size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
};
template <> struct hash<std::string>
{
  size_t operator()(const std::string& __s) const { return __stl_hash_string(__s.c_str()); }
};
template <> struct hash<char> {
  size_t operator()(char __x) const { return __x; }
};
template <> struct hash<unsigned char> {
  size_t operator()(unsigned char __x) const { return __x; }
};
template <> struct hash<signed char> {
  size_t operator()(unsigned char __x) const { return __x; }
};
template <> struct hash<short> {
  size_t operator()(short __x) const { return __x; }
};
template <> struct hash<unsigned short> {
  size_t operator()(unsigned short __x) const { return __x; }
};
template <> struct hash<int> {
  size_t operator()(int __x) const { return __x; }
};
template <> struct hash<unsigned int> {
  size_t operator()(unsigned int __x) const { return __x; }
};
template <> struct hash<long> {
  size_t operator()(long __x) const { return __x; }
};
template <> struct hash<unsigned long> {
  size_t operator()(unsigned long __x) const { return __x; }
};
template <> struct hash<float> {
  size_t operator()(float __x) const 
  {
    // -0.0 and 0.0 should return same hash
    if (__x == 0.0) return 0;
    return *(uint32_t*)(&__x);
  }
};
template <> struct hash<double> {
  size_t operator()(double __x) const 
  { 
    // -0.0 and 0.0 should return same hash
    if (__x == 0.0) return 0;
    return *(uint64_t*)(&__x);
  }
};

}
