#pragma once

#include "../GpUtils_global.hpp"
#include "../Macro/GpMacroClass.hpp"
#include "../Types/Strings/GpUTF.hpp"

#include <errno.h>
#include <string>
#include <cstring>

namespace GPlatform {

class GP_UTILS_API GpErrno
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpErrno)

public:
    static inline std::u8string_view    SGetAndClear (void)
    {
        std::u8string_view sv = GpUTF::S_As_UTF8(std::strerror(errno));
        errno = 0;
        return sv;
    }

#if defined(GP_OS_WINDOWS)
    static std::u8string    SWinGetAndClear (void);
#endif//#if defined(GP_OS_WINDOWS)
};

}//namespace GPlatform
