#pragma once

#include <atomic>
#include <deque>
#include <functional>
#include <random>
#include <thread>
#include <utility>
#include <vector>

#include "memory/isolated.h"

/// @file worker.h

namespace fib {

  struct pool;
  struct worker;

  /// something to do
  using task = std::function<void(worker&)>;

  /// The maximum number of workers allowed in any given pool.
  static const size_t max_workers = 16;

  /// @brief A member of a thread pool, replete with a local work-sharing deque.
  ///
  /// Never give this to another thread.
  /// Do not remember the current worker across blocking calls.

  // TODO: upgrade to support fibers
  struct worker {
    std::mt19937 rng;   ///< local random number generator to avoid having to go back to a central pool of randomness for sharing candidate selection
    std::deque<task> q; ///< local jobs
    pool & p;           ///< owning pool
    int i;              ///< worker id within the pool
    friend struct pool;

    /// Schedule a @p task.
    template <typename ... T, typename F> void spawn(F && f, T && ... args) {
       // TODO: Should we eagerly context switch and enqueue the current fiber instead?
       q.push_back(task(std::forward(f), std::forward(args)...));
    }
  private:
    /// construct a new worker
    template <typename SeedSeq> worker(pool &p, int i, SeedSeq & seed) : rng(seed), i(i), p(p) {}
    /// private entry point
    void main();
  };

  /// a work-sharing thread pool
  struct pool {
    /// @brief Create a pool
    /// @param N number of workers
    /// @param rng random number generator used to seed local worker random number generators
    /// @param args the initial batch of tasks used to seed the pool
    template <typename ... Ts> pool(int N, std::mt19937 & rng, Ts && ... args);

    virtual ~pool();

    int N;                                               ///< the number of actual workers
    memory::isolated<std::atomic<task*>> s[max_workers]; ///< mailboxes for sharing work
    std::vector<std::thread> threads;                    ///< the threads that run the workers
    std::atomic<bool> shutdown;                          ///< flag used to shut everything down gracefully

private:
    std::vector<worker> workers; ///< direct handles to each of our workers. not for public consumption
  };

  namespace detail {
    /// a placeholder task used to indicate lack of work
    struct dummy_task : task {
      /// @brief The only instance of dummy_task.
      ///
      /// This task gets posted in a mailbox to indicate that the corresponding worker is looking for work
      static dummy_task instance;
    };
  };

  template <typename ... Ts> pool::pool(int N, std::mt19937 & rng, Ts && ... args) : N(N) {
    for (int i = 0;i < N;++i)
      s[i].data.store(&detail::dummy_task::instance, std::memory_order_relaxed);

    shutdown.store(false, std::memory_order_relaxed);

    for (int i = 0;i < N;++i) {
      std::seed_seq s { rng(), rng(), rng(), rng() };
      workers.push_back(worker(*this, i, s));
    }

    {
      int i = 0;
      // pre-load our starting tasks
      for (auto && task : { std::function<void(worker&)>(args) ... }) {
        workers[i++].q.push_front(task); // distribute tasks round-robin to start before the threads kick in
        i %= N;
      }
    }

    for (int i = 0; i < N;++i) {
      auto w = workers[i];
      threads.push_back(std::thread([&w] { w.main(); }));
    }
  }
}
