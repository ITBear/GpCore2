#include "GpEventPublisher.hpp"

#if defined (GP_USE_EVENT_BUS)

namespace GPlatform {

GpEventPublisher::GpEventPublisher (void) noexcept
{
}

GpEventPublisher::~GpEventPublisher (void) noexcept
{
}

void    GpEventPublisher::PushEvent (GpEvent::SP aEvent)
{
    iSubscribers.Apply([&](GpEventSubscriber::SP& aSubscriber)
    {
        aSubscriber->PushEvent(aEvent);
    });
}

void    GpEventPublisher::Subscribe (GpEventSubscriber::SP aSubscriber)
{
    iSubscribers.Register(aSubscriber.PC(), std::move(aSubscriber));
}

void    GpEventPublisher::Unsubscribe (GpEventSubscriber::SP aSubscriber)
{
    iSubscribers.Unregister(aSubscriber.PC());
}

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
