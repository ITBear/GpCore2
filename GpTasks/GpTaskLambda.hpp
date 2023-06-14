#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskBase.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskLambda final: public GpTaskBase
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskLambda)
    CLASS_DD(GpTaskLambda)

    using OnStartFnT    = std::function<void()>;
    using OnStepFnT     = std::function<GpTaskDoRes(EventOptRefT aEvent)>;
    using OnStoptFnT    = std::function<void()>;

public:
                                GpTaskLambda    (std::optional<OnStartFnT>  aOnStartFn,
                                                 std::optional<OnStepFnT>   aOnStepFn,
                                                 std::optional<OnStoptFnT>  aOnStoptFn);
    virtual                     ~GpTaskLambda   (void) noexcept override final;

protected:
    virtual void                OnStart         (void) override final;
    virtual GpTaskDoRes         OnStep          (EventOptRefT aEvent) override final;
    virtual void                OnStop          (void) noexcept override final;

private:
    std::optional<OnStartFnT>   iOnStartFn;
    std::optional<OnStepFnT>    iOnStepFn;
    std::optional<OnStoptFnT>   iOnStoptFn;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
