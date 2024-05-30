#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_TIMERS)

#include "../../Types/Units/SI/GpUnitsSI_Time.hpp"
#include "../../Types/Containers/GpContainersT.hpp"
#include "../../SyncPrimitives/GpSpinLockRW.hpp"

#include <mutex>
#include <shared_mutex>

namespace GPlatform {

class GP_UTILS_API GpTimer
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTimer)
    CLASS_DD(GpTimer)
    TAG_SET(THREAD_SAFE)

    enum class ShotRes
    {
        KEEP_FOR_NEXT,
        REMOVE
    };

    using TestRes       = std::tuple<ShotRes, bool>;
    using CallbackFnT   = std::function<void(const GpTimer& aTimer)>;

public:
                            GpTimer                 (void) noexcept = default;
    inline                  GpTimer                 (CallbackFnT&&          aCallbackFn,
                                                     const milliseconds_t   aPeriod,
                                                     const milliseconds_t   aDelayBeforeFirstShot   = 0.0_si_s,
                                                     const u_int_64         aShotsMaxCount          = 0,
                                                     const bool             aIsReturnToPool         = false) noexcept;
                            ~GpTimer                (void) noexcept;

    void                    Start                   (void);
    inline void             Stop                    (void);
    inline void             Reload                  (CallbackFnT&&          aCallbackFn,
                                                     const milliseconds_t   aPeriod,
                                                     const milliseconds_t   aDelayBeforeFirstShot   = 0.0_si_s,
                                                     const u_int_64         aShotsMaxCount          = 0,
                                                     const bool             aIsReturnToPool         = false);

    ShotRes                 TryMakeShot             (void) noexcept;    

    bool                    IsShotsLimited          (void) const noexcept {return iShotsMaxCount > 0;}
    u_int_64                ShotsMaxCount           (void) const noexcept {return iShotsMaxCount;}
    inline u_int_64         ShotsCount              (void) const noexcept;
    milliseconds_t          Period                  (void) const noexcept {return iPeriod;}
    milliseconds_t          DelayBeforeFirstShot    (void) const noexcept {return iDelayBeforeFirstShot;}
    inline milliseconds_t   LastShotTS              (void) const noexcept;
    inline milliseconds_t   NextShotTS              (void) const noexcept;
    inline bool             IsStarted               (void) const noexcept;
    inline bool             IsReturnToPool          (void) const noexcept;

private:
    TestRes                 IsReadyToShot           (void) const noexcept;

private:
    mutable GpSpinLockRW    iLock;
    CallbackFnT             iCallbackFn;
    milliseconds_t          iPeriod;
    milliseconds_t          iDelayBeforeFirstShot;
    u_int_64                iShotsMaxCount          = 0;
    u_int_64                iShotsCount             = 0;
    milliseconds_t          iLastShotTS;
    milliseconds_t          iNextShotTS;
    bool                    iIsStarted              = false;
    bool                    iIsReturnToPool         = false;
};

GpTimer::GpTimer
(
    CallbackFnT&&           aCallbackFn,
    const milliseconds_t    aPeriod,
    const milliseconds_t    aDelayBeforeFirstShot,
    const u_int_64          aShotsMaxCount,
    const bool              aIsReturnToPool
) noexcept:
iCallbackFn          (std::move(aCallbackFn)),
iPeriod              (aPeriod),
iDelayBeforeFirstShot(aDelayBeforeFirstShot),
iShotsMaxCount       (aShotsMaxCount),
iIsReturnToPool      (aIsReturnToPool)
{
}

void    GpTimer::Stop (void)
{
    std::scoped_lock lock(iLock);

    if (iIsStarted)
    {
        iIsStarted = false;
    }
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
    std::scoped_lock lock(iLock);

    iIsStarted = false;

    iCallbackFn             = std::move(aCallbackFn);
    iPeriod                 = aPeriod;
    iDelayBeforeFirstShot   = aDelayBeforeFirstShot;
    iShotsMaxCount          = aShotsMaxCount;
    iIsReturnToPool         = aIsReturnToPool;
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

bool    GpTimer::IsReturnToPool (void) const noexcept
{
    std::shared_lock lock(iLock);
    return iIsReturnToPool;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_TIMERS)
