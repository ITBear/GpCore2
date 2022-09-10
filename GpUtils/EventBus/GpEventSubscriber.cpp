#include "GpEventSubscriber.hpp"

#if defined (GP_USE_EVENT_BUS)

namespace GPlatform {

GpEventSubscriber::~GpEventSubscriber (void) noexcept
{
    std::scoped_lock lock(iEventsLock);

    iEvents.clear();
}

void    GpEventSubscriber::PushEvent (GpEvent::SP aEvent)
{
    std::scoped_lock lock(iEventsLock);

    iEvents.push_back(std::move(aEvent));
    OnPushEvent();
}

GpEvent::SP GpEventSubscriber::PopNextEvent (void)
{
    std::scoped_lock lock(iEventsLock);

    if (iEvents.empty())
    {
        return GpEvent::SP::SNull();
    }

    GpEvent::SP event = iEvents.front();
    iEvents.pop_front();

    return event;
}

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
