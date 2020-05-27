#include "GpEventBus.hpp"

#if defined (GP_USE_EVENT_BUS)

#include <mutex>

namespace GPlatform {

GpEventBus::GpEventBus (void) noexcept
{
}

GpEventBus::~GpEventBus	(void) noexcept
{
	Clear();
}

void	GpEventBus::Clear (void) noexcept
{
	std::scoped_lock lock(iLock);
	iSubscribers.clear();
}

void	GpEventBus::Subscribe (GpEventSubscriber::SP aSubscriber)
{
	std::scoped_lock lock(iLock);
	iSubscribers.emplace(std::move(aSubscriber));
}

void	GpEventBus::Unsubscribe (GpEventSubscriber::SP aSubscriber)
{
	std::scoped_lock lock(iLock);
	iSubscribers.erase(aSubscriber);
}

/*void	GpEventBus::Post (GpEvent::SP aEvent)
{
	std::scoped_lock lock(iLock);
	THROW_NOT_IMPLEMENTED();
}*/

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)