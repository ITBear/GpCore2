#include "GpTimer.hpp"

#if defined(GP_USE_TIMERS)

#include "../../DateTime/GpDateTimeOps.hpp"
#include "../../Types/Strings/GpStringUtils.hpp"
#include "../../Types/Strings/GpStringOps.hpp"

namespace GPlatform {

GpTimer::~GpTimer (void) noexcept
{
}

void    GpTimer::Start (void)
{
    std::scoped_lock lock(iLock);

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

GpTimer::ShotRes    GpTimer::TryMakeShot (void) noexcept
{
    try
    {
        std::shared_lock lock(iLock);

        const TestRes testRes = IsReadyToShot();

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

GpTimer::TestRes    GpTimer::IsReadyToShot (void) const noexcept
{
    // Check shots limit
    if (IsShotsLimited())
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
