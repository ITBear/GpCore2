#pragma once

#include "../../GpMacro.hpp"

#if defined(GP_USE_TIMERS)

#include "../../Types/Containers/GpElementsCatalog.hpp"
#include "../../Threads/GpThread.hpp"
#include "GpTimerShotEventFactory.hpp"
#include "GpTimer.hpp"

namespace GPlatform {

class GP_UTILS_API GpTimersManager final: public GpRunnable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTimersManager)
    CLASS_DECLARE_DEFAULTS(GpTimersManager)

    using TimersT = GpElementsCatalog<const void*, GpTimer::SP>;

public:
    static void                     SStart              (void);
    static void                     SStop               (void);
    static GpTimersManager&         SManager            (void) {return sTimersManager.V();}

                                    GpTimersManager     (void) noexcept;
                                    ~GpTimersManager    (void) noexcept;

public:
    void                            AddTimer            (GpTimer::SP aTimer);

    virtual void                    Run                 (GpThreadStopToken aStopToken) noexcept override final;

private:
    milliseconds_t                  iStep       = 500.0_si_ms;
    TimersT                         iTimers;

    static GpTimersManager::SP      sTimersManager;
    static GpThread                 sTimersThread;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
