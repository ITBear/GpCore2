#include "GpTaskFiberBarrier.hpp"
#include <mutex>

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

void    GpTaskFiberBarrier::Release (void)
{
    std::scoped_lock lock(iWakeupLock);

    if (iCounter > 1_cnt)
    {
        iCounter--;
    } else if (iCounter == 1_cnt)
    {
        iCounter--;
        WakeupAll();
    } else
    {
        THROW_GPE("iCounter == 0"_sv);
    }
}

void    GpTaskFiberBarrier::Wait (GpTaskFiber::SP aTask)
{
    //Subscribe
    {
        std::scoped_lock lock(iWakeupLock);

        if (iCounter == 0_cnt)
        {
            return;
        }

        iWakeupOnAllReleasedTasks.emplace_back(std::move(aTask));
    }

    //Wait
    while (true)
    {
        //Check counter
        {
            std::scoped_lock lock(iWakeupLock);

            if (iCounter == 0_cnt)
            {
                return;
            }
        }

        //Wait
        GpTaskFiber::SYield(GpTask::ResT::WAITING);
    }
}

void    GpTaskFiberBarrier::WakeupAll (void)
{
    for (GpTaskFiber::SP& t: iWakeupOnAllReleasedTasks)
    {
        t.Vn().MoveToReady();
    }

    iWakeupOnAllReleasedTasks.clear();
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
