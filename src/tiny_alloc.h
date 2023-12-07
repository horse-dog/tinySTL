// malloc_alloc_template, default_alloc_template, simple_alloc.

#pragma once
#include <string.h>
#include <stdexcept>

// defination of throw bad alloc.
#ifndef __THROW_BAD_ALLOC
#  if defined(__STL_NO_BAD_ALLOC) || !defined(__STL_USE_EXCEPTIONS)
#    include <stdio.h>
#    include <stdlib.h>
#    define __THROW_BAD_ALLOC fprintf(stderr, "out of memory\n"); exit(1)
#  else /* Standard conforming out-of-memory handling */
#    include <new>
#    define __THROW_BAD_ALLOC throw std::bad_alloc()
#  endif
#endif

namespace tinySTL 
{

template <typename _Tp>
inline _Tp* addressof(_Tp& __r) noexcept
{
  return reinterpret_cast<_Tp*>(
      &const_cast<char&>(reinterpret_cast<const volatile char&>(__r)));
}

template <class _Tp>
const _Tp* addressof(const _Tp&&) = delete;

template <typename _Tp>
struct aligned_membuf 
{
  struct Tval { _Tp _M_val; };
  
  alignas(alignof(Tval)) unsigned char _M_data[sizeof(_Tp)];
  
  aligned_membuf() = default;
  
  aligned_membuf(std::nullptr_t) {}
  
  void *addr() noexcept { return (void*)(&_M_data); }
  
  const void *addr() const noexcept 
    { return (const void*)(&_M_data); }

  _Tp *ptr() noexcept { return (_Tp*) addr(); }

  const _Tp *ptr() const noexcept { return (const _Tp*) addr(); }
};

// wrapper of _Alloc with type _Tp.
template<class _Tp, class _Alloc>
class simple_alloc {
 public:
  static _Tp* allocate(size_t __n) { 
    return 0 == __n ? 0 : (_Tp*) _Alloc::allocate(__n * sizeof (_Tp));
  }

  // same as allocate(1).
  static _Tp* allocate(void) { 
    return (_Tp*) _Alloc::allocate(sizeof (_Tp)); 
  }

  static void deallocate(_Tp* __p, size_t __n) { 
    if (0 != __n) _Alloc::deallocate(__p, __n * sizeof (_Tp)); 
  }

  static void deallocate(_Tp* __p) { 
    _Alloc::deallocate(__p, sizeof (_Tp)); 
  }
};


// first memory adaptor. __inst is useless.
template <int __inst>
class __malloc_alloc_template {
 private:
  // process out of memory.
  static void* _S_oom_malloc(size_t);
  static void* _S_oom_realloc(void*, size_t);

  // out of memory handler.
  static void (* __malloc_alloc_oom_handler)();
  using oom_handler_t = void(*)(void);

 public:
  static void* allocate(size_t __n) 
  {
    void* __result = malloc(__n);
    if (0 == __result) __result = _S_oom_malloc(__n);
    return __result;
  }

  static void deallocate(void* __p, size_t /* __n */) 
  { 
    free(__p); 
  }

  static void* reallocate(void* __p, size_t /* old_sz */, size_t __new_sz) 
  {
    void* __result = realloc(__p, __new_sz);
    if (0 == __result) __result = _S_oom_realloc(__p, __new_sz);
    return __result;
  }

  static oom_handler_t __set_malloc_handler(oom_handler_t __f)
  {
    auto __old = __malloc_alloc_oom_handler;
    __malloc_alloc_oom_handler = __f;
    return(__old);
  }
};

// default oom handler is nullptr.
template <int __inst>
void (* __malloc_alloc_template<__inst>::__malloc_alloc_oom_handler)() = 0;


template <int __inst>
void* __malloc_alloc_template<__inst>::_S_oom_malloc(size_t __n)
{
  oom_handler_t __my_malloc_handler;
  void* __result;

  for (;;) {
    __my_malloc_handler = __malloc_alloc_oom_handler;
    if (0 == __my_malloc_handler) { __THROW_BAD_ALLOC; }
    (*__my_malloc_handler)();
    __result = malloc(__n);
    if (__result) return(__result);
  }
}

template <int __inst>
void* __malloc_alloc_template<__inst>::_S_oom_realloc(void* __p, size_t __n)
{
  oom_handler_t __my_malloc_handler;
  void* __result;

  for (;;) {
    __my_malloc_handler = __malloc_alloc_oom_handler;
    if (0 == __my_malloc_handler) { __THROW_BAD_ALLOC; }
    (*__my_malloc_handler)();
    __result = realloc(__p, __n);
    if (__result) return(__result);
  }
}

typedef __malloc_alloc_template<0> malloc_alloc;


// second memory adaptor. __inst is useless.
// threads == true : ==> multi-threads environment.
// threads == false: ==> single-thread environment.
template <bool threads, int inst>
class __default_alloc_template {
 private:
  enum { _ALIGN = 8 };
  enum { _MAX_BYTES = 128 };
  enum { _NFREELISTS = 16 };// _MAX_BYTES / _ALIGN

  static size_t _S_round_up(size_t __bytes) { 
    return (((__bytes) + (size_t) _ALIGN - 1) & ~((size_t) _ALIGN - 1)); 
  }

  union _Obj {
    union _Obj* _M_free_list_link;
    char _M_client_data[1]; // The client sees this.
  };

  // 16 freelists.
  static _Obj* volatile _S_free_list[_NFREELISTS];

  // wanna __bytes memory, find best index in freelist.
  static size_t _S_freelist_index(size_t __bytes) {
    return (((__bytes) + (size_t) _ALIGN - 1) / (size_t) _ALIGN - 1);
  }

  // Returns an object of size __n, and optionally adds to size __n free list.
  static void* _S_refill(size_t __n);

  // Allocates a chunk for nobjs of size size.  nobjs may be reduced
  // if it is inconvenient to allocate the requested number.
  static char* _S_chunk_alloc(size_t __size, int& __nobjs);

  // Chunk allocation state.
  static char* _S_start_free; // modify just at _S_chunk_alloc
  static char* _S_end_free;   // modify just at _S_chunk_alloc
  static size_t _S_heap_size;
  static void** _S_malloc_ptr;
  static size_t _S_malloc_ptr_size;
  static size_t _S_malloc_ptr_cap;

 private:
  static void _S_add_malloc_ptr(void* __ptr) {
    if (_S_malloc_ptr == 0) {
      if (atexit(_S_mempool_free) != 0)
        throw std::runtime_error("atexit");
    }
    if (_S_malloc_ptr_size == _S_malloc_ptr_cap) {
      size_t __newlen = _S_malloc_ptr_cap == 0 ? 32 : _S_malloc_ptr_cap * 2;
      void*  __newbuf = malloc_alloc::
      reallocate(_S_malloc_ptr, 0, __newlen * sizeof(void*));
      _S_malloc_ptr = (void**)__newbuf;
      _S_malloc_ptr_cap = __newlen;
    }
    _S_malloc_ptr[_S_malloc_ptr_size++] = __ptr;
  }

  static void _S_mempool_free(void) {
    for (size_t i = 0; i < _S_malloc_ptr_size; i++) {
      free(_S_malloc_ptr[i]);
      _S_malloc_ptr[i] = 0;
    }
    free(_S_malloc_ptr);
    _S_malloc_ptr = 0;
  }

 public:
  static void* allocate(size_t __n) {
    void* __ret = 0;

    if (__n > (size_t) _MAX_BYTES) {
      __ret = malloc_alloc::allocate(__n);
    } else {
      _Obj* volatile * __my_free_list
          = _S_free_list + _S_freelist_index(__n);
      _Obj* __result = *__my_free_list;
      if (__result == 0)
        __ret = _S_refill(_S_round_up(__n));
      else {
        *__my_free_list = __result->_M_free_list_link;
        __ret = __result;
      }
    }
    return __ret;
  };

  static void deallocate(void* __p, size_t __n) {
    if (__n > (size_t) _MAX_BYTES)
      malloc_alloc::deallocate(__p, __n);
    else {
      _Obj* volatile * __my_free_list
          = _S_free_list + _S_freelist_index(__n);
      _Obj* __q = (_Obj*)__p;

      __q -> _M_free_list_link = *__my_free_list;
      *__my_free_list = __q;
    }
  }

  static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz) {
    void* __result;
    size_t __copy_sz;

    if (__old_sz > (size_t) _MAX_BYTES && __new_sz > (size_t) _MAX_BYTES) {
        return malloc_alloc::reallocate(__p, __old_sz, __new_sz);
    }

    if (_S_round_up(__old_sz) == _S_round_up(__new_sz)) return __p;

    __result = allocate(__new_sz);
    __copy_sz = __new_sz > __old_sz ? __old_sz : __new_sz;
    memcpy(__result, __p, __copy_sz);
    deallocate(__p, __old_sz);
    return __result;
  }

};

// TODO: multi-thread env.
typedef __default_alloc_template<false, 0> alloc;

template <bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_start_free = 0;

template <bool __threads, int __inst>
char* __default_alloc_template<__threads, __inst>::_S_end_free = 0;

template <bool __threads, int __inst>
size_t __default_alloc_template<__threads, __inst>::_S_heap_size = 0;

template <bool __threads, int __inst>
void** __default_alloc_template<__threads, __inst>::_S_malloc_ptr = 0;

template <bool __threads, int __inst>
size_t __default_alloc_template<__threads, __inst>::_S_malloc_ptr_size = 0;

template <bool __threads, int __inst>
size_t __default_alloc_template<__threads, __inst>::_S_malloc_ptr_cap = 0;

template <bool __threads, int __inst>
typename __default_alloc_template<__threads, __inst>::_Obj* volatile
__default_alloc_template<__threads, __inst>::_S_free_list[
  __default_alloc_template<__threads, __inst>::_NFREELISTS
] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/* Returns an object of size __n, and optionally adds to size __n free list.*/
/* We assume that __n is properly aligned.                                  */
/* We hold the allocation lock.                                             */
template <bool __threads, int __inst> void*
__default_alloc_template<__threads, __inst>::_S_refill(size_t __n) {
  int __nobjs = 20;
  char* __chunk = _S_chunk_alloc(__n, __nobjs);
  _Obj* volatile* __my_free_list;
  _Obj* __result;
  _Obj* __current_obj;
  _Obj* __next_obj;
  int __i;

  if (1 == __nobjs) return __chunk;

  __my_free_list = _S_free_list + _S_freelist_index(__n);
  /* Build free list in chunk */
  __result = (_Obj*)__chunk;
  *__my_free_list = __next_obj = (_Obj*)(__chunk + __n);
  for (__i = 1; ; __i++) {
    __current_obj = __next_obj;
    __next_obj = (_Obj*)((char*)__next_obj + __n);
    if (__nobjs - 1 == __i) {
      __current_obj -> _M_free_list_link = 0;
      break;
    } else {
      __current_obj -> _M_free_list_link = __next_obj;
    }
  }
  return __result;
}


/* We allocate memory in large chunks in order to avoid fragmenting     */
/* the malloc heap too much.                                            */
/* We assume that size is properly aligned.                             */
/* We hold the allocation lock.                                         */
template <bool __threads, int __inst> char*
__default_alloc_template<__threads, __inst>::
_S_chunk_alloc(size_t __size, int& __nobjs) {
  char* __result;
  size_t __total_bytes = __size * __nobjs;
  size_t __bytes_left = _S_end_free - _S_start_free;

  if (__bytes_left >= __total_bytes) {
    __result = _S_start_free;
    _S_start_free += __total_bytes;
    return __result;
  } else if (__bytes_left >= __size) {
    __nobjs = (int)(__bytes_left / __size);
    __total_bytes = __size * __nobjs;
    __result = _S_start_free;
    _S_start_free += __total_bytes;
    return __result;
  } else {
    size_t __bytes_to_get = 2 * __total_bytes + _S_round_up(_S_heap_size >> 4);
    
    // Try to make use of the left-over piece.
    if (__bytes_left > 0) {
      _Obj* volatile* __my_free_list =
                      _S_free_list + _S_freelist_index(__bytes_left);

      ((_Obj*)_S_start_free) -> _M_free_list_link = *__my_free_list;
      *__my_free_list = (_Obj*)_S_start_free;
    }

    _S_start_free = (char*)malloc(__bytes_to_get);
    _S_add_malloc_ptr(_S_start_free);
    if (0 == _S_start_free) {
      size_t __i;
      _Obj* volatile* __my_free_list;
      _Obj* __p;
      // Try to make do with what we have.  That can't
      // hurt.  We do not try smaller requests, since that tends
      // to result in disaster on multi-process machines.
      for (__i = __size;
           __i <= (size_t) _MAX_BYTES;
           __i += (size_t) _ALIGN) {
        __my_free_list = _S_free_list + _S_freelist_index(__i);
        __p = *__my_free_list;
        if (0 != __p) {
          *__my_free_list = __p -> _M_free_list_link;
          _S_start_free = (char*)__p;
          _S_end_free = _S_start_free + __i;
          return _S_chunk_alloc(__size, __nobjs);
          // Any leftover piece will eventually make it to the
          // right free list.
        }
      }
      _S_end_free = 0;	// In case of exception.
      _S_start_free = (char*)malloc_alloc::allocate(__bytes_to_get);
      // This should either throw an
      // exception or remedy the situation.  Thus we assume it
      // succeeded.
    }
    _S_heap_size += __bytes_to_get;
    _S_end_free = _S_start_free + __bytes_to_get;
    return _S_chunk_alloc(__size, __nobjs);
  }
}

}