#pragma once

#include "../../GpUtils_global.hpp"

#if defined(GP_USE_TIMERS)

#include "GpTimerShotEvent.hpp"

namespace GPlatform {

class GP_UTILS_API GpTimerShotEventFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTimerShotEventFactory)
    CLASS_DD(GpTimerShotEventFactory)

public:
                                    GpTimerShotEventFactory     (void) noexcept = default;
    virtual                         ~GpTimerShotEventFactory    (void) noexcept = default;

    virtual GpTimerShotEvent::SP    NewInstance                 (void) const;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
