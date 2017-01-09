#include <atomic>
#include <chrono>
#include <exception>
#include <random>
#include <ratio>

#include "chrono.h"
#include "worker.h"

namespace fib {
  /// expected duration of a task.
  /**  If tasks tend to run longer than this then the model
       that says we can get away with using task sharing rather than task stealing becomes
       flawed and we don't share enough. Currently 0.1ms */
  static const double expected_task_duration = 100.0;

  void worker::run() {
// #ifdef FIB_SUPPORTS_CDS
//    cds_thread_attachment attach_thread;
// #endif

    std::uniform_int_distribution<int> random_peer(0, p.N - 2);
    std::exponential_distribution<double> random_delay_us(expected_task_duration);
    auto d = std::chrono::high_resolution_clock::now();
    for (;;) {
      task t;
      if (p.shutdown.load(std::memory_order_relaxed)) return; // check for pool shutdown
      if (q.empty()) {
        // acquire
        p.s[id].data.store(nullptr, std::memory_order_relaxed);
        // TODO: introduce exponential backoff here
        task * tp = p.s[id].data.load(std::memory_order_relaxed);
        while (t == nullptr) {
          // unemployed
          std::this_thread::yield();
          if (p.shutdown.load(std::memory_order_relaxed)) return; // check for pool shutdown
          tp = p.s[id].data.load(std::memory_order_relaxed);
        }
        // employed
        t = *tp;
        delete tp;
      } else {
        // have work
        t = q.back();
        q.pop_back();
      }
      if (p.N > 1) { // we have peers, so see if we should hand off work
        auto then = std::chrono::high_resolution_clock::now();
        // communicate if we should deal and we have something to deal out
        if (then > d && !q.empty()) {
          // deal attempt
          int j = random_peer(rng);
          if (j >= id) j += 1; // make sure it isn't us. can't wrap: j < N-1 before.

          task * expected = nullptr;
          // compare_exchange_weak should be fine, we're already in an outer loop, we'll come back
          // on excessively weak architectures, this might mean that the effective delay is much higher though
          if (p.s[j].data.load(std::memory_order_relaxed) == nullptr
           && p.s[j].data.compare_exchange_weak(expected, &q.front(), std::memory_order_seq_cst))
            q.pop_front(); // we gave the front of the deque away
            // sent work to worker j;

          // don't resample time and round down to err on the side of too much sharing if tasks run long
          d = then - fib::chrono::floor<std::chrono::high_resolution_clock::duration>(
            std::chrono::duration<double, std::micro>(random_delay_us(rng))
          );
        }
      }
      try {
        t(*this);
      } catch (...) {
        p.shutdown.store(true, std::memory_order_release);
        throw;
      }
    }
  } // worker::main

  pool::~pool() {
    shutdown.store(true, std::memory_order_release);
    for (auto && thread : threads)
      thread.join();
  }

  detail::dummy_task detail::dummy_task::instance;
}
