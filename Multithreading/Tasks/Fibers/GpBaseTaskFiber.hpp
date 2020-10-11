#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiber.hpp"

namespace GPlatform {

class GPCORE_API GpBaseTaskFiber: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpBaseTaskFiber)
    CLASS_DECLARE_DEFAULTS(GpBaseTaskFiber)

    using EventOptRefT  = std::optional<std::reference_wrapper<GpEvent>>;

protected:
                            GpBaseTaskFiber         (void) noexcept;
    virtual                 ~GpBaseTaskFiber        (void) noexcept override;

protected:
    virtual void            FiberFn                 (GpThreadStopToken aStopToken) override final;

    virtual void            OnStart                 (void) = 0;
    virtual void            OnStep                  (EventOptRefT aEvent) = 0;
    virtual void            OnStop                  (void) noexcept = 0;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
