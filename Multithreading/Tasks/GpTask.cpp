#include "GpTask.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskScheduler.hpp"
#include "GpTaskSchedulerAccessor.hpp"

namespace GPlatform {

void    GpTask::MoveToReady (void)
{
    THROW_GPE_COND_CHECK_M(iScheduler.has_value(), "Add task to scheduler"_sv);
    GpTaskSchedulerAccessor::SMoveToReady(iScheduler.value().get(), iThisWeakPtr);
}

void    GpTask::Terminate (void) noexcept
{
    //NOP
}

GpEventSubscriber::PushEvevtRes GpTask::OnPushEvent (GpEvent::SP& /*aEvent*/) noexcept
{
    //if (iScheduler.has_value())
    //{
        MoveToReady();
        return PushEvevtRes::ACCEPT;
    //} else
    //{
    //  return PushEvevtRes::REJECT;
    //}
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
