#include "GpSystemInfo.hpp"

#include <thread>
#include <GpCore2/Config/IncludeExt/fmt.hpp>

#include "../Exceptions/GpException.hpp"

#if defined(GP_OS_LINUX)
#   include <sys/utsname.h>
#endif// #if defined(GP_OS_LINUX)

#if defined(GP_OS_WINDOWS)
#   include "GpSystemInfoWin.hpp"
#endif// #if defined(GP_OS_WINDOWS)

namespace GPlatform {

std::string GpSystemInfo::SOsName (void)
{
    return GP_OS_NAME;
}

std::string GpSystemInfo::SOsInfo (void)
{
    std::string info;

#if defined(GP_OS_LINUX)
    struct utsname buffer;

    THROW_COND_GP
    (
        uname(&buffer) == 0,
        "Failed to call uname()"_sv
    );

    info = fmt::format
    (
        "Name: {}. Machine: {}. Release: {}. Version: {}",
        buffer.sysname,
        buffer.machine,
        buffer.release,
        buffer.version
    );
#elif defined GP_OS_WINDOWS

    // Architecture
    std::string osArchitecture;
    {
        SYSTEM_INFO si;
        GetNativeSystemInfo(&si);

        switch (si.wProcessorArchitecture)
        {
            case PROCESSOR_ARCHITECTURE_INTEL:
            {
                osArchitecture = "x86";
            } break;
            case PROCESSOR_ARCHITECTURE_AMD64:
            {
                osArchitecture = "x86_64";
            } break;
            case PROCESSOR_ARCHITECTURE_ARM:
            {
                osArchitecture = "arm";
            } break;
            case PROCESSOR_ARCHITECTURE_ARM64:
            {
                osArchitecture = "arm64";
            } break;
            case PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64:
            {
                osArchitecture = "arm32_on_win64";
            } break;
            default:
            {
                osArchitecture = "Unknown";
            }
        }
    }

    // Build version
    DWORD dwBuildNumber = 0;
    {
        OSVERSIONINFOEX osvi;

        ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

        GetVersionEx((OSVERSIONINFO*)&osvi);

        dwBuildNumber = osvi.dwBuildNumber;
    }

    info = fmt::format
    (              
        "{} (build {}), architecture {}",
        GpSystemInfoWin::SWindowsVersion(),
        dwBuildNumber,
        osArchitecture
    );
#else
#   error Unknown OS
#endif

    return info;
}

std::string GpSystemInfo::SArcName (void)
{
    return GP_ARCH_NAME;
}

size_t  GpSystemInfo::SHardwareConcurrency (void)
{
    return std::thread::hardware_concurrency();
}

}// namespace GPlatform
