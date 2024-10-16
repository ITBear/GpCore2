#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_ARCH_WASM_32)
#   define GP_DECL_IMPORT
#   define GP_DECL_EXPORT
#elif defined(GP_COMPILER_GCC) || defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_ICC)
#   define GP_DECL_IMPORT
#   define GP_DECL_EXPORT __attribute__((visibility("default")))// compile with -fvisibility=hidden
#elif defined(GP_COMPILER_MSVC)
#   if !defined(RELEASE_BUILD_STATIC)
#       define GP_DECL_IMPORT __declspec(dllimport)
#       define GP_DECL_EXPORT __declspec(dllexport)
#   else
#       define GP_DECL_IMPORT
#       define GP_DECL_EXPORT
#   endif
#else
#   error Unknown compiler
#endif
