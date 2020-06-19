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

void	GpConditionVar::WakeupAll (void) noexcept
{
    {
        std::scoped_lock lock(iWakeupMutex);
        iNeedToWakeUp = true;
    }

    iWakeupCV.notify_all();
}

void	GpConditionVar::WakeupOne (void) noexcept
{
    {
        std::scoped_lock lock(iWakeupMutex);
        iNeedToWakeUp = true;
    }

    iWakeupCV.notify_one();
}

void	GpConditionVar::WaitForWakeup (void) noexcept
{
    WaitForWakeup(0.0_si_s);
}

void	GpConditionVar::WaitForWakeup (const milliseconds_t aTimeout) noexcept
{
    std::unique_lock lock(iWakeupMutex);

    iWaitCounter++;

    try
    {
        if (aTimeout > 0.0_si_s)
        {
            iWakeupCV.wait_for(lock,
                               std::chrono::milliseconds(size_t(aTimeout.Value())),
                               [&]{return iNeedToWakeUp;});
        } else
        {
            iWakeupCV.wait(lock,
                           [&]{return iNeedToWakeUp;});
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
        iNeedToWakeUp = false;
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
