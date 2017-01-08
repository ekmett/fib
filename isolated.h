#pragma once

#include <cstdint>

namespace fib {
  // try to ensure that T gets its own cache line.
  template <typename T, size_t N = 64> 
  struct isolated {
    static const size_t padding_bytes = N;

    isolated(const isolated<T,N> & that) : data(that.data) {}

    isolated(T && data) : data(std::forward(data)) {}

    template <typename ... U> isolated(U&&...args) : data(std::forward(args)...) {}

    template <typename U> isolated & operator = (U && t) {
      data = t;
      return *this;
    }

    isolated & operator = (isolated & that) {
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
