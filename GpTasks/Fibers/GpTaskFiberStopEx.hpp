#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../GpUtils/Exceptions/GpException.hpp"

namespace GPlatform {

class GpTaskFiberStopEx final: public GpException
{
private:
                    GpTaskFiberStopEx   (void) noexcept = delete;

public:
    inline          GpTaskFiberStopEx   (const GpTaskFiberStopEx&   aException);
    inline          GpTaskFiberStopEx   (GpTaskFiberStopEx&&        aException);
    inline          GpTaskFiberStopEx   (std::u8string_view     aMsg,
                                         const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;

    virtual         ~GpTaskFiberStopEx  (void) noexcept override final = default;
};

GpTaskFiberStopEx::GpTaskFiberStopEx (const GpTaskFiberStopEx& aException):
GpException(aException)
{
}

GpTaskFiberStopEx::GpTaskFiberStopEx (GpTaskFiberStopEx&& aException):
GpException(std::move(aException))
{
}

GpTaskFiberStopEx::GpTaskFiberStopEx
(
    std::u8string_view      aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept:
GpException(aMsg, aSourceLocation)
{
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
