#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiber.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiberBase: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskFiberBase)
    CLASS_DECLARE_DEFAULTS(GpTaskFiberBase)

    using EventOptRefT  = std::optional<std::reference_wrapper<GpEvent>>;

protected:
                            GpTaskFiberBase         (std::string_view aName);
    virtual                 ~GpTaskFiberBase        (void) noexcept override;

    virtual void            FiberFn                 (GpThreadStopToken aStopToken) override final;

protected:
    virtual void            OnStart                 (void) = 0;
    virtual GpTask::ResT    OnStep                  (EventOptRefT aEvent) = 0;
    virtual void            OnStop                  (void) noexcept = 0;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
