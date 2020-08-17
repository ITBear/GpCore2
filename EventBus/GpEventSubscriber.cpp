#include "GpEventSubscriber.hpp"
#include <mutex>

#if defined (GP_USE_EVENT_BUS)

#include "GpEventBus.hpp"

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

    if (OnPushEvent(aEvent) == PushEvevtRes::ACCEPT)
    {
        iEvents.push(std::move(aEvent));
    }
}

GpEvent::SP GpEventSubscriber::PopNextEvent (void) noexcept
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

void    GpEventSubscriber::ClearEventsQueue (void) noexcept
{
    std::scoped_lock lock(iEventsLock);

    while (!iEvents.empty())
    {
        iEvents.pop();
    }
}

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
