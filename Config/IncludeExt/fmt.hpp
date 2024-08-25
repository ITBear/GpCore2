#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>

#if defined(GP_COMPILER_GCC) || defined(GP_COMPILER_CLANG)
#   if __has_include(<format>)
#       include <format>
#       define FMT_NAMESPASE std
        namespace fmt = std;
#   else
        GP_WARNING_PUSH()
        GP_WARNING_DISABLE(switch-default)
        GP_WARNING_DISABLE(switch-enum)
#       include <fmt/core.h>
#       include <fmt/format.h>
#       include <fmt/ostream.h>
#       include <fmt/ranges.h>
        GP_WARNING_POP()
#       define FMT_NAMESPASE fmt
#   endif// if __has_include(<format>)
#elif defined(GP_COMPILER_MSVC)
#   include <format>
#   define FMT_NAMESPASE std
    namespace fmt = std;
#endif
