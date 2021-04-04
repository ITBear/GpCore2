#include "GpTaskExecutor.hpp"
#include "../../Types/DateTime/GpDateTime.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskScheduler.hpp"

namespace GPlatform {

GpTaskExecutor::GpTaskExecutor
(
    GpWP<GpTaskScheduler>   aScheduler,
    GpConditionVar::SP      aCondVar
) noexcept:
GpRunnable(std::move(aCondVar)),
iScheduler(std::move(aScheduler))
{
}

GpTaskExecutor::~GpTaskExecutor (void) noexcept
{
}

void    GpTaskExecutor::Run (GpThreadStopToken aStopToken) noexcept
{
    GpTaskScheduler::SSetCurrentScheduler(iScheduler);

    GpTask::SP      currentTask;
    GpTask::ResT    currentTaskExecRes = GpTask::ResT::DONE;

    while (!aStopToken.stop_requested())
    {
        currentTask = iScheduler.Vn().Reshedule(currentTask, currentTaskExecRes);

        if (currentTask.IsNotNULL())
        {
            currentTaskExecRes = currentTask.Vn().Do(aStopToken);
        } else
        {
            WaitForWakeup(10.0_si_s);
            currentTaskExecRes = GpTask::ResT::DONE;
        }
    }

    if (currentTask.IsNotNULL())
    {
        currentTaskExecRes = currentTask.Vn().Do(aStopToken);
    }

    GpTaskScheduler::SSetCurrentScheduler(std::nullopt);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
