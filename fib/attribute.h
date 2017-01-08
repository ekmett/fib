#pragma once

/// @file attribute.h
/// @brief Portable support for @p __declspec and @p \__attribute__ annotations

/// @defgroup macros Macros
/// @brief Macros
///
/// @{

/// @defgroup declspecs __declspec support
/// @brief Portable support for Microsoft style @p __declspec annotations
///
/// @code{.cpp}
/// #include "fib/attribute.h"
/// @endcode
///
/// @{

// disable __declspec support on clang entirely unless we have -fms-extensions turned on
#if defined(__clang__) && !defined(_MSC_EXTENSIONS)
#define __declspec(X)
#endif

/// @def __has_declspec_attribute(X)
/// @brief clang's @p __has_declspec_attribute
#ifndef __has_declspec_attribute
#define __has_declspec_attribute(X) 0
#endif

/// @def __has_ms_declspec_attribute(X)
/// @brief modified form of clang's @p __has_declspec_attribute
///
/// Always returns 1 on Microsoft Visual C++
#ifdef _MSC_VER
#define __has_ms_declspec_attribute(X) 1
#else
#define __has_ms_declspec_attribute(X) __has_declspec_attribute(X)
#endif

/// @def FIB_DECLSPEC_NOALIAS
/// @brief portable version of @p __declspec(noalias)
#if __has_ms_declspec_attribute(noalias)
#define FIB_DECLSPEC_NOALIAS __declspec(noalias)
#else
#define FIB_DECLSPEC_NOALIAS
#endif

/// @def FIB_DECLSPEC_RESTRICT
/// @brief portable version of @p __declspec(restrict)
#if __has_ms_declspec_attribute(restrict)
#define FIB_DECLSPEC_RESTRICT __declspec(restrict)
#else
#define FIB_DECLSPEC_RESTRICT
#endif

/// @}

/// @defgroup attributes __attribute__ support
/// @brief Portable support for GCC-style @p \__attribute__ annotations
///
/// @code{.cpp}
/// #include "fib/attribute.h"
/// @endcode
///
/// @{

/// @def __has_attribute(X)
/// @brief clang's @p __has_attribute
#ifndef __has_attribute
#define __has_attribute(X) 0
#endif

/// @def __has_gcc_attribute(X)
/// @brief modified form of clang's @p __has_attribute
///
/// Always returns 1 on gcc. 
#ifdef GCC
#define __has_gcc_attribute(X) 1
#else
#define __has_gcc_attribute(X) __has_attribute(X)
#endif

/// @def FIB_ATTRIBUTE_MALLOC
/// @brief portable version of gcc's @p \__attribute__((malloc))
///
/// Nothing in the returned memory aliases any other pointer
#if __has_gcc_attribute(malloc)
#define FIB_ATTRIBUTE_MALLOC __attribute__((malloc))
#else
#define FIB_ATTRIBUTE_MALLOC
#endif

/// @def FIB_ATTRIBUTE_RETURNS_NONNULL
/// @brief portable version of gcc's @p \__attribute__((returns_nonnull))
///
/// Never returns a null pointer
#if __has_gcc_attribute(returns_nonnull)
#define FIB_ATTRIBUTE_RETURNS_NONNULL __attribute__((returns_nonnull))
#else
#define FIB_ATTRIBUTE_RETURNS_NONNULL
#endif

/// @def FIB_ATTRIBUTE_ALLOC_ALIGN
/// @brief portable version of gcc's @p \__attribute_((alloc_align(N)))
/// @param N the number of the argument that specifies the result alignment
///
/// The result will be aligned to at least an N byte boundary
#if __has_gcc_attribute(alloc_align)
#define FIB_ATTRIBUTE_ALLOC_ALIGN(N) __attribute__((alloc_align(N)))
#else
#define FIB_ATTRIBUTE_ALLOC_ALIGN(N)
#endif

/// @def FIB_ATTRIBUTE_ALLOC_SIZE
/// @brief portable version of gcc's @p \__attribute__((alloc_size(N)))
/// @param N the number of the argument that specifies the result size
///
/// The result will be exactly N bytes in size
#if __has_gcc_attribute(alloc_size)
#define FIB_ATTRIBUTE_ALLOC_SIZE(N) __attribute__((alloc_size(N)))
#else
#define FIB_ATTRIBUTE_ALLOC_SIZE(N)
#endif

/// @def FIB_ATTRIBUTE_CONST
/// @brief portable version of gcc's @p \__attribute__((const))
///
/// Used to annotate a pure function that accesses nothing other than its inputs to compute the output
#if __has_gcc_attribute(const)
#define FIB_ATTRIBUTE_CONST __attribute__((const))
#else
#define FIB_ATTRIBUTE_CONST
#endif

/// @def FIB_ATTRIBUTE_PURE
/// @brief portable version of gcc's @p \__attribute__((pure))
///
/// The result is a pure function, and can be subjected to common subexpression elimination
#if __has_gcc_attribute(pure)
#define FIB_ATTRIBUTE_PURE __attribute__((pure))
#else
#define FIB_ATTRIBUTE_PURE
#endif

/// @def FIB_ATTRIBUTE_WARN_UNUSED_RESULT
/// @brief portable version of gcc's @p \__attribute__((warn_unused_result))
///
/// Warn if the user doesn't do something with the result of this function.
#if __has_gcc_attribute(warn_unused_result)
#define FIB_ATTRIBUTE_WARN_UNUSED_RESULT __attribute((warn_unused_result))
#else
#define FIB_ATTRIBUTE_WARN_UNUSED_RESULT
#endif

/// @def FIB_ATTRIBUTE_UNUSED
/// @brief portable version of gcc's @p \__attribute__((unused))
///
/// Yes, we know this is unused. Shut up, compiler.
#if __has_gcc_attribute(unused)
#define FIB_ATTRIBUTE_UNUSED __attribute__((unused))
#else
#define FIB_ATTRIBUTE_UNUSED
#endif

// @}

// @}
