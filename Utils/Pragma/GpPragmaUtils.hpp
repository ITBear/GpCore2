#pragma once

#include "../../Config/GpConfig.hpp"

#define DIAG_STR(s) #s
#define DIAG_JOINSTR(x,y) DIAG_STR(x ## y)
#ifdef _MSC_VER
#   define DIAG_DO_PRAGMA(x) __pragma (#x)
#   define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(warning(x))
#else
#   define DIAG_DO_PRAGMA(x) _Pragma (#x)
#   define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(compiler diagnostic x)
#endif

#define GP_WARNING_PUSH()           DIAG_PRAGMA(GP_COMPILER_NAME, push)
#define GP_WARNING_POP()            DIAG_PRAGMA(GP_COMPILER_NAME, pop)
#define GP_WARNING_DISABLE(option)  DIAG_PRAGMA(GP_COMPILER_NAME, ignored DIAG_JOINSTR(-W,option))

GP_WARNING_DISABLE(pragmas);
