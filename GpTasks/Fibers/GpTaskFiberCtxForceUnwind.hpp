#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../GpTasks_global.hpp"

#include <GpCore2/GpUtils/Exceptions/GpException.hpp>

namespace GPlatform {

class GP_TASKS_API GpTaskFiberCtxForceUnwind final: public GpException
{
public:
                                        GpTaskFiberCtxForceUnwind   (void) noexcept = default;
    inline                              GpTaskFiberCtxForceUnwind   (const GpTaskFiberCtxForceUnwind& aException);
    inline                              GpTaskFiberCtxForceUnwind   (GpTaskFiberCtxForceUnwind&& aException);
    inline                              GpTaskFiberCtxForceUnwind   (std::string_view       aMsg,
                                                                     const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;
    virtual                             ~GpTaskFiberCtxForceUnwind  (void) noexcept override final;

    inline GpTaskFiberCtxForceUnwind&   operator=                   (const GpTaskFiberCtxForceUnwind&   aException);
    inline GpTaskFiberCtxForceUnwind&   operator=                   (GpTaskFiberCtxForceUnwind&&        aException);
};

GpTaskFiberCtxForceUnwind::GpTaskFiberCtxForceUnwind (const GpTaskFiberCtxForceUnwind& aException):
GpException(aException)
{
}

GpTaskFiberCtxForceUnwind::GpTaskFiberCtxForceUnwind (GpTaskFiberCtxForceUnwind&& aException):
GpException(std::move(aException))
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
