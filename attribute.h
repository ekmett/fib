#pragma once

#ifdef _MSC_VER
#define FIB_DECLSPEC_NOALIAS __declspec(noalias)
#define FIB_DECLSPEC_RESTRICT __declspec(restrict)
#else
#define FIB_DECLSPEC_NOALIAS
#define FIB_DECLSPEC_RESTRICT
#endif

#ifdef GCC
#define FIB_ATTRIBUTE_ALLOC_ALIGN(N) __attribute__((alloc_align(N)))
#define FIB_ATTRIBUTE_ALLOC_SIZE(N) __attribute__((alloc_size(N)))
#define FIB_ATTRIBUTE_MALLOC __attribute((malloc))
#else
#define FIB_ATTRIBUTE_ALLOC_ALIGN(N)
#define FIB_ATTRIBUTE_ALLOC_SIZE(N)
#define FIB_ATTRIBUTE_MALLOC
#endif

#if define(GCC) || defined(__clang__)
#define FIB_ATTRIBUTE_RETURNS_NONNULL __attribute((returns_nonnull))
#else
#define FIB_ATTRIBUTE_RETURNS_NONNULL
#endif
