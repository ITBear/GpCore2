#pragma once

#include "../GpMacro.hpp"

#if defined (GP_USE_EVENT_BUS)

#include "GpEventSubscriber.hpp"
#include "../Types/Containers/GpElementsCatalog.hpp"

namespace GPlatform {

class GpEventPublisher
{
public:
    CLASS_DD(GpEventPublisher)
    CLASS_REMOVE_CTRS_MOVE_COPY(GpEventPublisher)

    CLASS_TAG(THREAD_SAFE)

    using SubscribersT = GpElementsCatalog<const void*, GpEventSubscriber::SP>;

public:
                        GpEventPublisher    (void) noexcept = default;
    virtual             ~GpEventPublisher   (void) noexcept = default;

    inline void         PushEvent           (GpEvent::SP aEvent);
    inline void         Subscribe           (GpEventSubscriber::SP aSubscriber);
    inline void         Unsubscribe         (GpEventSubscriber::SP aSubscriber);

private:
    SubscribersT        iSubscribers;
};

void    GpEventPublisher::PushEvent (GpEvent::SP aEvent)
{
    iSubscribers.Apply([&](GpEventSubscriber::SP& aSubscriber)
    {
        aSubscriber.V().PushEvent(aEvent);
    });
}

void    GpEventPublisher::Subscribe (GpEventSubscriber::SP aSubscriber)
{
    iSubscribers.Register(aSubscriber.P(), std::move(aSubscriber));
}

void    GpEventPublisher::Unsubscribe (GpEventSubscriber::SP aSubscriber)
{
    iSubscribers.Unregister(aSubscriber.P());
}

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
