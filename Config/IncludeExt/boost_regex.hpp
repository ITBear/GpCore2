#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>

GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(conversion)
    GP_WARNING_DISABLE(switch-enum)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#if defined(GP_COMPILER_MSVC)
#   pragma warning(disable : 4365)
#   pragma warning(disable : 5262)
#endif// #if defined(GP_COMPILER_MSVC)

#include <boost/regex.hpp>

GP_WARNING_POP()
