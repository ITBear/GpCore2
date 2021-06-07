#include "GpEventSubscriber.hpp"

#if defined (GP_USE_EVENT_BUS)

namespace GPlatform {

GpEventSubscriber::GpEventSubscriber (void) noexcept
{
}

GpEventSubscriber::~GpEventSubscriber (void) noexcept
{
    std::scoped_lock lock(iEventsLock);

    while (!iEvents.empty())
    {
        iEvents.pop();
    }
}

void    GpEventSubscriber::PushEvent (GpEvent::SP aEvent)
{
    std::scoped_lock lock(iEventsLock);

    OnPushEvent();
    iEvents.push(std::move(aEvent));
}

GpEvent::SP GpEventSubscriber::PopNextEvent (void)
{
    std::scoped_lock lock(iEventsLock);

    if (iEvents.empty())
    {
        return GpEvent::SP::SNull();
    }

    GpEvent::SP event = iEvents.front();
    iEvents.pop();

    return event;
}

void    GpEventSubscriber::ClearEventsQueue (void)
{
    std::scoped_lock lock(iEventsLock);

    while (!iEvents.empty())
    {
        iEvents.pop();
    }
}

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
