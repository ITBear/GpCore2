#include "GpTimerShotEvent.hpp"

#if defined(GP_USE_TIMERS)

namespace GPlatform {

const GpUUID    GpTimerShotEvent::sTypeId = u8"9de1b354-9878-434e-1957-1ab45bf55400"_uuid;

GpTimerShotEvent::~GpTimerShotEvent (void) noexcept
{
}

const GpUUID&   GpTimerShotEvent::Type (void) const noexcept
{
    return GpTimerShotEvent::SType();
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
