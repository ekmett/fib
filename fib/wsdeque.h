#pragma once

#include <atomic>
#include <memory>
#include <utility>

#include "fib/memory/aligned_allocator.h"
#include "fib/circular_array.h"

/// @file wsdeque.h
/// @brief fib::wsdeque<T>

namespace fib {

  /// The result of attempting to steal from the bottom of our deque
  enum class stealing : int {
    empty = 0,  ///< no result
    stolen = 1, ///< we got away with it
    aborted = 2 ///< temporary failure
  };

  /// A work-stealing deque 
  /// Based on the paper [Dynamic circular word-stealing deque](http://dl.acm.org/citation.cfm?id=1073974) by David Chase and Yossi Lev.
  template <typename T, typename Allocator = fib::memory::aligned_allocator<T, 128>> struct wsdeque {
    /// @cond PRIVATE
    // noncopyable
    wsdeque(const wsdeque&) = delete;
    wsdeque& operator=(const wsdeque&) = delete;
    /// @endcond PRIVATE

    wsdeque(size_t initial_size = 32);
    ~wsdeque() noexcept;
    /// push onto the top of the deque. Only usable by the local thread
    void push(T x);
    /// pop off the top of the deque. Only usable by the local thread
    bool pop(T & result) noexcept;
    /// Provided for convenience for when T is meaningfully default constructible as a sentinel value
    inline T pop() noexcept; 
    /// Attempt steal from the bottom of the deque. This can be called from any thread
    stealing steal(T & result) noexcept;

  private:
    typedef circular_array<T, Allocator> circular_array_type;
    std::atomic<circular_array_type *> array;
    fib::memory::isolated<std::atomic<size_t>> top, bottom;
  };

  template <typename T, typename Allocator>
  inline wsdeque<T,Allocator>::wsdeque(size_t initial_size) : array(new circular_array_type(initial_size)) {
    top.data.store(0);
    bottom.data.store(0);
  }

  template <typename T, typename Allocator>
  inline wsdeque<T,Allocator>::~wsdeque() noexcept {
    circular_array_type * p = array.load(std::memory_order_relaxed);
    if (p) delete p;
  }

  template <typename T, typename Allocator>
  inline void wsdeque<T,Allocator>::push(T x) {
    size_t b = bottom.data.load(std::memory_order_relaxed);
    size_t t = top.data.load(std::memory_order_acquire);
    circular_array_type * a = array.load(std::memory_order_relaxed);
    if (b - t > a->size() - 1) {
      a = a->grow(t, b);
      array.store(a, std::memory_order_relaxed);
    }
    a->put(b, x);
    std::atomic_thread_fence(std::memory_order_release);
    bottom.data.store(b + 1, std::memory_order_relaxed);
  }

  template <typename T, typename Allocator>
  inline bool wsdeque<T,Allocator>::pop(T & result) noexcept {
    size_t b = bottom.data.load(std::memory_order_relaxed) - 1;
    circular_array_type * a = array.load(std::memory_order_relaxed);
    bottom.data.store(b, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    size_t t = top.data.load(std::memory_order_relaxed);
    if (t <= b) {
      T x = a->get(b);
      if (t == b) {
        if (!top.data.compare_exchange_strong(t, t + 1, std::memory_order_seq_cst, std::memory_order_relaxed)) {
          return false;
        }
        bottom.data.store(b + 1, std::memory_order_relaxed);
      }
      result = x;
      return true;
    } else {
      bottom.data.store(b + 1, std::memory_order_relaxed);
      return false;
    }
  }

  template <typename T, typename Allocator>
  inline T wsdeque<T,Allocator>::pop() noexcept {
    T result;
    return pop(result) ? result : T();
  }

  template <typename T, typename Allocator>
  inline stealing wsdeque<T,Allocator>::steal(T & result) noexcept {
    std::size_t t = top.data.load(std::memory_order_acquire);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    std::size_t b = bottom.data.load(std::memory_order_acquire);
    if (t >= b) return stealing::empty;
    if (t < b) {
      circular_array_type * a = array.load(std::memory_order_relaxed);
      T x = a->get(t);
      if (!top.data.compare_exchange_strong(t, t + 1, std::memory_order_seq_cst, std::memory_order_relaxed)) {
        return stealing::aborted;
      }
      result = x;
      return stealing::stolen;
    }
  }
}
