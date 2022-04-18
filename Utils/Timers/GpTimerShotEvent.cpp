#include "GpTimerShotEvent.hpp"
#include "../../Types/DateTime/GpDateTimeOps.hpp"

#if defined(GP_USE_TIMERS)

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpTimerShotEvent, GP_MODULE_UUID)

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

GpTimerShotEvent::~GpTimerShotEvent (void) noexcept
{
}

void    GpTimerShotEvent::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(shot_ts);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
