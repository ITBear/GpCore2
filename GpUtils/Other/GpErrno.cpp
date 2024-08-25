#include <GpCore2/GpUtils/Other/GpErrno.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>

#if defined(GP_OS_WINDOWS)
#   include <GpCore2/Config/IncludeExt/windows.hpp>
#endif// #if defined(GP_OS_WINDOWS)

namespace GPlatform {

#if defined(GP_OS_WINDOWS)

std::string GpErrno::SWinGetAndClear (void)
{
    //Get the error message, if any.
    const DWORD code = ::GetLastError();
    if (code == 0)
    {
        return std::string(); //No error message has been recorded
    }

    LPSTR buff = nullptr;
    const size_t size = FormatMessageA
    (
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        LPSTR(&buff),
        0,
        nullptr
    );

    std::string message(buff, size);
    LocalFree(buff);
    return message;
}

#endif// #if defined(GP_OS_WINDOWS)

}// namespace GPlatform
