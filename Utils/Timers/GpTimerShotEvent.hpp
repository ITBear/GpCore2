#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TIMERS)

#include "../../EventBus/GpEvent.hpp"

namespace GPlatform {

class GPCORE_API GpTimerShotEvent: public GpEvent
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTimerShotEvent)
    CLASS_DECLARE_DEFAULTS(GpTimerShotEvent)
    TYPE_STRUCT_DECLARE("0730c19d-9c0e-4cc8-ad89-747ee22eb7e2"_sv)

public:
                        GpTimerShotEvent        (void) noexcept;
    virtual             ~GpTimerShotEvent       (void) noexcept override;

    unix_ts_ms_t        ShotTS                  (void) const noexcept {return shot_ts;}

private:
    unix_ts_ms_t        shot_ts;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
