#include "GpTimersManager.hpp"

#if defined(GP_USE_TIMERS)

namespace GPlatform {

GpTimersManager::SP GpTimersManager::sTimersManager;
GpThread            GpTimersManager::sTimersThread;

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

GpTimersManager::GpTimersManager (void) noexcept:
GpRunnable(MakeSP<GpConditionVar>())
{
}

GpTimersManager::~GpTimersManager (void) noexcept
{
}

void    GpTimersManager::AddTimer (GpTimer::SP aTimer)
{
    iTimers.Register(aTimer.PC(), std::move(aTimer));
}

void    GpTimersManager::Run (GpThreadStopToken aStopToken) noexcept
{
    while (!aStopToken.stop_requested())
    {
        iTimers.Process([](auto& aTimers)
        {
            for (auto iter = aTimers.begin(); iter != aTimers.end(); )
            {
                if (iter->second->TryMakeShot() == GpTimer::ShotRes::REMOVE)
                {
                    iter = aTimers.erase(iter);
                } else
                {
                    iter++;
                }
            }
        });

        //WaitForWakeup(iStep);
        std::this_thread::sleep_for(std::chrono::milliseconds(iStep.As<ssize_t>()));
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
