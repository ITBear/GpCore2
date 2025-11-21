#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_TIMERS)

#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpSharedMap.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpSharedPool.hpp>
#include <GpCore2/GpUtils/Threads/GpThread.hpp>
#include <GpCore2/GpUtils/Threads/Timers/GpTimer.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLock.hpp>

namespace GPlatform {

class GP_UTILS_API GpTimersManager final: public GpRunnable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTimersManager)
    CLASS_DD(GpTimersManager)
    TAG_SET(THREAD_SAFE)

    using ActiveTimersT = boost::container::small_flat_map<const void*, GpTimer::SP, 64>;
    using TimersToAddT  = boost::container::small_vector<GpTimer::SP, 64>;

    class TimersPoolT final: public GpSharedPool<GpTimer::SP>
    {
    public:
                            TimersPoolT     (void) noexcept = default;
                            ~TimersPoolT    (void) noexcept = default;

    protected:
        virtual value_type  NewElement      (void) override final
        {
            return MakeSP<GpTimer>();
        }
    };

public:
                            GpTimersManager     (void) noexcept;
    virtual                 ~GpTimersManager    (void) noexcept override final;

    static GpTimersManager& S                   (void) {return *sTimersManager;}
    static void             SStart              (void);
    static void             SDisableShots       (void);
    static void             SStop               (void);
    static GpTimer::SP      SSingleShot         (GpTimer::CallbackFnT&& aCallbackFn,
                                                 milliseconds_t         aDelayBeforeShot);
    inline static void      SAddTimer           (GpTimer::SP aTimer);
    void                    AddTimer            (GpTimer::SP aTimer);

    virtual void            Run                 (GpConditionVarFlag& aStopFlag) noexcept override final;

private:
    void                    DisableShots        (void);

private:
    const milliseconds_t        iCheckPeriod = 16.337_si_ms; // TODO: move to config

    mutable GpSpinLock<>        iTimersToAddSpinLock;
    TimersToAddT                iTimersToAdd GUARDED_BY(iTimersToAddSpinLock);

    ActiveTimersT               iActiveTimers;
    TimersPoolT                 iTimersPool;

    std::atomic_flag            iIsShotsEnabled;

    static GpTimersManager*     sTimersManager;
    static GpConditionVarFlag   sTimersThreadStopFlag;
    static GpThread             sTimersThread;
};

void    GpTimersManager::SAddTimer (GpTimer::SP aTimer)
{
    S().AddTimer(std::move(aTimer));
}

}// namespace GPlatform

#endif// #if defined(GP_USE_TIMERS)
