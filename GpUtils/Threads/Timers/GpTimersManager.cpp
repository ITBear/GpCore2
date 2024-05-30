#include "GpTimersManager.hpp"

#if defined(GP_USE_TIMERS)

#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>

namespace GPlatform {

GpTimersManager::SP GpTimersManager::sTimersManager;
std::atomic_flag    GpTimersManager::sTimersThreadDestruct;
GpThread            GpTimersManager::sTimersThread("Timers manager");

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

bool    GpTimersManager::SSingleShot
(
    GpTimer::CallbackFnT&&  aCallbackFn,
    const milliseconds_t    aDelayBeforeShot
)
{
    GpTimersManager::SP managerSP = GpTimersManager::SManager();

    if (managerSP.IsNULL()) [[unlikely]]
    {
        return false;
    }

    GpTimersManager&    manager         = managerSP.Vn();
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

    return true;
}

void    GpTimersManager::AddTimer (GpTimer::SP aTimer)
{
    iTimers.SetOrUpdate(aTimer.P(), std::move(aTimer));
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

            iTimers.ProcessContainer
            (
                [&](auto& aTimers)
                {
                    for (auto iter = std::begin(aTimers); iter != std::end(aTimers); /*NOP*/)
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
        GpStringUtils::SCerr
        (
            fmt::format("[GpTimersManager::Run]: {}", e.what())
        );
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr
        (
            fmt::format("[GpTimersManager::Run]: {}", e.what())
        );
    } catch (...)
    {
        GpStringUtils::SCerr
        (
            "[GpTimersManager::Run]: unknown"
        );
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_TIMERS)
