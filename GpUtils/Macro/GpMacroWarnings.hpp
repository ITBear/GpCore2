#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_COMPILER_GCC) || defined(GP_COMPILER_CLANG)
#   define DIAG_STR(s) #s
#   define DIAG_JOINSTR(x,y) DIAG_STR(x ## y)
#   define DIAG_DO_PRAGMA(x) _Pragma (#x)
#   define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(compiler diagnostic x)

#   define GP_WARNING_PUSH()            DIAG_PRAGMA(GP_COMPILER_NAME, push)
#   define GP_WARNING_POP()             DIAG_PRAGMA(GP_COMPILER_NAME, pop)
#   define GP_WARNING_DISABLE(option)   DIAG_PRAGMA(GP_COMPILER_NAME, ignored DIAG_JOINSTR(-W,option))

#   if defined(GP_COMPILER_GCC)
#       define GP_WARNING_DISABLE_GCC(option)   DIAG_PRAGMA(GP_COMPILER_NAME, ignored DIAG_JOINSTR(-W,option))
#   else
#       define GP_WARNING_DISABLE_GCC(option)
#   endif

    GP_WARNING_DISABLE(pragmas)
#elif defined(GP_COMPILER_MSVC)
#   define GP_WARNING_PUSH()            _Pragma("warning(push)")
#   define GP_WARNING_POP()             _Pragma("warning(pop)")
#else
#   error Unknown compiller
#endif
