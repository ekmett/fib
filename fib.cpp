#include "fib.h"

/// @file fib.cpp
///
/// this module exists solely to serve as a general 'lint' pass over
/// all of the headers to ensure everything compiles.
///

/// @cond PRIVATE
namespace fib {
  /// Without this definition, this module would otherwise produce no object file and we'd have to listen to ranlib complain
  FIB_ATTRIBUTE_UNUSED int lint = 0;
}
/// @endcond
