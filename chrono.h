#pragma once

#include <chrono>
#include <type_traits>

namespace fib {
  namespace chrono {
    namespace detail {
      template <class T> struct is_duration : std::false_type {};
      template <class Rep, class Period> struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type {};
     
      // c++17 std::chrono::floor polyfill

      template <class To, class Rep, class Period> constexpr To floor_helper(const std::chrono::duration<Rep, Period>& d, To t) {
        return t > d ?  t - To{1} : t;
      }
    }
    template <class To, class Rep, class Period, class = typename std::enable_if<detail::is_duration<To>{}>::type> constexpr To floor(const std::chrono::duration<Rep, Period>& d) {
      return detail::floor_helper<To,Rep,Period>(d, std::chrono::duration_cast<To>(d));
    }
  }
}
