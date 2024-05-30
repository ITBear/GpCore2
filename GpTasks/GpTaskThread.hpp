#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "GpTask.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskThread: public GpTask
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskThread)
    CLASS_DD(GpTaskThread)

protected:
    inline                          GpTaskThread    (std::string aName) noexcept;
    inline                          GpTaskThread    (void) noexcept;

public:
    virtual                         ~GpTaskThread   (void) noexcept = default;

protected:
    virtual GpTaskRunRes::EnumT     Run             (void) noexcept override final;

    virtual void                    OnStart         (void) = 0;             // Calls once, before first call OnStep
    virtual GpTaskRunRes::EnumT     OnStep          (void) = 0;             // Calls until return DONE or exception or IsStopRequested() == true
    virtual GpException::C::Opt     OnStop          (void) noexcept = 0;    // It is called once before finishing in the following cases:
                                                                                // 1. In the event of an exception in OnStart, OnStep, or any other location.
                                                                                // 2. If OnStep returns DONE

private:
    GpException::C::Opt             CallOnStop      (void) noexcept;

private:
    bool                            iIsStartCalled  = false;
    bool                            iIsStopCalled   = false;
};

GpTaskThread::GpTaskThread (std::string aName) noexcept:
GpTask
(
    std::move(aName),
    GpTaskMode::THREAD
)
{
}

GpTaskThread::GpTaskThread (void) noexcept:
GpTask(GpTaskMode::THREAD)
{
}

}// namespace GPlatform
