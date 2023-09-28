#include "GpException.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "GpExceptionUtils.hpp"
#include "../Types/Strings/GpStringOps.hpp"
#include "../Types/Strings/GpStringUtils.hpp"

#if defined(GP_POSIX)
#   include <signal.h>
#   include <execinfo.h>
#endif//

namespace GPlatform{

GpException::GpException
(
    std::u8string_view      aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept
try
{
#if defined(GP_PRINT_EXCEPTIONS_STACKTRACE)
    std::u8string stackTraceStr;
    stackTraceStr.reserve(512);
    stackTraceStr.append(u8"\nStack trace:");

#   if defined(GP_POSIX)
    void*       ptrs[100];
    const int   size = backtrace(ptrs, 100);

    char** stackTracePtrs = backtrace_symbols(ptrs, size);

    for (int id = 0; id < size; id++)
    {
        stackTraceStr
            .append(u8"\n    [")
            .append(StrOps::SToString(size - id))
            .append(u8"]: ")
            .append(GpUTF::S_STR_To_UTF8(stackTracePtrs[id]));
    }

    std::free(stackTracePtrs);
#   else
#       error Unsupported platform
#   endif//

    auto res = GpExceptionUtils::SToString(aMsg, aSourceLocation, GpExceptionUtils::ExceptionType::GP, stackTraceStr);
#else
    auto res = GpExceptionUtils::SToString(aMsg, aSourceLocation, GpExceptionUtils::ExceptionType::GP, std::nullopt);
#endif//#if defined(GP_PRINT_EXCEPTIONS_STACKTRACE)

    iWhat           = std::move(res.fullMessage);
    iMsg            = std::move(res.message);
    iSourceLocation = aSourceLocation;
} catch(const std::exception& ex)
{
    GpStringUtils::SCerr(u8"[GpException::GpException]: "_sv + ex.what());
    std::terminate();
} catch(...)
{
    GpStringUtils::SCerr(u8"[GpException::GpException]: unknown exception"_sv);
    std::terminate();
}

GpException::~GpException (void) noexcept
{
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
