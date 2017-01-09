#pragma once

#include <memory>
#include <cstddef>
#include <cassert>
#include "fib/attribute.h"
#include "fib/memory/aligned_allocator.h"

namespace fib {

  template <typename T, typename Allocator = fib::memory::aligned_allocator<std::atomic<T>, 128>>
  struct circular_array {
    typedef typename std::allocator_traits<Allocator> allocator_traits;
    typedef typename allocator_traits::value_type value_type;
    typedef typename allocator_traits::pointer pointer;
    typedef typename allocator_traits::const_pointer const_pointer;
    typedef typename allocator_traits::void_pointer void_pointer;
    typedef typename allocator_traits::const_void_pointer const_void_pointer;
    typedef typename allocator_traits::difference_type difference_type;
    typedef typename allocator_traits::size_type size_type;
 
    // We can fix this later with a bunch of placement news, but it is expensive and I'm lazy.
    static_assert(sizeof(T) == sizeof(std::atomic<T>), "size of T isn't the same as the size of std::atomic<T>");

    const std::size_t N;
    Allocator allocator;

    circular_array(std::size_t N, circular_array * p = nullptr) : N(N), allocator(), previous(p) {
      assert(N > 0);
      assert((N&~N) == N); // N is a power of two
      items = reinterpret_cast<std::atomic<T>*>(allocator.allocate(N));
    }
    std::size_t size() const noexcept {
      return N;
    }
    T get(std::size_t index) const noexcept {
      return items[index & (size() - 1)].load(std::memory_order_relaxed);
    }
    void put(std::size_t index, T x) noexcept {
      items[index & (size() - 1)].store(x, std::memory_order_relaxed);
    }
    FIB_DECLSPEC_NOALIAS FIB_DECLSPEC_RESTRICT circular_array * grow(size_t top, size_t bottom) const FIB_ATTRIBUTE_RETURNS_NONNULL {
      circular_array * new_array = new circular_array(N * 2, this);
      for (std::size_t i = top; i != bottom; ++i)
        new_array->put(i, get(i));
      return new_array;
    }
  private:
    std::atomic<T> * items;
    std::unique_ptr<circular_array> previous;
  };
}
