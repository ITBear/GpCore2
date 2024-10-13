#include <GpCore2/GpUtils/Threads/Timers/GpTimersManager.hpp>

#if defined(GP_USE_TIMERS)

#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>

namespace GPlatform {

GpTimersManager::SP GpTimersManager::sTimersManager;
std::atomic_flag    GpTimersManager::sTimersThreadDestruct;
GpThread            GpTimersManager::sTimersThread("Timers manager");

GpTimersManager::GpTimersManager (void) noexcept
{
    iIsShotsEnabled.test_and_set();
}

GpTimersManager::~GpTimersManager (void) noexcept
{
}

void    GpTimersManager::SStart (void)
{
    if (sTimersManager.IsNULL())
    {
        sTimersManager = MakeSP<GpTimersManager>();
        sTimersThread.Run(sTimersManager);      
    }
}

void    GpTimersManager::SDisableShots (void)
{
    if (sTimersManager.IsNotNULL())
    {
        sTimersManager.Vn().DisableShots();
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

GpTimer::SP GpTimersManager::SSingleShot
(
    GpTimer::CallbackFnT&&  aCallbackFn,
    const milliseconds_t    aDelayBeforeShot,
    const bool              aUseTimersPool
)
{
    GpTimersManager::SP managerSP       = GpTimersManager::SManager();
    GpTimersManager&    manager         = managerSP.V();
    GpTimer::C::Opt::SP timerOpt        = aUseTimersPool ? manager.iTimersPool.Acquire() : std::nullopt;
    const bool          isReturnToPool  = timerOpt.has_value();
    GpTimer::SP         timerSP;

    if (isReturnToPool) [[likely]]
    {
        timerSP = std::move(timerOpt.value());
    } else
    {
        timerSP = MakeSP<GpTimer>();
    }

    timerSP.Vn().Reload
    (
        std::move(aCallbackFn),
        0.0_si_s,
        aDelayBeforeShot,
        u_int_64(1),
        isReturnToPool
    );

    manager.AddTimer(timerSP);
    timerSP->Start();

    return timerSP;
}

void    GpTimersManager::AddTimer (GpTimer::SP aTimer)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iTimersToAddSpinLock};

    iTimersToAdd.emplace_back(std::move(aTimer));
}

void    GpTimersManager::Run (std::atomic_flag& aStopRequest) noexcept
{
    try
    {
        // Init timers pool
        iTimersPool.Init(0, 128);// TODO: move to config

        // Wait for stop
        while (!aStopRequest.test())
        {
            const milliseconds_t startSTS = GpDateTimeOps::SSteadyTS_ms();

            if (iIsShotsEnabled.test()) [[likely]]
            {
                // Add new timers
                {
                    GpUniqueLock<GpSpinLock> uniqueLock{iTimersToAddSpinLock};

                    for (GpTimer::SP& timerToAddSP: iTimersToAdd)
                    {
                        const GpTimer* p = timerToAddSP.P();
                        iActiveTimers.emplace(p, std::move(timerToAddSP));
                    }

                    iTimersToAdd.clear();
                }

                // Check all active timers
                for (auto iter = std::begin(iActiveTimers); iter != std::end(iActiveTimers); /*NOP*/)
                {
                    GpTimer& timer = iter->second.V();

                    if (timer.TryMakeShot() == GpTimer::ShotRes::REMOVE)
                    {
                        if (timer.IsReturnToPool())
                        {
                            iTimersPool.Release(std::move(iter->second));
                        }

                        iter = iActiveTimers.erase(iter);
                    } else
                    {
                        iter++;
                    }
                }
            }

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
        GpStringUtils::SCerr("[GpTimersManager::Run]: unknown");
    }
}

void    GpTimersManager::OnNotify (void) noexcept
{
    // NOP
}

void    GpTimersManager::DisableShots (void)
{
    iIsShotsEnabled.clear();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_TIMERS)
