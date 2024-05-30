#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>

GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(pragma-pack)
    GP_WARNING_DISABLE(shorten-64-to-32)
    GP_WARNING_DISABLE(macro-redefined)
    GP_WARNING_DISABLE(unused-value)
    GP_WARNING_DISABLE(sign-conversion)
    GP_WARNING_DISABLE(expansion-to-defined)
    GP_WARNING_DISABLE(conversion)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#define NOMINMAX

#include <windows.h>

GP_WARNING_POP()
