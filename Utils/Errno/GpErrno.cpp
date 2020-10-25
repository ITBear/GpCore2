#include "GpErrno.hpp"
#include <cstring>

namespace GPlatform {

std::string GpErrno::SGetAndClear (void)
{
    std::string str = std::strerror(errno);
    errno = 0;
    return str;
}

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
    const size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                       NULL,
                                       code,
                                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                       LPSTR(&buff),
                                       0,
                                       NULL);

    std::string message(buff, size);
    LocalFree(buff);
    return message;
}
#endif//#if defined(GP_OS_WINDOWS)

}//namespace GPlatform
