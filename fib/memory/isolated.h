#pragma once

#include <cstdint>
#include <cstddef>
#include <utility>

namespace fib {
  namespace memory {
    /// isolated<T> tries to ensure that T gets its own cache line.
    /// @param T type
    /// @param N padding_bytes
    template <typename T, std::size_t N = 64> 
    struct isolated {
      static const std::size_t padding_bytes = N; ///< The number of bytes of padding before the item. The same amount of padding, minus the size of the item is applied after as well.
      typedef T type;                        ///< The type of content we're storing. Assumes sizeof(T) < N.
  
      /// copy constructor
      isolated(const isolated<T,N> & that) : data(that.data) {}
  
      /// move constructor
      isolated(T && data) : data(std::forward(data)) {}
  
      /// pass-through constructor
      template <typename ... U> isolated(U&&...args) : data(std::forward(args)...) {}
  
      /// content assignment operator
      template <typename U> isolated & operator = (U && t) {
        data = t;
        return *this;
      }
  
      /// assignment operator
      isolated & operator = (isolated & that) {
        data = that.data;
        return *this;
      }
  
    private:
      std::int8_t padding0[padding_bytes];
    public:
      /// The isolated contents
      T data;
    private:
      std::int8_t padding1[padding_bytes - sizeof(T)];
    };
  }
}
