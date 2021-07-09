#include "GpTaskFiberBarrier.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include <mutex>
#include <iostream>

namespace GPlatform {

static int _GpTaskFiberBarrier_counter = 0;

GpTaskFiberBarrier::GpTaskFiberBarrier (count_t aCounter) noexcept:
iCounter(aCounter)
{
    _GpTaskFiberBarrier_counter++;
    std::cout << "[GpTaskFiberBarrier::GpTaskFiberBarrier]: counter = " << _GpTaskFiberBarrier_counter << std::endl;
}

GpTaskFiberBarrier::~GpTaskFiberBarrier (void) noexcept
{
    _GpTaskFiberBarrier_counter--;
    std::cout << "[GpTaskFiberBarrier::~GpTaskFiberBarrier]: counter = " << _GpTaskFiberBarrier_counter << std::endl;
}

void    GpTaskFiberBarrier::Release (std::optional<std::any>&& aResult) noexcept
{
    try
    {
        std::scoped_lock lock(iWakeupLock);

        iResult.emplace_back(std::move(aResult));

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
    } catch (const std::exception& e)
    {
        GpExceptionsSink::SSink(e);
    } catch (...)
    {
        GpExceptionsSink::SSinkUnknown();
    }
}

const GpTaskFiberBarrier::ResultT&  GpTaskFiberBarrier::Wait (void)
{
    //Get current task
    GpTaskFiber::WP currentTask = GpTaskFiber::SCurrentTask();

    THROW_GPE_COND
    (
        currentTask.IsNotNULL(),
        "Call outside fiber task"_sv
    );

    //Subscribe
    {
        std::scoped_lock lock(iWakeupLock);

        if (iCounter == 0_cnt)
        {
            return iResult;
        }

        iWakeupOnAllReleasedTasks.emplace_back(std::move(currentTask));
    }

    //Wait
    while (true)
    {
        //Check counter
        {
            std::scoped_lock lock(iWakeupLock);

            if (iCounter == 0_cnt)
            {
                return iResult;
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
