#include <GpCore2/GpUtils/Exceptions/GpException.hpp>

#if defined(GP_USE_EXCEPTIONS)

#include <GpCore2/GpUtils/Exceptions/GpExceptionUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>

#if defined(GP_PRINT_EXCEPTIONS_STACKTRACE)
#   include <GpCore2/GpUtils/Debugging/GpStackTrace.hpp>
#endif// #if defined(GP_PRINT_EXCEPTIONS_STACKTRACE)

#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>

#if defined(GP_POSIX)
//# include <signal.h>
#   include <execinfo.h>
#endif//

namespace GPlatform {

GP_WARNING_PUSH()

#if defined(GP_OS_WINDOWS)
#   pragma warning(disable : 4297)
#endif// #if defined(GP_OS_WINDOWS)

GpException::GpException (const GpException& aException):
iWhat          {aException.iWhat},
iMsg           {aException.iMsg},
iSourceLocation{aException.iSourceLocation}
{
    //GpDebugging::SBreakpoint();
}

GpException::GpException (GpException&& aException) noexcept:
iWhat          {std::move(aException.iWhat)},
iMsg           {std::move(aException.iMsg)},
iSourceLocation{aException.iSourceLocation}//do not std::move
{
    //GpDebugging::SBreakpoint();
}

GpException::GpException
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept
try
{
    //GpDebugging::SBreakpoint();
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
    GpOutUtils::S().Err("[GpException::GpException]: "_sv + ex.what());
    std::terminate();
} catch(...)
{
    GpOutUtils::S().Err("[GpException::GpException]: unknown exception"_sv);
    std::terminate();
}

GP_WARNING_POP()

GpException::~GpException (void) noexcept
{
}

GpException&    GpException::operator= (const GpException&  aException)
{
    iWhat           = aException.iWhat;
    iMsg            = aException.iMsg;
    iSourceLocation = aException.iSourceLocation;

    return *this;
}

GpException&    GpException::operator= (GpException&& aException) noexcept
{
    iWhat           = std::move(aException.iWhat);
    iMsg            = std::move(aException.iMsg);
    iSourceLocation = aException.iSourceLocation;

    return *this;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
