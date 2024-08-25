#include <GpCore2/GpUtils/Exceptions/GpExceptionCode.hpp>

#if defined(GP_USE_EXCEPTIONS)

namespace GPlatform {

GpExceptionCode::GpExceptionCode
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept:
GpException{aMsg, aSourceLocation}
{
    //GpDebugging::SBreakpoint();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
