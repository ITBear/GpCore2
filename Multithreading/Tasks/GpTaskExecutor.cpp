#include "GpTaskExecutor.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskScheduler.hpp"

namespace GPlatform {

GpTaskExecutor::GpTaskExecutor (GpTaskScheduler&    aScheduler,
                                GpConditionVar::SP  aCondVar) noexcept:
GpRunnable(std::move(aCondVar)),
iScheduler(aScheduler)
{
}

GpTaskExecutor::~GpTaskExecutor (void) noexcept
{
}

void    GpTaskExecutor::Run (GpThreadStopToken aStopToken) noexcept
{
    GpTask::SP  currentTask;
    GpTask::Res currentTaskExecRes = GpTask::Res::DONE;

    while (!aStopToken.stop_requested())
    {
        currentTask = iScheduler.Reshedule(currentTask, currentTaskExecRes);

        if (currentTask.IsNotNULL())
        {
            currentTaskExecRes = currentTask.Vn().Do(aStopToken);
        } else
        {
            WaitForWakeup(10.0_si_s);
            currentTaskExecRes = GpTask::Res::DONE;
        }
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
