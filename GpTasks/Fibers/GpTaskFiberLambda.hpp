#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberBase.hpp"

namespace GPlatform {

/*class GP_TASKS_API GpTaskFiberLambda final: public GpTaskFiberBase
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskFiberLambda)
    CLASS_DD(GpTaskFiberLambda)

    using FnT       = std::function<void(GpTask& aTask)>;
    using OnStepFnT = std::function<GpTaskDoRes(EventOptRefT aEvent)>;

public:
                                GpTaskFiberLambda       (std::u8string              aName,
                                                         std::optional<FnT>         aBeforeStartFn,
                                                         std::optional<OnStepFnT>   aOnStepFn,
                                                         std::optional<FnT>         aAfterStopFn) noexcept;
    virtual                     ~GpTaskFiberLambda      (void) noexcept override final;

protected:
    virtual void                OnStart                 (void) override final;
    virtual GpTaskDoRes         OnStep                  (EventOptRefT aEvent) override final;
    virtual void                OnStop                  (void) noexcept override final;

private:
    std::optional<FnT>          iBeforeStartFn;
    std::optional<OnStepFnT>    iOnStepFn;
    std::optional<FnT>          iAfterStopFn;
};*/

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
