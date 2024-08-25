#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Other/GpMethodAccessGuard.hpp>
#include <GpCore2/GpUtils/Other/GpStartStopManager.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)

#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtx.hpp>

namespace GPlatform {

class GP_TASKS_API GpTaskFiber: public GpTask
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFiber)
    CLASS_DD(GpTaskFiber)
    TAG_SET(THREAD_SAFE)

    using StopExceptionsT = GpStartStopManager::StopExceptionsT;

protected:
    inline                      GpTaskFiber     (void) noexcept;
    inline                      GpTaskFiber     (std::string aName) noexcept;
    inline                      GpTaskFiber     (GpTaskId aId) noexcept;
    inline                      GpTaskFiber     (std::string    aName,
                                                 GpTaskId       aId) noexcept;

public:
    virtual                     ~GpTaskFiber    (void) noexcept override;

    static GpTaskFiber&         SCurrentFiber   (void);
    inline static void          SYeld           (GpTaskRunRes::EnumT aValue);
    [[nodiscard]] inline static GpTaskFiberCtx::TimeoutRes
                                SYeld           (milliseconds_t aTimeout);

    GpTaskRunRes::EnumT         FiberRun        (GpMethodAccessGuard<GpTaskFiberCtx>);
    void                        CallOnStop      (GpMethodAccessGuard<GpTaskFiber, GpTaskFiberCtx>) noexcept;

protected:
    bool                        IsStartCalled   (void) const noexcept {return iIsStartCalled;}
    bool                        IsStopCalled    (void) const noexcept {return iIsStopCalled;}

    virtual GpTaskRunRes::EnumT Run             (void) noexcept override final;

    virtual void                OnStart         (void) = 0;             // Calls once, before first call OnStep
    virtual GpTaskRunRes::EnumT OnStep          (void) = 0;             // Calls until return DONE or exception or IsStopRequested() == true
    virtual void                OnStop          (StopExceptionsT& aStopExceptionsOut) noexcept = 0; // It is called once before finishing in the following cases:
                                                                        // 1. In the event of an exception in OnStart, OnStep, or any other location.
                                                                        // 2. If OnStep returns DONE
    virtual void                OnStopException (const GpException& aException) noexcept = 0;

private:
    GpException::C::Opt         ClearCtx        (void) noexcept;

private:
    GpTaskFiberCtx::SP          iCtx;
    bool                        iIsStartCalled  = false;
    bool                        iIsStopCalled   = false;
};

GpTaskFiber::GpTaskFiber (void) noexcept:
GpTask{GpTaskMode::FIBER}
{
}

GpTaskFiber::GpTaskFiber (std::string aName) noexcept:
GpTask{std::move(aName), GpTaskMode::FIBER}
{
}

GpTaskFiber::GpTaskFiber (GpTaskId aId) noexcept:
GpTask{GpTaskMode::FIBER, aId}
{
}

GpTaskFiber::GpTaskFiber
(
    std::string aName,
    GpTaskId    aId
) noexcept:
GpTask{std::move(aName), GpTaskMode::FIBER, aId}
{
}

void    GpTaskFiber::SYeld (const GpTaskRunRes::EnumT aValue)
{
    SCurrentFiber().iCtx.Vn().CallYield(aValue);
}

GpTaskFiberCtx::TimeoutRes  GpTaskFiber::SYeld (const milliseconds_t aTimeout)
{
    return SCurrentFiber().iCtx.Vn().CallYield(aTimeout);
}

inline void YELD (const GpTaskRunRes::EnumT aValue)
{
    GpTaskFiber::SYeld(aValue);
}

inline void YELD_WAIT (void)
{
    GpTaskFiber::SYeld(GpTaskRunRes::WAIT);
}

[[nodiscard]] inline GpTaskFiberCtx::TimeoutRes YELD_WAIT (const milliseconds_t aTimeout)
{
    return GpTaskFiber::SYeld(aTimeout);
}

inline void YELD_READY_TO_RUN (void)
{
    YELD(GpTaskRunRes::READY_TO_RUN);
}

inline void YELD_DONE (void)
{
    YELD(GpTaskRunRes::DONE);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
