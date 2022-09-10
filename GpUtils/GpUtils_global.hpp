#pragma once

#include "../Config/GpConfig.hpp"

//*************************** EXPORT DEFINES **********************************
#if defined(GP_COMPILER_GCC) || defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_ICC)
#   define GP_DECL_IMPORT
#   define GP_DECL_EXPORT __attribute__((visibility("default")))//compile with -fvisibility=hidden
#elif defined(GP_COMPILER_MSVS)
#   define GP_DECL_IMPORT __declspec(dllimport)
#   define GP_DECL_EXPORT __declspec(dllexport)
#else
#   error Unknown compiler
#endif

#if defined(GP_UTILS_LIBRARY)
    #define GP_UTILS_API GP_DECL_EXPORT
#else
    #define GP_UTILS_API GP_DECL_IMPORT
#endif

namespace GPlatform {}
