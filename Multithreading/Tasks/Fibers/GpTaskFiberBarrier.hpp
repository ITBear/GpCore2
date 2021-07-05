#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiber.hpp"
#include "../../SyncPrimitives/GpSpinlock.hpp"
#include <any>

namespace GPlatform {

class GPCORE_API GpTaskFiberBarrier
{
public:
    CLASS_REMOVE_CTRS(GpTaskFiberBarrier)
    CLASS_DECLARE_DEFAULTS(GpTaskFiberBarrier)

    using ResultT = GpVector<std::optional<std::any>>;

public:
                                    GpTaskFiberBarrier      (count_t aCounter) noexcept;
                                    ~GpTaskFiberBarrier     (void) noexcept;

    void                            Release                 (std::optional<std::any>&& aResult) noexcept;
    const ResultT&                  Wait                    (void);

private:
    void                            WakeupAll               (void);

private:
    count_t                         iCounter = 0_cnt;
    GpSpinlock                      iWakeupLock;
    GpTaskFiber::C::Vec::SP         iWakeupOnAllReleasedTasks;
    ResultT                         iResult;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
