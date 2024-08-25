#pragma once

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

#include <errno.h>
#include <cstring>

namespace GPlatform {

class GP_UTILS_API GpErrno
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpErrno)

public:
    static inline std::string_view  SGetAndClear (void)
    {
        std::string_view sv = std::strerror(errno);
        errno = 0;
        return sv;
    }

#if defined(GP_OS_WINDOWS)
    static std::string  SWinGetAndClear (void);
#endif// #if defined(GP_OS_WINDOWS)
};

}// namespace GPlatform
