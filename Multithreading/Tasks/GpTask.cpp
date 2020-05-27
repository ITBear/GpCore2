#include "GpTask.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskScheduler.hpp"
#include "GpTaskSchedulerAccessor.hpp"

namespace GPlatform {

void	GpTask::MoveToReady (void)
{
	GpTaskSchedulerAccessor::SMoveToReady(iScheduler.value().get(), iThisWeakPtr);
}

GpEventSubscriber::PushEvevtRes	GpTask::OnPushEvent (GpEvent::SP& /*aEvent*/) noexcept
{
	if (iScheduler.has_value())
	{
		MoveToReady();
		return PushEvevtRes::ACCEPT;
	} else
	{
		return PushEvevtRes::REJECT;
	}
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)