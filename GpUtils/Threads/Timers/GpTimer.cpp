#include <GpCore2/GpUtils/Threads/Timers/GpTimer.hpp>

#if defined(GP_USE_TIMERS)

#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GpTimer::GpTimer (void) noexcept
{
}

GpTimer::GpTimer
(
    CallbackFnT&&           aCallbackFn,
    const milliseconds_t    aPeriod,
    const milliseconds_t    aDelayBeforeFirstShot,
    const u_int_64          aShotsMaxCount,
    const bool              aIsReturnToPool
) noexcept:
iCallbackFn          {std::move(aCallbackFn)},
iPeriod              {aPeriod},
iDelayBeforeFirstShot{aDelayBeforeFirstShot},
iShotsMaxCount       {aShotsMaxCount},
iIsReturnToPool      {aIsReturnToPool}
{
}

GpTimer::~GpTimer (void) noexcept
{
}

void    GpTimer::Start (void)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    if (iIsStarted)
    {
        return;
    }

    const milliseconds_t nowTS = GpDateTimeOps::SSteadyTS_ms();

    iLastShotTS = nowTS;
    iNextShotTS = nowTS + iDelayBeforeFirstShot;
    iIsStarted  = true;
    iShotsCount = 0;
}

bool    GpTimer::Stop (void)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    if (iIsStarted)
    {
        iIsStarted = false;
        return true;
    }

    return false;
}

void    GpTimer::Reload
(
    CallbackFnT&&           aCallbackFn,
    const milliseconds_t    aPeriod,
    const milliseconds_t    aDelayBeforeFirstShot,
    const u_int_64          aShotsMaxCount,
    const bool              aIsReturnToPool
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    iIsStarted = false;

    iCallbackFn             = std::move(aCallbackFn);
    iPeriod                 = aPeriod;
    iDelayBeforeFirstShot   = aDelayBeforeFirstShot;
    iShotsMaxCount          = aShotsMaxCount;
    iIsReturnToPool         = aIsReturnToPool;
}

GpTimer::ShotRes    GpTimer::TryMakeShot (void) noexcept
{
    try
    {
        GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

        const TestRes testRes = _IsReadyToShot();

        if (std::get<1>(testRes) == true)
        {
            iShotsCount++;
            iLastShotTS = iNextShotTS;
            iNextShotTS = iNextShotTS + iPeriod;

            if (iCallbackFn)
            {
                iCallbackFn(*this);
            }
        }

        return std::get<0>(testRes);
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr("[GpTimersManager::TryMakeShot]: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr("[GpTimersManager::TryMakeShot]: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr("[GpTimersManager::TryMakeShot]: unknown"_sv);
    }

    return GpTimer::ShotRes::REMOVE;
}

GpTimer::TestRes    GpTimer::_IsReadyToShot (void) const noexcept
{
    // Check shots limit
    if (_IsShotsLimited())
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

}// namespace GPlatform

#endif// #if defined(GP_USE_TIMERS)
