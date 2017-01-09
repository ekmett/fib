#include "fib.h"

/// this module exists solely to serve as a general 'lint' pass over
/// all of the headers to ensure everything compiles.

/// @cond PRIVATE
namespace {
  FIB_ATTRIBUTE_UNUSED int lint = 0;
  FIB_ATTRIBUTE_UNUSED fib::circular_array<int> test(10);
}
/// @endcond
