#include "GpTimersManager.hpp"

#if defined(GP_USE_TIMERS)

#include "../../Types/Strings/GpStringUtils.hpp"
#include "../../Types/Strings/GpStringOps.hpp"

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

void    GpTimersManager::AddTimer (GpTimer::SP aTimer)
{
    iTimers.Set(aTimer.P(), std::move(aTimer));
}

void    GpTimersManager::Run (GpThreadStopToken aStopToken) noexcept
{
    try
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

            if (CVF().WaitFor(iStep) == GpConditionVarFlag::WaitForResT::OK)
            {
                break;
            } else //GpConditionVarFlag::WaitForResT::TIMEOUT
            {
                //continue;
            }
        }
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(u8"[GpTimersManager::Run]: "_sv + e.what() + "\n"_sv);
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpTimersManager::Run]: "_sv + e.what() + "\n"_sv);
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpTimersManager::Run]: unknown\n"_sv);
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
