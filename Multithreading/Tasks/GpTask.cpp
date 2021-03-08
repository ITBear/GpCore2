#include "GpTask.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskScheduler.hpp"
#include "GpTaskSchedulerAccessor.hpp"

namespace GPlatform {

void    GpTask::MoveToReady (void)
{
    THROW_GPE_COND
    (
        iScheduler.has_value(),
        "Add task to scheduler"_sv
    );

    GpTaskSchedulerAccessor::SMoveToReady(iScheduler.value().get(), iThisWeakPtr);
}

void    GpTask::OnPushEvent (void)
{
    MoveToReady();
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
