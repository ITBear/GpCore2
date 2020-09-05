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
    CLASS_REMOVE_CTRS(GpTaskFiberBarrier)
    CLASS_DECLARE_DEFAULTS(GpTaskFiberBarrier)

public:
    inline                      GpTaskFiberBarrier      (const count_t aCountToRelease) noexcept;
                                ~GpTaskFiberBarrier     (void) noexcept;

    //for std::scoped_lock use
    void                        lock                    (void) noexcept {/*NOP*/}
    void                        unlock                  (void) noexcept {Release();}
    bool                        try_lock                (void) noexcept {return true;}

    inline count_t              CountLeft               (void) const noexcept;
    void                        Release                 (void) noexcept;
    void                        WakeupOnAllReleased     (GpTaskFiber::WP aTask);

private:
    void                        WakeupAll               (void) noexcept;

private:
    std::atomic_size_t          iCountLeft;
    GpSpinlock                  iWakeupLock;
    GpTaskFiber::C::Vec::SP     iWakeupOnAllReleasedTasks;
};

GpTaskFiberBarrier::GpTaskFiberBarrier (const count_t aCountToRelease) noexcept:
iCountLeft(aCountToRelease.ValueAs<size_t>())
{
}

count_t GpTaskFiberBarrier::CountLeft (void) const noexcept
{
    const size_t cnt = iCountLeft.load(std::memory_order_acquire);
    return count_t::SMake(cnt);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
