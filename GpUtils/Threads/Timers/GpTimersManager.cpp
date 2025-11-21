#include <GpCore2/GpUtils/Threads/Timers/GpTimersManager.hpp>

#if defined(GP_USE_TIMERS)

#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>

namespace GPlatform {

GpTimersManager*    GpTimersManager::sTimersManager = nullptr;
GpConditionVarFlag  GpTimersManager::sTimersThreadStopFlag;
GpThread            GpTimersManager::sTimersThread(GpTimersManager::sTimersThreadStopFlag, "Timers manager");

GpTimersManager::GpTimersManager (void) noexcept
{
    iIsShotsEnabled.test_and_set();
}

GpTimersManager::~GpTimersManager (void) noexcept
{
}

void    GpTimersManager::SStart (void)
{
    if (sTimersManager == nullptr)
    {
        GpTimersManager::UP timersManagerUP = std::make_unique<GpTimersManager>();
        sTimersManager = timersManagerUP.get();
        sTimersThread.Run(std::move(timersManagerUP));
    }
}

void    GpTimersManager::SDisableShots (void)
{
    if (sTimersManager != nullptr)
    {
        sTimersManager->DisableShots();
    }
}

void    GpTimersManager::SStop (void)
{
    if (sTimersManager != nullptr)
    {
        sTimersThread.RequestStop();
        sTimersThread.Join();
        sTimersManager = nullptr;
    }
}

GpTimer::SP GpTimersManager::SSingleShot
(
    GpTimer::CallbackFnT&&  aCallbackFn,
    const milliseconds_t    aDelayBeforeShot
)
{
    GpTimersManager&        manager         = GpTimersManager::S();
    GpTimer::C::Opts::SP    timerOpt        = manager.iTimersPool.Acquire();
    const bool              isReturnToPool  = timerOpt.has_value();
    GpTimer::SP             timerSP;

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
        0.0_si_ms,
        aDelayBeforeShot,
        u_int_64{1},
        isReturnToPool
    );

    manager.AddTimer(timerSP);
    timerSP->Start();

    return timerSP;
}

void    GpTimersManager::AddTimer (GpTimer::SP aTimer)
{
    GpUniqueLock uniqueLock{iTimersToAddSpinLock};

    iTimersToAdd.emplace_back(std::move(aTimer));
}

void    GpTimersManager::Run (GpConditionVarFlag& aStopFlag) noexcept
{
    try
    {
        // Init timers pool
        iTimersPool.Init(0, 128);// TODO: move to config

        // Wait for stop
        bool stopFlagValue = aStopFlag.Test();
        while (!stopFlagValue)
        {
            const milliseconds_t startSTS = GpDateTimeOps::SSteadyTS_ms();

            if (iIsShotsEnabled.test()) [[likely]]
            {
                // Add new timers
                {
                    GpUniqueLock uniqueLock{iTimersToAddSpinLock};

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
                stopFlagValue = aStopFlag.WaitFor(waitTimeout);
            } else
            {
                stopFlagValue = aStopFlag.Test();
            }
        }
    } catch (const GpException& e)
    {
        GpOutUtils::S().Err
        (
            fmt::format("[GpTimersManager::Run]: {}", e.what())
        );
    } catch (const std::exception& e)
    {
        GpOutUtils::S().Err
        (
            fmt::format("[GpTimersManager::Run]: {}", e.what())
        );
    } catch (...)
    {
        GpOutUtils::S().Err("[GpTimersManager::Run]: unknown");
    }
}

void    GpTimersManager::DisableShots (void)
{
    iIsShotsEnabled.clear();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_TIMERS)
