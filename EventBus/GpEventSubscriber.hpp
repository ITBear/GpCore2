#pragma once

#include "../Config/GpConfig.hpp"

#if defined (GP_USE_EVENT_BUS)

#include <mutex>
#include "GpEvent.hpp"
#include "../Multithreading/SyncPrimitives/GpSpinlock.hpp"

namespace GPlatform {

class GPCORE_API GpEventSubscriber
{
public:
    CLASS_DECLARE_DEFAULTS(GpEventSubscriber)
    CLASS_REMOVE_CTRS_MOVE_COPY(GpEventSubscriber)

protected:
                            GpEventSubscriber               (void) noexcept;

public:
    virtual                 ~GpEventSubscriber              (void) noexcept;

    void                    PushEvent                       (GpEvent::SP aEvent);
    GpEvent::SP             PopNextEvent                    (void);
    bool                    HasEvents                       (void) const noexcept {std::scoped_lock lock(iEventsLock); return !iEvents.empty();}

    GpSpinlock&             EventsLock                      (void) const noexcept {return iEventsLock;}
    bool                    HasEventsNoLock                 (void) const noexcept {return !iEvents.empty();}

protected:  
    void                    ClearEventsQueue                (void);
    virtual void            OnPushEvent                     (void) = 0;

private:
    mutable GpSpinlock      iEventsLock;
    GpEvent::C::Queue::SP   iEvents;
};

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
