#include "GpTaskFiberBarrier.hpp"
#include <mutex>

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {


void    GpTaskFiberBarrier::Release (void)
{
    const intmax_t cnt = iCounter.fetch_sub(1, std::memory_order_release);

    if (cnt == 1)
    {
        WakeupAll();
    }
}

void    GpTaskFiberBarrier::Wait (GpTaskFiber::SP aTask)
{
    //Subscribe
    {
        std::scoped_lock lock(iWakeupLock);

        if (iCounter.load(std::memory_order_acquire) <= 0)
        {
            return;
        }

        iWakeupOnAllReleasedTasks.emplace_back(std::move(aTask));
    }

    //Wait
    while (iCounter.load(std::memory_order_acquire) > 0)
    {
        GpTaskFiberCtx::SYeld(GpTask::ResT::WAITING);
    }
}

void    GpTaskFiberBarrier::WakeupAll (void)
{
    std::scoped_lock lock(iWakeupLock);

    for (GpTaskFiber::SP& t: iWakeupOnAllReleasedTasks)
    {
        t.Vn().MoveToReady();
    }

    iWakeupOnAllReleasedTasks.clear();
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
