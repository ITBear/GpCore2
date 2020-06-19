#pragma once

#include "GpEnvironmentDetector.hpp"

#include <version>

#if !defined(__cpp_consteval)
#	define consteval constexpr
#endif

//************************ STRICT ALIASING *********************************
#if defined(GP_COMPILER_ICC)
#	error Unknown value for STRICT ALIASING __restrict keyword
#elif defined(GP_COMPILER_CLANG)
#	define _R_ __restrict__
#elif defined(GP_COMPILER_GCC)
#	define _R_ __restrict
#	define BUILTIN_OVERFLOW_CONSTEXPR
#elif defined(GP_COMPILER_MSVS)
#	define _R_ __restrict
#else
#	error Unknown compiler
#endif
