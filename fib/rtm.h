#pragma once

// enable RTM if we're build with feature -mrtm
#ifdef __RTM__
#define FIB_RTM
#endif

/// @file rtm.h
/// @brief Intel Restricted Transactional Memory

#define FIB_RTM_STARTED  (~0U)
#define FIB_RTM_EXPLICIT (1 << 0)
#define FIB_RTM_RETRY    (1 << 1)
#define FIB_RTM_CONFLICT (1 << 2)
#define FIB_RTM_CAPACITY (1 << 3)
#define FIB_RTM_DEBUG    (1 << 4)
#define FIB_RTM_NESTED   (1 << 5)
#define FIB_RTM_CODE(X)  ((X) >> 24 & 0xff)

namespace fib {
#ifdef FIB_RTM
  static FIB_ATTRIBUTE_ALWAYS_INLINE inline uint32_t xbegin() {
    int ret = FIB_RTM_STARTED
    asm volatile(".byte 0xc7,0xf8 ; .long 0" : "+a" (ret) :: "memory");
    return ret;
  }

  static FIB_ATTRIBUTE_ALWAYS_INLINE inline void xend() {
    asm volatile(".byte 0x0f,0x01,0xd5" ::: "memory");
  }

  static FIB_ATTRIBUTE_ALWAYS_INLINE inline void xabort() {
    asm volatile(".byte 0xc6,0xf8,%P0" :: "i" (status) : "memory")
  }

  static FIB_ATTRIBUTE_ALWAYS_INLINE inline uint32_t xtest() {
    bool r;
    asm volatile(".byte 0x0f,0x01,0xd6 ; setnz %0" : "=r" (r) :: "memory");
    return r;
  }
#endif
}
