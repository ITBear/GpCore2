#include "GpTaskScheduler.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskSchedulerFactory.hpp"

namespace GPlatform {

GpTaskScheduler::SP GpTaskScheduler::sInstance;

void    GpTaskScheduler::SStart
(
    const GpTaskSchedulerFactory&   aFactory,
    const size_t                    aExecutorsCount,
    const size_t                    aTasksMaxCount
)
{
    sInstance = aFactory.NewInstance();

    S().Start
    (
        aExecutorsCount,
        aTasksMaxCount
    );
}

void    GpTaskScheduler::SStopAndClear (void)
{
    if (GpTaskScheduler::sInstance.IsNotNULL())
    {
        S().RequestStopAndJoin();
        sInstance.Clear();
    }
}

void    GpTaskScheduler::Start
(
    const size_t aExecutorsCount,
    const size_t aTasksMaxCount
)
{
    iExecutorsCount = aExecutorsCount;
    iTasksMaxCount  = aTasksMaxCount;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
