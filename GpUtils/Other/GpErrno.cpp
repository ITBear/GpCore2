#include <GpCore2/GpUtils/Other/GpErrno.hpp>
#include <GpCore2/GpUtils/Types/Bits/GpBitOps.hpp>

#if defined(GP_OS_WINDOWS)
#   include <GpCore2/Config/IncludeExt/windows.hpp>
#endif// #if defined(GP_OS_WINDOWS)

namespace GPlatform {

#if defined(GP_OS_WINDOWS)

std::string GpErrno::SWinGetAndClear (void)
{
    // Get the error message, if any.
    const DWORD code = ::GetLastError();
    if (code == 0)
    {
        return std::string(); // No error message has been recorded
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

#if defined(GP_OS_MACOS)
std::string GpErrno::SStatusToStr (OSStatus aStatus)
{
    std::string s;

    // Convert to a 4-character code if it fits
    const s_int_32 status = BitOps::H2N(static_cast<s_int_32>(aStatus));
    std::array<char, sizeof(s_int_32)> codeBuff;
    std::memcpy(codeBuff.data(), &status, sizeof(s_int_32));

    if (isprint(codeBuff[0]) && isprint(codeBuff[1]) && isprint(codeBuff[2]) && isprint(codeBuff[3]))
    {
        s.resize(4);

        s[0] = codeBuff[0];
        s[1] = codeBuff[1];
        s[2] = codeBuff[2];
        s[3] = codeBuff[3];
    } else {
        s = StrOps::SFromSI64(aStatus);
    }

    return s;
}
#endif// #if defined(GP_OS_MACOS)

}// namespace GPlatform
