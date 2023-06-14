#include "GpSystemInfo.hpp"

namespace GPlatform {

std::u8string   GpSystemInfo::SOsName (void)
{
    return GP_OS_NAME;
}

std::u8string   GpSystemInfo::SArcName (void)
{
    return GP_ARCH_NAME;
}

}//namespace GPlatform
