#pragma once

#include "../GpMacro.hpp"

#if defined (GP_USE_EVENT_BUS)

#include <mutex>
#include "GpEvent.hpp"
#include "../SyncPrimitives/GpSpinlock.hpp"

namespace GPlatform {

class GP_UTILS_API GpEventSubscriber
{
public:
    CLASS_DD(GpEventSubscriber)
    CLASS_REMOVE_CTRS_MOVE_COPY(GpEventSubscriber)

protected:
                            GpEventSubscriber   (void) noexcept = default;

public:
    virtual                 ~GpEventSubscriber  (void) noexcept;

    void                    PushEvent           (GpEvent::SP aEvent);
    GpEvent::SP             PopNextEvent        (void);
    bool                    HasEvents           (void) const noexcept {std::scoped_lock lock(iEventsLock); return !iEvents.empty();}

    GpSpinlock&             EventsLock          (void) const noexcept {return iEventsLock;}
    bool                    HasEventsNoLock     (void) const noexcept {return !iEvents.empty();}

protected:
    virtual void            OnPushEvent         (void) = 0;

private:
    mutable GpSpinlock      iEventsLock;
    GpEvent::C::Deque::SP   iEvents;
};

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
