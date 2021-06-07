#include "GpTimerShotEventFactory.hpp"

#if defined(GP_USE_TIMERS)

namespace GPlatform {

GpTimerShotEvent::SP    GpTimerShotEventFactory::NewInstance (void) const
{
    return MakeSP<GpTimerShotEvent>();
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
