#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../GpTasks_global.hpp"
#include "../../GpUtils/Exceptions/GpException.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskFiberCtxForceUnwind final: public GpException
{
public:
                                        GpTaskFiberCtxForceUnwind   (void) noexcept = default;
    inline                              GpTaskFiberCtxForceUnwind   (const GpException& aException);
    inline                              GpTaskFiberCtxForceUnwind   (GpException&& aException);
    inline                              GpTaskFiberCtxForceUnwind   (std::u8string_view     aMsg,
                                                                     const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;
    inline                              GpTaskFiberCtxForceUnwind   (std::string_view       aMsg,
                                                                     const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;
    virtual                             ~GpTaskFiberCtxForceUnwind  (void) noexcept override final;

    inline GpTaskFiberCtxForceUnwind&   operator=                   (const GpTaskFiberCtxForceUnwind&   aException);
    inline GpTaskFiberCtxForceUnwind&   operator=                   (GpTaskFiberCtxForceUnwind&&        aException);
};

GpTaskFiberCtxForceUnwind::GpTaskFiberCtxForceUnwind (const GpException& aException):
GpException(aException)
{
}

GpTaskFiberCtxForceUnwind::GpTaskFiberCtxForceUnwind (GpException&& aException):
GpException(std::move(aException))
{
}

GpTaskFiberCtxForceUnwind::GpTaskFiberCtxForceUnwind
(
    std::u8string_view      aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept:
GpException(aMsg, aSourceLocation)
{
}

GpTaskFiberCtxForceUnwind::GpTaskFiberCtxForceUnwind
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept:
GpException(aMsg, aSourceLocation)
{
}

GpTaskFiberCtxForceUnwind&  GpTaskFiberCtxForceUnwind::operator= (const GpTaskFiberCtxForceUnwind& aException)
{
    GpException::operator=(aException);
    return *this;
}

GpTaskFiberCtxForceUnwind&  GpTaskFiberCtxForceUnwind::operator= (GpTaskFiberCtxForceUnwind&& aException)
{
    GpException::operator=(std::move(aException));
    return *this;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
#endif// #if defined(GP_USE_MULTITHREADING)
