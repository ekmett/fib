#pragma once

#ifndef __has_declspec_attribute
#ifdef _MSC_VER
#define __has_declspec_attribute(X) 1
#else
#define __has_declspec_attribute(X) 0
#endif
#endif

#ifndef __has_attribute
#define __has_attribute(X) 0
#endif

#ifdef GCC
#define __has_gcc_attribute(X) 1
#else
#define __has_gcc_attribute(X) __has_attribute(X)
#endif

#if __has_declspec_attribute(noalias)
#define FIB_DECLSPEC_NOALIAS __declspec(noalias)
#else
#define FIB_DECLSPEC_NOALIAS
#endif

#if __has_declspec_attribute(restrict)
#define FIB_DECLSPEC_RESTRICT __declspec(restrict)
#else
#define FIB_DECLSPEC_RESTRICT
#endif

#if __has_gcc_attribute(malloc)
#define FIB_ATTRIBUTE_MALLOC __attribute((malloc))
#else
#define FIB_ATTRIBUTE_MALLOC
#endif

#if __has_gcc_attribute(returns_nonnull)
#define FIB_ATTRIBUTE_RETURNS_NONNULL __attribute((returns_nonnull))
#else
#define FIB_ATTRIBUTE_RETURNS_NONNULL
#endif

#if __has_gcc_attribute(alloc_align)
#define FIB_ATTRIBUTE_ALLOC_ALIGN(N) __attribute__((alloc_align(N))
#else
#define FIB_ATTRIBUTE_ALLOC_ALIGN(N)
#endif

#if __has_gcc_attribute(alloc_size)
#define FIB_ATTRIBUTE_ALLOC_SIZE(N) __attribute__((alloc_size(N))
#else
#define FIB_ATTRIBUTE_ALLOC_SIZE(N)
#endif
