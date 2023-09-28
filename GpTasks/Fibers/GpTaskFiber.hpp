#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../GpTask.hpp"
#include "GpTaskFiberCtx.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskFiber: public GpTask
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFiber)
    CLASS_DD(GpTaskFiber)

public:
    inline                              GpTaskFiber     (void) noexcept;
    inline                              GpTaskFiber     (std::u8string aName) noexcept;
    virtual                             ~GpTaskFiber    (void) noexcept override;

    inline static GpTaskFiber&          SCurrentFiber   (void);
    inline static void                  SYeld           (const GpTaskRunRes::EnumT aValue);
    inline static void                  SYeld           (const milliseconds_t aTimeout);

    GpTaskRunRes::EnumT                 FiberRun        (void); // Call from GpTaskFiberCtx only
    void                                CallStop        (void); // Call from GpTaskFiber, GpTaskFiberCtx only

protected:
    virtual GpTaskRunRes::EnumT         Run             (void) noexcept override final;

    virtual void                        OnStart         (void) = 0;             // Calls once, before first call OnStep
    virtual GpTaskRunRes::EnumT         OnStep          (void) = 0;             // Calls until return DONE or exception or IsStopRequested() == true
    virtual std::optional<GpException>  OnStop          (void) noexcept = 0;    // It is called once before finishing in the following cases:
                                                                        // 1. In the event of an exception in OnStart, OnStep, or any other location.
                                                                        // 2. If OnStep returns DONE
private:
    std::optional<GpException>          ClearCtx        (void) noexcept;

private:
    GpTaskFiberCtx::SP                  iCtx;
    bool                                iIsStartCalled  = false;
    bool                                iIsStopCalled   = false;
};

GpTaskFiber::GpTaskFiber (void) noexcept:
GpTask(GpTaskMode::FIBER)
{
}

GpTaskFiber::GpTaskFiber (std::u8string aName) noexcept:
GpTask
(
    std::move(aName),
    GpTaskMode::FIBER
)
{
}

GpTaskFiber&    GpTaskFiber::SCurrentFiber (void)
{
    GpTask::C::Opt::Ref currentTaskOpt = GpTask::SCurrentTask();

    if (!currentTaskOpt.has_value()) [[unlikely]]
    {
        THROW_GP(u8"Call Yeld from outside fiber"_sv);
    }

    GpTask& currentTask = currentTaskOpt.value();

    if (currentTask.Mode() != GpTaskMode::FIBER) [[unlikely]]
    {
        THROW_GP(u8"Call Yeld from not fiber task"_sv);
    }

    return static_cast<GpTaskFiber&>(currentTask);
}

void    GpTaskFiber::SYeld (const GpTaskRunRes::EnumT aValue)
{
    SCurrentFiber().iCtx.Vn().Yield(aValue);
}

void    GpTaskFiber::SYeld (const milliseconds_t aTimeout)
{
    SCurrentFiber().iCtx.Vn().Yield(aTimeout);
}

inline void YELD (const GpTaskRunRes::EnumT aValue)
{
    GpTaskFiber::SYeld(aValue);
}

inline void YELD_WAIT (void)
{
    GpTaskFiber::SYeld(GpTaskRunRes::WAIT);
}

inline void YELD_WAIT (const milliseconds_t aTimeout)
{
    GpTaskFiber::SYeld(aTimeout);
}

inline void YELD_READY_TO_RUN (void)
{
    YELD(GpTaskRunRes::READY_TO_RUN);
}

inline void YELD_DONE (void)
{
    YELD(GpTaskRunRes::DONE);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
