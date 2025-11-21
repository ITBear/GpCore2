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

    using ExceptionsT = GpStartStopManager::ExceptionsT;

protected:
                                GpTaskFiber     (void) noexcept;
                                GpTaskFiber     (std::string aName) noexcept;

public:
    virtual                     ~GpTaskFiber    (void) noexcept override;

    static GpTaskFiber::SP      SCurrentFiber   (void);
    static void                 SYield          (GpTaskRunRes::EnumT aValue);
    [[nodiscard]] static GpTaskFiberCtx::TimeoutRes
                                SYield          (milliseconds_t aTimeout);

    // For use in GpTaskFiberCtx
    GpTaskRunRes::EnumT         FiberRun        (GpMethodAccessGuard<GpTaskFiberCtx>);
    void                        CallOnStop      (GpMethodAccessGuard<GpTaskFiberCtx>) noexcept;

protected:
    virtual GpTaskRunRes::EnumT Run             (void) noexcept override final;

    virtual void                OnStart         (void) = 0;             // Calls once, before first call OnStep
    virtual GpTaskRunRes::EnumT OnStep          (void) = 0;             // Calls until return DONE or exception or IsStopRequested() == true
    virtual void                OnStop          (ExceptionsT& aStopExceptionsOut) noexcept = 0; // It is called once before finishing in the following cases:
                                                                        // 1. In the event of an exception in OnStart, OnStep, or any other location.
                                                                        // 2. If OnStep returns DONE
    virtual void                OnStopException (const GpException& aException) noexcept = 0;

private:
    void                        CallOnStop      (ExceptionsT& aStopExceptionsOut) noexcept;
    GpException::C::Opt         ClearCtx        (void) noexcept;

private:
    GpTaskFiberCtx::UP  iCtxUP;
    bool                iIsStartCalled  = false;
    bool                iIsStopCalled   = false;
};

inline void YIELD (const GpTaskRunRes::EnumT aValue)
{
    GpTaskFiber::SYield(aValue);
}

inline void YIELD_WAIT (void)
{
    GpTaskFiber::SYield(GpTaskRunRes::WAIT);
}

[[nodiscard]] inline GpTaskFiberCtx::TimeoutRes YIELD_WAIT (const milliseconds_t aTimeout)
{
    return GpTaskFiber::SYield(aTimeout);
}

inline void YIELD_READY_TO_RUN (void)
{
    YIELD(GpTaskRunRes::READY_TO_RUN);
}

inline void YIELD_DONE (void)
{
    YIELD(GpTaskRunRes::DONE);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
