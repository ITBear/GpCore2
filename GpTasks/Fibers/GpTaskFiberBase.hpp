#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiber.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskFiberBase: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskFiberBase)
    CLASS_DD(GpTaskFiberBase)

    using EventOptRefT  = std::optional<std::reference_wrapper<GpEvent>>;

protected:
    inline              GpTaskFiberBase     (std::string aName) noexcept;
    virtual             ~GpTaskFiberBase    (void) noexcept override;

protected:
    virtual void        FiberFn             (GpThreadStopToken aStopToken) override final;

    virtual void        OnStart             (void) = 0;
    virtual GpTaskDoRes OnStep              (EventOptRefT aEvent) = 0;
    virtual void        OnStop              (void) noexcept = 0;
};

GpTaskFiberBase::GpTaskFiberBase (std::string aName) noexcept:
GpTaskFiber(std::move(aName))
{
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
