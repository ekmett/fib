#pragma once

#include <cstdint>

namespace fib {
  // try to ensure that T gets its own cache line.
  template <typename T, size_t N = 64> 
  struct cache_isolated {
    static const size_t padding_bytes = N;

    cache_isolated(const cache_isolated<T,N> & that) : data(that.data) {}

    cache_isolated(T && data) : data(std::forward(data)) {}

    template <typename ... U> cache_isolated(U&&...args) : data(std::forward(args)...) {}

    template <typename U> cache_isolated & operator = (U && t) {
      data = t;
      return *this;
    }

    cache_isolated & operator = (cache_isolated & that) {
      data = that.data;
      return *this;
    }

  private:
    std::int8_t padding0[padding_bytes];
  public:
    T data;
  private:
    std::int8_t padding1[padding_bytes - sizeof(T)];
  };
}
