#include "GpConditionVar.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

bool    GpConditionVar::WaitForWakeup (const milliseconds_t aTimeout) noexcept
{
    std::unique_lock lock(iWakeupMutex);

    iWaitCounter++;
    bool res = true;

    try
    {
        if (aTimeout > 0.0_si_s)
        {
            iWakeupCV.wait_for
            (
                lock,
                std::chrono::milliseconds(aTimeout.As<ssize_t>()),
                [&]
                {
                    if (iWakeUpState == WakeUpState::ONE)
                    {
                        iWakeUpState = WakeUpState::WAIT;
                        return true;
                    } else if (iWakeUpState == WakeUpState::ALL)
                    {
                        return true;
                    } else
                    {
                        return false;
                    }
                }
            );
        } else
        {
            iWakeupCV.wait
            (
                lock,
                [&]
                {
                    if (iWakeUpState == WakeUpState::ONE)
                    {
                        iWakeUpState = WakeUpState::WAIT;
                        return true;
                    } else if (iWakeUpState == WakeUpState::ALL)
                    {
                        return true;
                    } else
                    {
                        return false;
                    }
                }
            );
        }
    } catch (const std::exception& e)
    {
        res = false;
    } catch (...)
    {
        res = false;
    }

    iWaitCounter--;

    if (iWaitCounter == 0)
    {
        iWakeUpState = WakeUpState::WAIT;
    }

    return res;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
