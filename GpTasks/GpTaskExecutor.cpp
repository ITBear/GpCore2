#include "GpTaskExecutor.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskScheduler.hpp"
#include "GpTaskAccessor.hpp"

namespace GPlatform {

void    GpTaskExecutor::Run (GpThreadStopToken aStopToken) noexcept
{
    GpTaskScheduler& taskScheduler = GpTaskScheduler::S();

    GpTask::SP  currentTaskSP;
    GpTaskDoRes currentTaskExecRes = GpTaskDoRes::DONE;

    while (!aStopToken.stop_requested())
    {
        currentTaskSP = taskScheduler._Reshedule(std::move(currentTaskSP), currentTaskExecRes);

        if (currentTaskSP.IsNotNULL())
        {
            currentTaskExecRes = GpTaskAccessor::SRun(currentTaskSP.Vn(), aStopToken);
        } else
        {
            std::ignore = WaitForWakeup(1.0_si_s);
            currentTaskExecRes = GpTaskDoRes::DONE;
        }
    }

    if (currentTaskSP.IsNotNULL())
    {
        //GpTaskAccessor::SRun(currentTaskSP.Vn(), aStopToken);
        GpTaskScheduler::_SDestroyTask(currentTaskSP.Vn());
        currentTaskSP.Clear();
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
