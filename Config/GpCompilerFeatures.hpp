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

#define CPP_VERSION_03 199711L
#define CPP_VERSION_11 201103L
#define CPP_VERSION_14 201402L
#define CPP_VERSION_17 201703L
#define CPP_VERSION_20 202002L
#define CPP_VERSION_23 202303L //TODO: check

#if __cplusplus < CPP_VERSION_20
#   define consteval constexpr
#endif
