#include <GpCore2/GpUtils/Exceptions/GpException.hpp>

#if defined(GP_USE_EXCEPTIONS)

#include <GpCore2/GpUtils/Exceptions/GpExceptionUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>

#if defined(GP_PRINT_EXCEPTIONS_STACKTRACE)
#   include "../Debugging/GpStackTrace.hpp"
#endif// #if defined(GP_PRINT_EXCEPTIONS_STACKTRACE)

#if defined(GP_POSIX)
#   include <signal.h>
#   include <execinfo.h>
#endif//

namespace GPlatform {

GP_WARNING_PUSH()

#if defined(GP_OS_WINDOWS)
#   pragma warning(disable : 4297)
#endif// #if defined(GP_OS_WINDOWS)

GpException::GpException
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept
try
{
    std::optional<std::string> stackTraceStrOpt;

#if defined(GP_PRINT_EXCEPTIONS_STACKTRACE)
    stackTraceStrOpt = GpStackTrace::STaceToStr();
#endif// #if defined(GP_PRINT_EXCEPTIONS_STACKTRACE)

    auto res = GpExceptionUtils::SToString
    (
        aMsg,
        aSourceLocation,
        stackTraceStrOpt
    );

    iWhat           = std::move(res.fullMessage);
    iMsg            = std::move(res.message);
    iSourceLocation = aSourceLocation;
} catch(const std::exception& ex)
{
    GpStringUtils::SCerr("[GpException::GpException]: "_sv + ex.what());
    std::terminate();
} catch(...)
{
    GpStringUtils::SCerr("[GpException::GpException]: unknown exception"_sv);
    std::terminate();
}

GP_WARNING_POP()

GpException::~GpException (void) noexcept
{
}

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
