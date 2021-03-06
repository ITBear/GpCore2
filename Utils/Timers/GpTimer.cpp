#include "GpTimer.hpp"

#if defined(GP_USE_TIMERS)

#include "../../Types/DateTime/GpDateTimeOps.hpp"

namespace GPlatform {

GpTimer::GpTimer
(
    GpTimerShotEventFactory::SP aFactory,
    const milliseconds_t        aPeriod
) noexcept:
iFactory(std::move(aFactory)),
iIsShotsLimited(false),
iPeriod(aPeriod),
iDelayBeforeFirstShot(0.0_si_ms)
{
}

GpTimer::GpTimer
(
    GpTimerShotEventFactory::SP aFactory,
    const milliseconds_t        aPeriod,
    const milliseconds_t        aDelayBeforeFirstShot
) noexcept:
iFactory(std::move(aFactory)),
iIsShotsLimited(false),
iPeriod(aPeriod),
iDelayBeforeFirstShot(aDelayBeforeFirstShot)
{
}

GpTimer::GpTimer
(
    GpTimerShotEventFactory::SP aFactory,
    const milliseconds_t        aPeriod,
    const u_int_64              aShotsMaxCount
) noexcept:
iFactory(std::move(aFactory)),
iIsShotsLimited(true),
iShotsMaxCount(aShotsMaxCount),
iPeriod(aPeriod),
iDelayBeforeFirstShot(0.0_si_ms)
{
}

GpTimer::GpTimer
(
    GpTimerShotEventFactory::SP aFactory,
    const milliseconds_t        aPeriod,
    const milliseconds_t        aDelayBeforeFirstShot,
    const u_int_64              aShotsMaxCount
) noexcept:
iFactory(std::move(aFactory)),
iIsShotsLimited(true),
iShotsMaxCount(aShotsMaxCount),
iPeriod(aPeriod),
iDelayBeforeFirstShot(aDelayBeforeFirstShot)
{
}

GpTimer::~GpTimer (void) noexcept
{
}

void    GpTimer::Start (void)
{
    std::scoped_lock lock(iLock);

    if (!iIsStarted)
    {
        const milliseconds_t nowTS = GpDateTimeOps::SSteadyTS_ms();

        iLastShotTS = nowTS;
        iNextShotTS = nowTS + iDelayBeforeFirstShot;
        iIsStarted  = true;
        iShotsCount = 0;
    }
}

void    GpTimer::Stop (void)
{
    std::scoped_lock lock(iLock);

    if (iIsStarted)
    {
        iIsStarted = false;
    }
}

GpTimer::ShotRes    GpTimer::TryMakeShot (void)
{
    std::shared_lock lock(iLock);

    const TestRes testRes = IsReadyToShot();

    if (std::get<1>(testRes) == true)
    {
        MakeShot();
    }

    return std::get<0>(testRes);
}

u_int_64    GpTimer::ShotsCount (void) const noexcept
{
    std::shared_lock lock(iLock);
    return iShotsCount;
}

milliseconds_t  GpTimer::LastShotTS (void) const noexcept
{
    std::shared_lock lock(iLock);
    return iLastShotTS;
}

milliseconds_t  GpTimer::NextShotTS (void) const noexcept
{
    std::shared_lock lock(iLock);
    return iNextShotTS;
}

bool    GpTimer::IsStarted (void) const noexcept
{
    std::shared_lock lock(iLock);
    return iIsStarted;
}

GpTimer::TestRes    GpTimer::IsReadyToShot (void) const noexcept
{
    // Check shots limit
    if (iIsShotsLimited)
    {
        if (iShotsCount >= iShotsMaxCount)
        {
            return {ShotRes::REMOVE, false};
        }
    }

    // Check if started
    if (!iIsStarted)
    {
        return {ShotRes::KEEP_FOR_NEXT, false};
    }

    // Check iNextShotTS
    if (iNextShotTS > GpDateTimeOps::SSteadyTS_ms())
    {
        return {ShotRes::KEEP_FOR_NEXT, false};
    }

    return {ShotRes::KEEP_FOR_NEXT, true};
}

void    GpTimer::MakeShot (void)
{
    iShotsCount++;
    iLastShotTS = iNextShotTS;
    iNextShotTS = iNextShotTS + iPeriod;

    PushEvent(iFactory->NewInstance());
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
