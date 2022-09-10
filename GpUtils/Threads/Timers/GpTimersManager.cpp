#include "GpTimersManager.hpp"

#if defined(GP_USE_TIMERS)

#include <iostream>

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
    iTimers.Register(aTimer.P(), std::move(aTimer));
}

void    GpTimersManager::Run (GpThreadStopToken aStopToken) noexcept
{
    while (!aStopToken.stop_requested())
    {
        iTimers.Process([](auto& aTimers)
        {
            for (auto iter = aTimers.begin(); iter != aTimers.end(); )
            {
                if (iter->second.V().TryMakeShot() == GpTimer::ShotRes::REMOVE)
                {
                    iter = aTimers.erase(iter);
                } else
                {
                    iter++;
                }
            }
        });

        if (WaitForWakeup(iStep) == false)
        {
            std::cerr << "[GpTimersManager::Run]: WaitForWakeup failed..." << std::endl;
            break;
        }
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
