#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TIMERS)

#include "GpTimerShotEvent.hpp"

namespace GPlatform {

class GPCORE_API GpTimerShotEventFactory
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTimerShotEventFactory)
    CLASS_DECLARE_DEFAULTS(GpTimerShotEventFactory)

public:
                                    GpTimerShotEventFactory     (void) noexcept = default;
    virtual                         ~GpTimerShotEventFactory    (void) noexcept = default;

    virtual GpTimerShotEvent::SP    NewInstance                 (void) const;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
