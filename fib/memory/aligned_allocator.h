#pragma once

#include <memory>

#include "fib/attribute.h"

/// @file fib/memory/aligned_allocator.h
/// @brief provides @ref fib::memory::aligned_allocator

namespace fib {
  namespace memory {
    namespace detail {
      FIB_DECLSPEC_NOALIAS FIB_DECLSPEC_RESTRICT void* allocate_aligned_memory(size_t align, size_t size) FIB_ATTRIBUTE_ALLOC_ALIGN(1) FIB_ATTRIBUTE_ALLOC_SIZE(2) FIB_ATTRIBUTE_MALLOC FIB_ATTRIBUTE_RETURNS_NONNULL;
      void deallocate_aligned_memory(void* ptr) noexcept;
    }
  
    /// This allocator ensures all storage has at least a specified alignment.
    ///
    /// @param T the type of content being stored
    /// @param alignment minimum alignment
    template <typename T, size_t alignment = 32>
    struct aligned_allocator;

    /// @cond PRIVATE
    template <size_t alignment>
    struct aligned_allocator<void, alignment> {
      typedef void*             pointer;
      typedef const void*       const_pointer;
      typedef void              value_type;
  
      template <class U> struct rebind { typedef aligned_allocator<U, alignment> other; };
    };
    /// @endcond
  
    template <typename T, size_t alignment> struct aligned_allocator {
      typedef T         value_type;
      typedef T*        pointer;
      typedef const T*  const_pointer;
      typedef T&        reference;
      typedef const T&  const_reference;
      typedef size_t    size_type;
      typedef ptrdiff_t difference_type;
  
      typedef std::true_type propagate_on_container_move_assignment;
  
      /// @cond PRIVATE
      template <class U> struct rebind { typedef aligned_allocator<U, alignment> other; };
      /// @endcond
  
      aligned_allocator() noexcept {}
  
      template <class U> aligned_allocator(const aligned_allocator<U, alignment>&) noexcept {}
  
      size_type max_size() const noexcept {
        return (size_type(~0) - size_type(alignment)) / sizeof(T);
      }
  
      pointer address(reference x) const noexcept {
        return std::addressof(x);
      }
  
      const_pointer address(const_reference x) const noexcept {
        return std::addressof(x);
      }
  
      FIB_DECLSPEC_NOALIAS FIB_DECLSPEC_RESTRICT pointer allocate(size_type n, typename aligned_allocator<void, alignment>::const_pointer = 0) FIB_ATTRIBUTE_MALLOC FIB_ATTRIBUTE_RETURNS_NONNULL {
        void* ptr = detail::allocate_aligned_memory(alignment, n * sizeof(T));
        if (ptr == nullptr) throw std::bad_alloc();      
        return reinterpret_cast<pointer>(ptr);
      }
  
      void deallocate(pointer p, size_type) noexcept {
        return detail::deallocate_aligned_memory(p);
      }
  
      template <class U, class ...Args>
      void construct(U* p, Args&&... args) { 
        ::new(reinterpret_cast<void*>(p)) U(std::forward<Args>(args)...);
      }
  
      void destroy(pointer p) { p->~T(); }
    };
  
    /// @cond PRIVATE
    template <typename T, size_t alignment> struct aligned_allocator<const T, alignment> {
  
      typedef T         value_type;
      typedef const T*  pointer;
      typedef const T*  const_pointer;
      typedef const T&  reference;
      typedef const T&  const_reference;
      typedef size_t    size_type;
      typedef ptrdiff_t difference_type;
  
      typedef std::true_type propagate_on_container_move_assignment;
  
      template <class U> struct rebind { typedef aligned_allocator<U, alignment> other; };
  
      aligned_allocator() noexcept {}
  
      template <class U> aligned_allocator(const aligned_allocator<U, alignment>&) noexcept {}
  
      size_type max_size() const noexcept {
        return (size_type(~0) - size_type(alignment)) / sizeof(T);
      }
  
      const_pointer address(const_reference x) const noexcept {
        return std::addressof(x);
      }
  
      FIB_DECLSPEC_NOALIAS FIB_DECLSPEC_RESTRICT pointer allocate(size_type n, typename aligned_allocator<void, alignment>::const_pointer = 0) FIB_ATTRIBUTE_MALLOC FIB_ATTRIBUTE_RETURNS_NONNULL {
        void* ptr = detail::allocate_aligned_memory(alignment, n * sizeof(T));
        if (ptr == nullptr) throw std::bad_alloc();
        return reinterpret_cast<pointer>(ptr);
      }
  
      void deallocate(pointer p, size_type) noexcept {
        return detail::deallocate_aligned_memory(p);
      }
  
      template <class U, class ...Args> void construct(U* p, Args&&... args) {
        ::new(reinterpret_cast<void*>(p)) U(std::forward<Args>(args)...);
      }
  
      void destroy(pointer p) { p->~T(); }
    };
    // @endcond
  
    template <typename T, size_t Talignment, typename U, size_t Ualignment>
    inline bool operator== (const aligned_allocator<T, Talignment>&, const aligned_allocator<U, Ualignment>&) noexcept {
      return Talignment == Ualignment;
    }
  
    template <typename T, size_t Talignment, typename U, size_t Ualignment>
    inline bool operator!= (const aligned_allocator<T, Talignment>&, const aligned_allocator<U, Ualignment>&) noexcept {
      return Talignment != Ualignment;
    }
  }
}
