#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberBase.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiberLambda final: public GpTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS(GpTaskFiberLambda)
    CLASS_DECLARE_DEFAULTS(GpTaskFiberLambda)

    using OnStartFn = std::function<void()>;
    using OnStepFn  = std::function<ResT(EventOptRefT aEvent)>;
    using OnStoptFn = std::function<void()>;

public:
                                GpTaskFiberLambda       (std::optional<OnStartFn>   aOnStartFn,
                                                         std::optional<OnStepFn>    aOnStepFn,
                                                         std::optional<OnStoptFn>   aOnStoptFn);
    virtual                     ~GpTaskFiberLambda      (void) noexcept override final;

protected:
    virtual void                OnStart                 (void) override final;
    virtual GpTask::ResT        OnStep                  (EventOptRefT aEvent) override final;
    virtual void                OnStop                  (void) noexcept override final;

private:
    std::optional<OnStartFn>    iOnStartFn;
    std::optional<OnStepFn>     iOnStepFn;
    std::optional<OnStoptFn>    iOnStoptFn;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
