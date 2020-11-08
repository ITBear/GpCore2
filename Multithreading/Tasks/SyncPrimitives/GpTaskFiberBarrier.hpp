#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../Fibers/GpTaskFiber.hpp"
#include "../../SyncPrimitives/GpSpinlock.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiberBarrier
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTaskFiberBarrier)
    CLASS_DECLARE_DEFAULTS(GpTaskFiberBarrier)

public:
    inline                      GpTaskFiberBarrier      (void) noexcept;
    inline                      ~GpTaskFiberBarrier     (void) noexcept;

    inline void                 Acquire                 (void) noexcept;
    void                        Release                 (void);

    void                        Wait                    (GpTaskFiber::SP aTask);

private:
    void                        WakeupAll               (void);

private:
    std::atomic_intmax_t        iCounter;
    GpSpinlock                  iWakeupLock;
    GpTaskFiber::C::Vec::SP     iWakeupOnAllReleasedTasks;
};

GpTaskFiberBarrier::GpTaskFiberBarrier (void) noexcept
{
}

GpTaskFiberBarrier::~GpTaskFiberBarrier (void) noexcept
{
}

void    GpTaskFiberBarrier::Acquire (void) noexcept
{
    iCounter.fetch_add(1, std::memory_order_release);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
