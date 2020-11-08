#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Exceptions/GpException.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiberStopEx final: public GpException
{
public:
private:
                            GpTaskFiberStopEx   (void) noexcept = delete;
                            GpTaskFiberStopEx   (const GpTaskFiberStopEx&   aException) noexcept = delete;

    GpTaskFiberStopEx&      operator=           (const GpTaskFiberStopEx& aException) noexcept = delete;
    GpTaskFiberStopEx&      operator=           (GpTaskFiberStopEx&& aException) noexcept = delete;

public:
                            GpTaskFiberStopEx   (GpTaskFiberStopEx&& aException) noexcept;
                            GpTaskFiberStopEx   (std::string_view       aMsg,
                                                 const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;

    virtual                 ~GpTaskFiberStopEx  (void) noexcept override final;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
