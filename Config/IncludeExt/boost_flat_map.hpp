#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>

GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(switch-default)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#include <boost/container/flat_map.hpp>

GP_WARNING_POP()
