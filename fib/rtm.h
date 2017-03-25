#pragma once

#ifdef __RTM__
#define FIB_RTM
#endif

/// @file fib/rtm.h
/// @brief Restricted Transactional Memory

/// @defgroup rtm RTM
/// @brief Restricted Transactional Memory
///
/// @{

namespace fib {
#ifdef FIB_RTM
  static const uint32_t rtm_started = ~0U;            ///< result of rtm_begin if it works
  static const uint32_t rtm_status_explicit = 1 << 0; ///< rtm_abort called explicitly
  static const uint32_t rtm_status_retry    = 1 << 1; ///< may succeed on retry, always clear if bit 0 set
  static const uint32_t rtm_status_conflict = 1 << 2; ///< set if another logical processor conflicted
  static const uint32_t rtm_status_capacity = 1 << 3; ///< an internal buffer overflowed
  static const uint32_t rtm_status_debug    = 1 << 4; ///< a debug breakport was hit
  static const uint32_t rtm_status_nested   = 1 << 5; ///< abort during nested transaction 

  static inline constexpr uint32_t rtm_status_code(uint32_t x) { // retrieve the argument to xabort
    return (x >> 24) & 0xff;
  }

  /// Attempts to begin a RTM transaction. Returns rtm_started if successful.
  static FIB_ATTRIBUTE_ALWAYS_INLINE inline uint32_t rtm_begin() {
    int ret = rtm_started;
    asm volatile(".byte 0xc7,0xf8 ; .long 0" : "+a" (ret) :: "memory");
    return ret;
  }

  /// End an RTM transaction.
  static FIB_ATTRIBUTE_ALWAYS_INLINE inline void rtm_end() {
    asm volatile(".byte 0x0f,0x01,0xd5" ::: "memory");
  }

  /// Abort an RTM transaction.
  static FIB_ATTRIBUTE_ALWAYS_INLINE inline void rtm_abort(const uint32_t status) {
    asm volatile(".byte 0xc6,0xf8,%P0" :: "i" (status) : "memory");
  }

  /// Is RTM active?
  static FIB_ATTRIBUTE_ALWAYS_INLINE inline bool rtm_test() {
    bool r;
    asm volatile(".byte 0x0f,0x01,0xd6 ; setnz %0" : "=r" (r) :: "memory");
    return r;
  }
#endif
}

/// @}
