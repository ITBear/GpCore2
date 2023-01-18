#include "GpException.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "GpExceptionUtils.hpp"

#include <signal.h>
#include <execinfo.h>

namespace GPlatform{

GpException::GpException
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept
try
{
    auto res = GpExceptionUtils::SToString(aMsg, aSourceLocation, GpExceptionUtils::ExceptionType::GP);

    iWhat           = std::move(res.fullMessage);
    iMsg            = std::move(res.message);
    iSourceLocation = aSourceLocation;

#if defined(GP_POSIX)
    void*       ptrs[100];
    const int   size = backtrace(ptrs, 100);

    fprintf(stderr, "Exception:\n");
    backtrace_symbols_fd(ptrs, size, STDERR_FILENO);
#else
#   error Unsupported platform
#endif
} catch(...)
{
}

GpException::~GpException (void) noexcept
{
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
