#include "GpTaskFiberBarrier.hpp"
#include <mutex>

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

GpTaskFiberBarrier::~GpTaskFiberBarrier (void) noexcept
{
}

void    GpTaskFiberBarrier::Release (void) noexcept
{
    const size_t cnt = iCountLeft.fetch_sub(1, std::memory_order_release);

    if (cnt <= 1)
    {
        WakeupAll();
    }
}

void    GpTaskFiberBarrier::WakeupOnAllReleased (GpTaskFiber::WP aTask)
{
    {
        std::scoped_lock lock(iWakeupLock);

        if (CountLeft() == 0_cnt)
        {
            return;
        }

        iWakeupOnAllReleasedTasks.emplace_back(GpTaskFiber::SP(aTask));
    }

    bool fin;
    do
    {
        GpTaskFiberCtx::SYeld(GpTask::Res::WAITING);

        {
            std::scoped_lock lock(iWakeupLock);
            fin = CountLeft() == 0_cnt;
        }
    } while (!fin);
}

void    GpTaskFiberBarrier::WakeupAll (void) noexcept
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
