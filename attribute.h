#pragma once

// __declspec support

#ifndef __has_declspec_attribute
#define __has_declspec_attribute(X) 0
#endif

#ifdef _MSC_VER
#define __has_ms_declspec_attribute(X) 1
#else
#define __has_ms_declspec_attribute(X) __has_declspec_attribute(X)
#endif

#if __has_ms_declspec_attribute(noalias)
#define FIB_DECLSPEC_NOALIAS __declspec(noalias)
#else
#define FIB_DECLSPEC_NOALIAS
#endif

#if __has_ms_declspec_attribute(restrict)
#define FIB_DECLSPEC_RESTRICT __declspec(restrict)
#else
#define FIB_DECLSPEC_RESTRICT
#endif

// __attribute__ support

#ifndef __has_attribute
#define __has_attribute(X) 0
#endif

#ifdef GCC
#define __has_gcc_attribute(X) 1
#else
#define __has_gcc_attribute(X) __has_attribute(X)
#endif

#if __has_gcc_attribute(malloc)
#define FIB_ATTRIBUTE_MALLOC __attribute__((malloc))
#else
#define FIB_ATTRIBUTE_MALLOC
#endif

#if __has_gcc_attribute(returns_nonnull)
#define FIB_ATTRIBUTE_RETURNS_NONNULL __attribute__((returns_nonnull))
#else
#define FIB_ATTRIBUTE_RETURNS_NONNULL
#endif

#if __has_gcc_attribute(alloc_align)
#define FIB_ATTRIBUTE_ALLOC_ALIGN(N) __attribute__((alloc_align(N)))
#else
#define FIB_ATTRIBUTE_ALLOC_ALIGN(N)
#endif

#if __has_gcc_attribute(alloc_size)
#define FIB_ATTRIBUTE_ALLOC_SIZE(N) __attribute__((alloc_size(N)))
#else
#define FIB_ATTRIBUTE_ALLOC_SIZE(N)
#endif

#if __has_gcc_attribute(const)
#define FIB_ATTRIBUTE_CONST __attribute__((const))
#else
#define FIB_ATTRIBUTE_CONST
#endif

#if __has_gcc_attribute(pure)
#define FIB_ATTRIBUTE_PURE __attribute__((pure))
#else
#define FIB_ATTRIBUTE_PURE
#endif
