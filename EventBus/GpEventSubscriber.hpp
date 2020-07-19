#pragma once

#include "../Config/GpConfig.hpp"

#if defined (GP_USE_EVENT_BUS)

#include "GpEvent.hpp"
#include "../Multithreading/SyncPrimitives/GpSpinlock.hpp"

namespace GPlatform {

class GPCORE_API GpEventSubscriber
{
public:
    CLASS_DECLARE_DEFAULTS(GpEventSubscriber)
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpEventSubscriber)

    enum class PushEvevtRes
    {
        REJECT,
        ACCEPT
    };

protected:
                            GpEventSubscriber   (void) noexcept;

public:
    virtual                 ~GpEventSubscriber  (void) noexcept;

    void                    PushEvent           (GpEvent::SP aEvent);

protected:
    GpEvent::SP             PopNextEvent        (void) noexcept;
    virtual PushEvevtRes    OnPushEvent         (GpEvent::SP& aEvent) noexcept = 0;

private:
    GpSpinlock              iEventsLock;
    GpEvent::C::Queue::SP   iEvents;
};

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
