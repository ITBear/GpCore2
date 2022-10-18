#pragma once

#include "GpEnvironmentDetector.hpp"

#if defined (__cplusplus)

#include <version>

#endif//#if defined (__cplusplus)

//************************ STRICT ALIASING *********************************
#if defined(GP_COMPILER_ICC)
#   error Unknown value for STRICT ALIASING __restrict keyword
#elif defined(GP_COMPILER_CLANG)
#   define _R_ __restrict__
#   define BUILTIN_OVERFLOW_CONSTEXPR
#elif defined(GP_COMPILER_GCC)
#   define _R_ __restrict
#   define BUILTIN_OVERFLOW_CONSTEXPR
#elif defined(GP_COMPILER_MSVS)
#   define _R_ __restrict
#else
#   error Unknown compiler
#endif

//199711L (C++98 or C++03)
//201103L (C++11)
//201402L (C++14)
//201703L (C++17)
//202002L (C++20)

#if __cplusplus < 202002L
#   define consteval  constexpr
#endif
