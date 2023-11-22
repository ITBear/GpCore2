#include "GpSystemInfo.hpp"

#include <thread>
#include <fmt/include/fmt/core.h>

#include "../Exceptions/GpException.hpp"

#if defined(GP_OS_LINUX)
#   include <sys/utsname.h>
#endif// #if defined(GP_OS_LINUX)

namespace GPlatform {

std::u8string   GpSystemInfo::SOsName (void)
{
    return GP_OS_NAME;
}

std::u8string   GpSystemInfo::SOsInfo (void)
{
    std::u8string info;

#if defined(GP_OS_LINUX)
    struct utsname buffer;

    THROW_COND_GP
    (
        uname(&buffer) == 0,
        u8"Failed to call uname()"_sv
    );

    info = GpUTF::S_STR_To_UTF8
    (
        fmt::format
        (
            "Name: {}. Machine: {}. Release: {}. Version: {}",
            buffer.sysname,
            buffer.machine,
            buffer.release,
            buffer.version
        )
    );
#else
#   error Unknown OS
#endif

    return info;
}

std::u8string   GpSystemInfo::SArcName (void)
{
    return GP_ARCH_NAME;
}

size_t  GpSystemInfo::SHardwareConcurrency (void)
{
    return std::thread::hardware_concurrency();
}

}//namespace GPlatform
