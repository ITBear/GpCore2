#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_TIMERS)

#include "../../Types/Containers/GpDictionary.hpp"
#include "../../Types/Containers/GpElementsPool.hpp"
#include "../../Threads/GpThread.hpp"
#include "GpTimer.hpp"

namespace GPlatform {

class GP_UTILS_API GpTimersManager final: public GpRunnable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTimersManager)
    CLASS_DD(GpTimersManager)
    TAG_SET(THREAD_SAFE)

    using TimersT = GpDictionary<const void*, GpTimer::SP>;

    class TimersPoolT final: public GpElementsPool<GpTimer::SP>
    {
    public:
                                TimersPoolT         (void) noexcept = default;
                                ~TimersPoolT        (void) noexcept = default;

    protected:
        virtual value_type      NewElement          (GpSpinlock& /*aLocked*/) override final
        {
            return MakeSP<GpTimer>();
        }
    };

public:
                                GpTimersManager     (void) noexcept = default;
                                ~GpTimersManager    (void) noexcept final = default;

    static void                 SStart              (void);
    static void                 SStop               (void);
    static GpTimersManager&     SManager            (void) {return sTimersManager.V();}

    static void                 SSingleShot         (GpTimer::CallbackFnT&& aCallbackFn,
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

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
