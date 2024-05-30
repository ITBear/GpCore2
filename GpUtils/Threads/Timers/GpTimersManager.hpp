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

namespace GPlatform {

class GP_UTILS_API GpTimersManager final: public GpRunnable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTimersManager)
    CLASS_DD(GpTimersManager)
    TAG_SET(THREAD_SAFE)

    using TimersT = GpDictionary<boost::container::small_flat_map<const void*, GpTimer::SP, 64>>;

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
                                GpTimersManager     (void) noexcept = default;
                                ~GpTimersManager    (void) noexcept final = default;

    static void                 SStart              (void);
    static void                 SStop               (void);
    static GpTimersManager::SP  SManager            (void) {return sTimersManager;}
    [[nodiscard]] static bool   SSingleShot         (GpTimer::CallbackFnT&& aCallbackFn,
                                                     const milliseconds_t   aDelayBeforeShot);

    void                        AddTimer            (GpTimer::SP aTimer);

    virtual void                Run                 (std::atomic_flag& aStopRequest) noexcept override final;

private:
    const milliseconds_t        iCheckPeriod = 333.337_si_ms;
    TimersT                     iTimers;
    TimersPoolT                 iTimersPool;

    static GpTimersManager::SP  sTimersManager;
    static std::atomic_flag     sTimersThreadDestruct;
    static GpThread             sTimersThread;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_TIMERS)
