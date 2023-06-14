#pragma once

#include "../../Config/GpConfig.hpp"

#if defined (GP_USE_EVENT_BUS)

#include "GpEventSubscriber.hpp"
#include "../Types/Containers/GpDictionary.hpp"

namespace GPlatform {

class GpEventPublisher
{
public:
    CLASS_DD(GpEventPublisher)
    CLASS_REMOVE_CTRS_MOVE_COPY(GpEventPublisher)

    TAG_SET(THREAD_SAFE)

    using SubscribersT = GpDictionary<const void*, GpEventSubscriber::SP>;

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
    iSubscribers.Set(aSubscriber.P(), std::move(aSubscriber));
}

void    GpEventPublisher::Unsubscribe (GpEventSubscriber::SP aSubscriber)
{
    iSubscribers.Erase(aSubscriber.P());
}

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
