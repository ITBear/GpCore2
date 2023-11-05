#include "GpTimersManager.hpp"

#if defined(GP_USE_TIMERS)

#include "../../Types/Strings/GpStringUtils.hpp"
#include "../../Types/Strings/GpStringOps.hpp"
#include "../../DateTime/GpDateTimeOps.hpp"

namespace GPlatform {

GpTimersManager::SP     GpTimersManager::sTimersManager;
std::atomic_flag        GpTimersManager::sTimersThreadDestruct;
GpThread                GpTimersManager::sTimersThread(u8"Timers manager");

void    GpTimersManager::SStart (void)
{
    if (sTimersManager.IsNULL())
    {
        sTimersManager = MakeSP<GpTimersManager>();
        sTimersThread.Run(sTimersManager);
    }
}

void    GpTimersManager::SStop (void)
{
    if (sTimersManager.IsNotNULL())
    {
        sTimersThread.RequestStop();
        sTimersThread.Join();
        sTimersManager.Clear();
    }
}

void    GpTimersManager::SSingleShot
(
    GpTimer::CallbackFnT&&  aCallbackFn,
    const milliseconds_t    aDelayBeforeShot
)
{
    GpTimersManager&    manager         = GpTimersManager::SManager();
    GpTimer::C::Opt::SP timerOpt        = manager.iTimersPool.Acquire();
    const bool          isReturnToPool  = timerOpt.has_value();
    GpTimer::SP         timer;

    if (isReturnToPool) [[likely]]
    {
        timer = std::move(timerOpt.value());
    } else
    {
        timer = MakeSP<GpTimer>();
    }

    timer->Reload
    (
        std::move(aCallbackFn),
        0.0_si_s,
        aDelayBeforeShot,
        u_int_64(1),
        isReturnToPool
    );

    manager.AddTimer(timer);
    timer->Start();
}

void    GpTimersManager::AddTimer (GpTimer::SP aTimer)
{
    iTimers.Set(aTimer.P(), std::move(aTimer));
}

void    GpTimersManager::Run (std::atomic_flag& aStopRequest) noexcept
{
    try
    {
        // Init timers pool
        iTimersPool.Init(0, 1024);// TODO: move to config

        // Wait for stop
        while (!aStopRequest.test())
        {
            const milliseconds_t startSTS = GpDateTimeOps::SSteadyTS_ms();

            iTimers.Process
            (
                [&](auto& aTimers)
                {
                    for (auto iter = aTimers.begin(); iter != aTimers.end(); /*NOP*/)
                    {
                        GpTimer& timer = iter->second.V();

                        if (timer.TryMakeShot() == GpTimer::ShotRes::REMOVE)
                        {
                            if (timer.IsReturnToPool())
                            {
                                iTimersPool.Release(std::move(iter->second));
                            }

                            iter = aTimers.erase(iter);
                        } else
                        {
                            iter++;
                        }
                    }
                }
            );

            const milliseconds_t endSTS         = GpDateTimeOps::SSteadyTS_ms();
            const milliseconds_t waitTimeout    = iCheckPeriod - (endSTS - startSTS);

            if (waitTimeout > 0.0_si_ms) [[likely]]
            {
                WaitForAndReset(waitTimeout);
            }
        }
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(u8"[GpTimersManager::Run]: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpTimersManager::Run]: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpTimersManager::Run]: unknown"_sv);
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
