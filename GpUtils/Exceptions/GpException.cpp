#include "GpException.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "GpExceptionUtils.hpp"

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
} catch(...)
{
}

GpException::~GpException (void) noexcept
{
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
