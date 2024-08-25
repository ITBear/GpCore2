#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_TIMERS)

#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpDictionary.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpElementsPool.hpp>
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

    class TimersPoolT final: public GpElementsPool<GpTimer::SP>
    {
    public:
                                TimersPoolT         (void) noexcept = default;
                                ~TimersPoolT        (void) noexcept = default;

    protected:
        virtual value_type      NewElement          (void) override final
        {
            return MakeSP<GpTimer>();
        }
    };

public:
                                GpTimersManager     (void) noexcept;
    virtual                     ~GpTimersManager    (void) noexcept override final;

    static void                 SStart              (void);
    static void                 SDisableShots       (void);
    static void                 SStop               (void); 
    static GpTimersManager::SP  SManager            (void) {return sTimersManager;}
    static GpTimer::SP          SSingleShot         (GpTimer::CallbackFnT&& aCallbackFn,
                                                     milliseconds_t         aDelayBeforeShot,
                                                     bool                   aUseTimersPool);
    inline static void          SAddTimer           (GpTimer::SP aTimer);
    void                        AddTimer            (GpTimer::SP aTimer);

    virtual void                Run                 (std::atomic_flag& aStopRequest) noexcept override final;

protected:
    virtual void                OnNotify            (void) noexcept override final;

private:
    void                        DisableShots        (void);

private:
    const milliseconds_t        iCheckPeriod = 16.337_si_ms; // TODO: move to config

    mutable GpSpinLock          iTimersToAddSpinLock;
    TimersToAddT                iTimersToAdd GUARDED_BY(iTimersToAddSpinLock);

    ActiveTimersT               iActiveTimers;
    TimersPoolT                 iTimersPool;

    std::atomic_flag            iIsShotsEnabled;

    static GpTimersManager::SP  sTimersManager;
    static std::atomic_flag     sTimersThreadDestruct;
    static GpThread             sTimersThread;
};

void    GpTimersManager::SAddTimer (GpTimer::SP aTimer)
{
    SManager()->AddTimer(std::move(aTimer));
}

}// namespace GPlatform

#endif// #if defined(GP_USE_TIMERS)
