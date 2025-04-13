#include <GpCore2/GpTasks/ITC/GpItcFutureUtils.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>

namespace GPlatform {

void    GpItcFutureUtils::SYield (const milliseconds_t aTimeout)
{
    std::ignore = YIELD_WAIT(aTimeout);
}

void    GpItcFutureUtils::SYield (void)
{
    YIELD_WAIT();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
