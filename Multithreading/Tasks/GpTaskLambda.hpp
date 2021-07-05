#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskBase.hpp"

namespace GPlatform {

class GPCORE_API GpTaskLambda final: public GpTaskBase
{
public:
    CLASS_REMOVE_CTRS(GpTaskLambda)
    CLASS_DECLARE_DEFAULTS(GpTaskLambda)

    using OnStartFn = std::function<void()>;
    using OnStepFn  = std::function<ResT(EventOptRefT aEvent)>;
    using OnStoptFn = std::function<void()>;

public:
                                GpTaskLambda    (std::optional<OnStartFn>   aOnStartFn,
                                                 std::optional<OnStepFn>    aOnStepFn,
                                                 std::optional<OnStoptFn>   aOnStoptFn);
    virtual                     ~GpTaskLambda   (void) noexcept override final;

protected:
    virtual void                OnStart         (void) override final;
    virtual ResT                OnStep          (EventOptRefT aEvent) override final;
    virtual void                OnStop          (void) noexcept override final;

private:
    std::optional<OnStartFn>    iOnStartFn;
    std::optional<OnStepFn>     iOnStepFn;
    std::optional<OnStoptFn>    iOnStoptFn;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
