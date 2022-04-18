#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TIMERS)

#include <functional>
#include "../../Types/Units/SI/GpUnitsSI_Time.hpp"
#include "../../Types/DateTime/GpDateTimeOps.hpp"

namespace GPlatform {

class GpDoOnceInPeriod
{
public:
                            GpDoOnceInPeriod    (void) noexcept = delete;
                            GpDoOnceInPeriod    (const milliseconds_t aPeriod) noexcept: iPeriod(aPeriod){}
                            ~GpDoOnceInPeriod   (void) noexcept {}

    inline bool             Do                  (std::function<void()> aFn);

private:
    const milliseconds_t    iPeriod;
    milliseconds_t          iLastDoSTS = 0.0_si_ms;
};

bool    GpDoOnceInPeriod::Do (std::function<void()> aFn)
{
    const milliseconds_t nowSTS = GpDateTimeOps::SSteadyTS_ms();

    if ((nowSTS - iLastDoSTS) >= iPeriod)
    {
        aFn();
        iLastDoSTS = nowSTS;
        return true;
    } else
    {
        return false;
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
