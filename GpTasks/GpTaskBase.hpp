#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTask.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskBase: public GpTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskBase)
    CLASS_DD(GpTaskBase)

    using EventOptRefT  = std::optional<std::reference_wrapper<GpEvent>>;

protected:
    inline              GpTaskBase  (std::u8string aName) noexcept;
    virtual             ~GpTaskBase (void) noexcept override;

protected:
    virtual GpTaskDoRes _Run        (GpThreadStopToken aStopToken) noexcept override final;

    virtual void        OnStart     (void) = 0;
    virtual GpTaskDoRes OnStep      (EventOptRefT aEvent) = 0;
    virtual void        OnStop      (void) noexcept = 0;

private:
    bool                iIsStarted = false;
};

GpTaskBase::GpTaskBase (std::u8string aName) noexcept:
GpTask
(
    std::move(aName),
    GpTaskType::THREAD
)
{
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
