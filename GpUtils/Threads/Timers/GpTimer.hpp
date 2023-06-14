#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_TIMERS)

#include "../../EventBus/GpEventPublisher.hpp"
#include "../../Types/Units/SI/GpUnitsSI_Time.hpp"
#include "GpTimerShotEventFactory.hpp"

namespace GPlatform {

class GP_UTILS_API GpTimer final: protected GpEventPublisher
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTimer)
    CLASS_DD(GpTimer)

    enum class ShotRes
    {
        KEEP_FOR_NEXT,
        REMOVE
    };

    using TestRes = std::tuple<ShotRes, bool>;

public:
                                GpTimer             (GpTimerShotEventFactory::SP    aFactory,
                                                     const milliseconds_t           aPeriod) noexcept;
                                GpTimer             (GpTimerShotEventFactory::SP    aFactory,
                                                     const milliseconds_t           aPeriod,
                                                     const milliseconds_t           aDelayBeforeFirstShot) noexcept;
                                GpTimer             (GpTimerShotEventFactory::SP    aFactory,
                                                     const milliseconds_t           aPeriod,
                                                     const u_int_64                 aShotsMaxCount) noexcept;
                                GpTimer             (GpTimerShotEventFactory::SP    aFactory,
                                                     const milliseconds_t           aPeriod,
                                                     const milliseconds_t           aDelayBeforeFirstShot,
                                                     const u_int_64                 aShotsMaxCount) noexcept;
    virtual                     ~GpTimer            (void) noexcept override final;

    static void                 SSingleShot         (GpEventSubscriber::SP  aSubscriber,
                                                     const milliseconds_t   aDelayBeforeShot);

    using GpEventPublisher::Subscribe;
    using GpEventPublisher::Unsubscribe;

    void                        Start               (void);
    void                        Stop                (void);
    ShotRes                     TryMakeShot         (void);

    bool                        IsShotsLimited      (void) const noexcept {return iIsShotsLimited;}
    u_int_64                    ShotsMaxCount       (void) const noexcept {return iShotsMaxCount;}
    u_int_64                    ShotsCount          (void) const noexcept;
    milliseconds_t              Period              (void) const noexcept {return iPeriod;}
    milliseconds_t              DelayBeforeFirstShot(void) const noexcept {return iDelayBeforeFirstShot;}
    milliseconds_t              LastShotTS          (void) const noexcept;
    milliseconds_t              NextShotTS          (void) const noexcept;
    bool                        IsStarted           (void) const noexcept;

private:
    TestRes                     IsReadyToShot       (void) const noexcept;
    void                        MakeShot            (void);

private:
    mutable GpRWLock            iLock;
    GpTimerShotEventFactory::SP iFactory;
    const bool                  iIsShotsLimited;
    const u_int_64              iShotsMaxCount  = 0;
    u_int_64                    iShotsCount     = 0;
    const milliseconds_t        iPeriod;
    const milliseconds_t        iDelayBeforeFirstShot;
    milliseconds_t              iLastShotTS;
    milliseconds_t              iNextShotTS;
    bool                        iIsStarted = false;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
