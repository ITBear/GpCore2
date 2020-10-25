#pragma once

#include "../../GpCore_global.hpp"
#include "../../Types/Classes/GpClassesDefines.hpp"

#include <errno.h>
#include <string>

namespace GPlatform {

class GPCORE_API GpErrno
{
    CLASS_REMOVE_CTRS(GpErrno)

public:
    static std::string  SGetAndClear    (void);

#if defined(GP_OS_WINDOWS)
    static std::string  SWinGetAndClear (void);
#endif//#if defined(GP_OS_WINDOWS)
};

}//namespace GPlatform
