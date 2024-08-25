#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_TIMERS)

#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSharedMutex.hpp>

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
                            GpTimer                 (void) noexcept;
                            GpTimer                 (CallbackFnT&&  aCallbackFn,
                                                     milliseconds_t aPeriod,
                                                     milliseconds_t aDelayBeforeFirstShot   = 0.0_si_s,
                                                     u_int_64       aShotsMaxCount          = 0,
                                                     bool           aIsReturnToPool         = false) noexcept;
                            ~GpTimer                (void) noexcept;

    void                    Start                   (void);
    bool                    Stop                    (void);
    void                    Reload                  (CallbackFnT&&  aCallbackFn,
                                                     milliseconds_t aPeriod,
                                                     milliseconds_t aDelayBeforeFirstShot   = 0.0_si_s,
                                                     u_int_64       aShotsMaxCount          = 0,
                                                     bool           aIsReturnToPool         = false);

    ShotRes                 TryMakeShot             (void) noexcept;    

    inline bool             IsShotsLimited          (void) const noexcept;
    inline u_int_64         ShotsMaxCount           (void) const noexcept;
    inline u_int_64         ShotsCount              (void) const noexcept;
    inline milliseconds_t   Period                  (void) const noexcept;
    inline milliseconds_t   DelayBeforeFirstShot    (void) const noexcept;
    inline milliseconds_t   LastShotTS              (void) const noexcept;
    inline milliseconds_t   NextShotTS              (void) const noexcept;
    inline bool             IsStarted               (void) const noexcept;
    inline bool             IsReturnToPool          (void) const noexcept;

private:
    TestRes                 _IsReadyToShot          (void) const noexcept REQUIRES(iSpinLockRW);

    inline bool             _IsShotsLimited         (void) const noexcept REQUIRES_SHARED(iSpinLockRW);
    inline u_int_64         _ShotsMaxCount          (void) const noexcept REQUIRES_SHARED(iSpinLockRW);
    inline u_int_64         _ShotsCount             (void) const noexcept REQUIRES_SHARED(iSpinLockRW);
    inline milliseconds_t   _Period                 (void) const noexcept REQUIRES_SHARED(iSpinLockRW);
    inline milliseconds_t   _DelayBeforeFirstShot   (void) const noexcept REQUIRES_SHARED(iSpinLockRW);
    inline milliseconds_t   _LastShotTS             (void) const noexcept REQUIRES_SHARED(iSpinLockRW);
    inline milliseconds_t   _NextShotTS             (void) const noexcept REQUIRES_SHARED(iSpinLockRW);
    inline bool             _IsStarted              (void) const noexcept REQUIRES_SHARED(iSpinLockRW);
    inline bool             _IsReturnToPool         (void) const noexcept REQUIRES_SHARED(iSpinLockRW);

private:
    mutable GpSpinLockRW    iSpinLockRW;

    CallbackFnT             iCallbackFn             GUARDED_BY(iSpinLockRW);
    milliseconds_t          iPeriod                 GUARDED_BY(iSpinLockRW);
    milliseconds_t          iDelayBeforeFirstShot   GUARDED_BY(iSpinLockRW);
    u_int_64                iShotsMaxCount          GUARDED_BY(iSpinLockRW) = 0;
    u_int_64                iShotsCount             GUARDED_BY(iSpinLockRW) = 0;
    milliseconds_t          iLastShotTS             GUARDED_BY(iSpinLockRW);
    milliseconds_t          iNextShotTS             GUARDED_BY(iSpinLockRW);
    bool                    iIsStarted              GUARDED_BY(iSpinLockRW) = false;
    bool                    iIsReturnToPool                                 = false;
};

bool    GpTimer::IsShotsLimited (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return _IsShotsLimited();
}

u_int_64    GpTimer::ShotsMaxCount (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return _ShotsMaxCount();
}

u_int_64    GpTimer::ShotsCount (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return _ShotsCount();
}

milliseconds_t  GpTimer::Period (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return _Period();
}

milliseconds_t  GpTimer::DelayBeforeFirstShot (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return _DelayBeforeFirstShot();
}

milliseconds_t  GpTimer::LastShotTS (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return _LastShotTS();
}

milliseconds_t  GpTimer::NextShotTS (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return _NextShotTS();
}

bool    GpTimer::IsStarted (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return _IsStarted();
}

bool    GpTimer::IsReturnToPool (void) const noexcept
{
    GpSharedLock<GpSpinLockRW> sharedLock{iSpinLockRW};

    return _IsReturnToPool();
}

bool    GpTimer::_IsShotsLimited (void) const noexcept
{
    return iShotsMaxCount > 0;
}

u_int_64    GpTimer::_ShotsMaxCount (void) const noexcept
{
    return iShotsMaxCount;
}

u_int_64    GpTimer::_ShotsCount (void) const noexcept
{
    return iShotsCount;
}

milliseconds_t  GpTimer::_Period (void) const noexcept
{
    return iPeriod;
}

milliseconds_t  GpTimer::_DelayBeforeFirstShot (void) const noexcept
{
    return iDelayBeforeFirstShot;
}

milliseconds_t  GpTimer::_LastShotTS (void) const noexcept
{
    return iLastShotTS;
}

milliseconds_t  GpTimer::_NextShotTS (void) const noexcept
{
    return iNextShotTS;
}

bool    GpTimer::_IsStarted (void) const noexcept
{
    return iIsStarted;
}

bool    GpTimer::_IsReturnToPool (void) const noexcept
{
    return iIsReturnToPool;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_TIMERS)
