#pragma once

#include "../Config/GpConfig.hpp"

#if defined (GP_USE_EVENT_BUS)

#include "GpEventSubscriber.hpp"
#include "../Types/Containers/GpElementsCatalog.hpp"

namespace GPlatform {

class GPCORE_API GpEventPublisher
{
public:
    CLASS_DECLARE_DEFAULTS(GpEventPublisher)
    CLASS_REMOVE_CTRS_MOVE_COPY(GpEventPublisher)

    CLASS_TAG(THREAD_SAFE)

    using SubscribersT = GpElementsCatalog<const void*, GpEventSubscriber::SP>;

public:
                        GpEventPublisher    (void) noexcept;
    virtual             ~GpEventPublisher   (void) noexcept;

    void                PushEvent           (GpEvent::SP aEvent);
    void                Subscribe           (GpEventSubscriber::SP aSubscriber);
    void                Unsubscribe         (GpEventSubscriber::SP aSubscriber);

private:
    SubscribersT        iSubscribers;
};

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
