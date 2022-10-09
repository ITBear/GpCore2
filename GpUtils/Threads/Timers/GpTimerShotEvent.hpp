#pragma once

#include "../../GpMacro.hpp"

#if defined(GP_USE_TIMERS)

#include "../../EventBus/GpEvent.hpp"
#include "../../Types/Units/Other/unix_ts_t.hpp"
#include "../../DateTime/GpDateTimeOps.hpp"

namespace GPlatform {

class GP_UTILS_API GpTimerShotEvent final: public GpEvent
{
public:
    CLASS_DD(GpTimerShotEvent)

public:
    inline                  GpTimerShotEvent    (void) noexcept;
    inline                  GpTimerShotEvent    (const GpTimerShotEvent& aEvent) noexcept;
    inline                  GpTimerShotEvent    (GpTimerShotEvent&& aEvent) noexcept;
    virtual                 ~GpTimerShotEvent   (void) noexcept override final;

    static const GpUUID&    SType               (void) noexcept {return sTypeId;}
    virtual const GpUUID&   Type                (void) const noexcept override final;

    unix_ts_ms_t            ShotTS              (void) const noexcept {return shot_ts;}

private:
    unix_ts_ms_t            shot_ts;

    static const GpUUID     sTypeId;
};

GpTimerShotEvent::GpTimerShotEvent (void) noexcept:
shot_ts(GpDateTimeOps::SUnixTS_ms())
{
}

GpTimerShotEvent::GpTimerShotEvent (const GpTimerShotEvent& aEvent) noexcept:
GpEvent(aEvent),
shot_ts(aEvent.shot_ts)
{
}

GpTimerShotEvent::GpTimerShotEvent (GpTimerShotEvent&& aEvent) noexcept:
GpEvent(std::move(aEvent)),
shot_ts(std::move(aEvent.shot_ts))
{
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
