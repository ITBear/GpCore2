#include "GpErrno.hpp"

namespace GPlatform {

#if defined(GP_OS_WINDOWS)
std::u8string   GpErrno::SWinGetAndClear (void)
{
    //Get the error message, if any.
    const DWORD code = ::GetLastError();
    if (code == 0)
    {
        return std::u8string(); //No error message has been recorded
    }

    LPSTR buff = nullptr;
    const size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                       NULL,
                                       code,
                                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                       LPSTR(&buff),
                                       0,
                                       NULL);

    std::u8string message(buff, size);
    LocalFree(buff);
    return message;
}
#endif//#if defined(GP_OS_WINDOWS)

}//namespace GPlatform
