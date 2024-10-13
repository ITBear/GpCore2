#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_TIMERS)

#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>

#include <functional>

namespace GPlatform {

class GpDoOnceInPeriod
{
public:
    enum class Mode
    {
        AT_FIRST_CALL,
        AT_TIMEOUT
    };

public:
                            GpDoOnceInPeriod    (void) noexcept = delete;
    inline                  GpDoOnceInPeriod    (const milliseconds_t   aPeriod,
                                                 const Mode             aMode) noexcept;
                            ~GpDoOnceInPeriod   (void) noexcept {}

    inline bool             Do                  (std::function<void()> aFn);
    inline void             ResetCounter        (void) noexcept;

private:
    const milliseconds_t    iPeriod;
    milliseconds_t          iLastDoSTS = 0.0_si_ms;
};

GpDoOnceInPeriod::GpDoOnceInPeriod
(
    const milliseconds_t    aPeriod,
    const Mode              aMode
) noexcept:
iPeriod(aPeriod)
{
    if (aMode == Mode::AT_TIMEOUT)
    {
        iLastDoSTS = GpDateTimeOps::SSteadyTS_ms();
    }
}

bool    GpDoOnceInPeriod::Do (std::function<void()> aFn)
{
    const milliseconds_t nowSTS = GpDateTimeOps::SSteadyTS_ms();

    if ((nowSTS - iLastDoSTS) >= iPeriod) [[unlikely]]
    {
        aFn();
        iLastDoSTS = nowSTS;
        return true;
    } else
    {
        return false;
    }
}

void    GpDoOnceInPeriod::ResetCounter (void) noexcept
{
    iLastDoSTS = GpDateTimeOps::SSteadyTS_ms();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_TIMERS)
