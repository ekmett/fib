#include <atomic>
#include <deque>
#include <functional>
#include <random>
#include <thread>
#include <utility>
#include <vector>

#include "cache_isolated.h"

namespace fib {

  struct pool;
  struct worker;

  using task = std::function<void(worker&)>;

  static const size_t max_workers = 16;
  static const double task_delta = 0.0002;

  struct worker {
    // todo: upgrade to support fibers
    template <typename SeedSeq> worker(pool &p, int i, SeedSeq & seed) : rng(seed), i(i), p(p) {}
    std::mt19937 rng;
    std::deque<task> q; // local jobs
    pool & p; // owning pool
    int i; // worker id
    void main();
  };

  struct pool {
    // in between the time we start and the time we stop tasks are running. 
    // This provides no mechanism to detect their state, however.
    template <typename ... Ts> pool(int N, std::mt19937 r, Ts && ... args); // give us a list of starting tasks
    virtual ~pool();

    int N;
    cache_isolated<std::atomic<task*>> s[max_workers]; // messaging primitives
    std::vector<std::thread> threads;
    std::atomic<bool> shutdown;
    std::vector<worker> workers;

    void run(task) {} // enqueue a task

    template <typename ... T, typename F> void run(F && f, T && ... args) {
      run(std::function<void(T...)>(std::forward(f), std::forward(args)...));
    }

    void run(int, task) {} // enqueue a task with affinity    

    template <typename ... T, typename F>
    void run(int i, F && f, T && ... args) {
      run(i, std::function<void(T...)>(std::forward(f), std::forward(args)...));
    }
  };

  namespace detail {
    struct dummy_task : task {
      static dummy_task instance;    
    };
  };

  template <typename ... Ts> pool::pool(int N, std::mt19937 rng, Ts && ... args) : N(N) {

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
