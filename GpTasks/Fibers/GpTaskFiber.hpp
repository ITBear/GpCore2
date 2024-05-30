#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Other/GpMethodAccessGuard.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../GpTask.hpp"
#include "GpTaskFiberCtx.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskFiber: public GpTask
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFiber)
    CLASS_DD(GpTaskFiber)
    TAG_SET(THREAD_SAFE)

public:
    inline                      GpTaskFiber     (void) noexcept;
    inline                      GpTaskFiber     (std::string aName) noexcept;
    virtual                     ~GpTaskFiber    (void) noexcept override;

    static GpTaskFiber&         SCurrentFiber   (void);
    inline static void          SYeld           (const GpTaskRunRes::EnumT aValue);
    inline static void          SYeld           (const milliseconds_t aTimeout);

    GpTaskRunRes::EnumT         FiberRun        (GpMethodAccessGuard<GpTaskFiberCtx>);
    void                        CallOnStop      (GpMethodAccessGuard<GpTaskFiber, GpTaskFiberCtx>);

protected:
    bool                        IsStartCalled   (void) const noexcept {return iIsStartCalled;}
    bool                        IsStopCalled    (void) const noexcept {return iIsStopCalled;}

    virtual GpTaskRunRes::EnumT Run             (void) noexcept override final;

    virtual void                OnStart         (void) = 0;             // Calls once, before first call OnStep
    virtual GpTaskRunRes::EnumT OnStep          (void) = 0;             // Calls until return DONE or exception or IsStopRequested() == true
    virtual GpException::C::Opt OnStop          (void) noexcept = 0;    // It is called once before finishing in the following cases:
                                                                        // 1. In the event of an exception in OnStart, OnStep, or any other location.
                                                                        // 2. If OnStep returns DONE
private:
    GpException::C::Opt         ClearCtx        (void) noexcept;

private:
    GpTaskFiberCtx::SP          iCtx;
    bool                        iIsStartCalled  = false;
    bool                        iIsStopCalled   = false;
};

GpTaskFiber::GpTaskFiber (void) noexcept:
GpTask(GpTaskMode::FIBER)
{
}

GpTaskFiber::GpTaskFiber (std::string aName) noexcept:
GpTask
(
    std::move(aName),
    GpTaskMode::FIBER
)
{
}

void    GpTaskFiber::SYeld (const GpTaskRunRes::EnumT aValue)
{
    SCurrentFiber().iCtx.Vn().CallYield(aValue);
}

void    GpTaskFiber::SYeld (const milliseconds_t aTimeout)
{
    SCurrentFiber().iCtx.Vn().CallYield(aTimeout);
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

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
