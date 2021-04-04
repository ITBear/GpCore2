#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiber.hpp"
#include "../../SyncPrimitives/GpSpinlock.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiberBarrier
{
public:
    CLASS_REMOVE_CTRS(GpTaskFiberBarrier)
    CLASS_DECLARE_DEFAULTS(GpTaskFiberBarrier)

public:
                                GpTaskFiberBarrier      (count_t aCounter) noexcept:iCounter(aCounter) {}
                                ~GpTaskFiberBarrier     (void) noexcept {}

    void                        Release                 (void);
    void                        Wait                    (GpTaskFiber::SP aTask);

private:
    void                        WakeupAll               (void);

private:
    count_t                     iCounter = 0_cnt;
    GpSpinlock                  iWakeupLock;
    GpTaskFiber::C::Vec::SP     iWakeupOnAllReleasedTasks;  
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
