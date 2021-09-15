#include "GpConditionVar.hpp"
#include "../../Exceptions/GpExceptionsSink.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpConditionVar::GpConditionVar (void) noexcept
{
}

GpConditionVar::~GpConditionVar (void) noexcept
{
}

void    GpConditionVar::WakeupAll (void) noexcept
{
    {
        std::scoped_lock lock(iWakeupMutex);
        iWakeUpState = WakeUpState::ALL;
    }

    iWakeupCV.notify_all();
}

void    GpConditionVar::WakeupOne (void) noexcept
{
    {
        std::scoped_lock lock(iWakeupMutex);
        iWakeUpState = WakeUpState::ONE;
    }

    iWakeupCV.notify_one();
}

void    GpConditionVar::WaitForWakeup (void) noexcept
{
    WaitForWakeup(0.0_si_s);
}

void    GpConditionVar::WaitForWakeup (const milliseconds_t aTimeout) noexcept
{
    std::unique_lock lock(iWakeupMutex);

    iWaitCounter++;

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
        GpExceptionsSink::SSink(e);
    } catch (...)
    {
        GpExceptionsSink::SSinkUnknown();
    }

    iWaitCounter--;

    if (iWaitCounter == 0_cnt)
    {
        iWakeUpState = WakeUpState::WAIT;
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
