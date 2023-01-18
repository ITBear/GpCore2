#include "GpTaskExecutor.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskScheduler.hpp"
#include "GpTaskAccessor.hpp"
#include "GpTaskExecutorsPool.hpp"

namespace GPlatform {

void    GpTaskExecutor::Run (GpThreadStopToken aStopToken) noexcept
{
    GpTask::SP  currentTaskSP;

    try
    {
        iExecutorsPool.MarkAsBusy(iId);

        GpTaskScheduler& taskScheduler = GpTaskScheduler::S();

        GpTaskDoRes currentTaskExecRes = GpTaskDoRes::DONE;

        while (!aStopToken.stop_requested())
        {
            currentTaskSP = taskScheduler._Reshedule(std::move(currentTaskSP), currentTaskExecRes);

            if (currentTaskSP.IsNotNULL())
            {
                GpTask& task = currentTaskSP.Vn();

                task.DownRunRequestFlag();
                currentTaskExecRes = GpTaskAccessor::SRun(task, aStopToken);
            } else
            {
                iExecutorsPool.MarkAsIdle(iId);
                std::ignore = CVF().WaitForAndReset(0.5_si_s);
                iExecutorsPool.MarkAsBusy(iId);
                currentTaskExecRes = GpTaskDoRes::DONE;
            }
        }

        if (currentTaskSP.IsNotNULL())
        {
            GpTaskScheduler::S()._DestroyTaskSP(std::move(currentTaskSP));
            currentTaskSP.Clear();
        }
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr("[GpTaskExecutor::Run]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr("[GpTaskExecutor::Run]: exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr("[GpTaskExecutor::Run]: unknown exception"_sv);
    }

    if (currentTaskSP.IsNotNULL())
    {
        try
        {
            GpTaskScheduler::S()._DestroyTaskSP(std::move(currentTaskSP));
            currentTaskSP.Clear();
        } catch (const GpException& e)
        {
            GpStringUtils::SCerr("[GpTaskExecutor::Run]: exception: "_sv + e.what());
        } catch (const std::exception& e)
        {
            GpStringUtils::SCerr("[GpTaskExecutor::Run]: exception: "_sv + e.what());
        } catch (...)
        {
            GpStringUtils::SCerr("[GpTaskExecutor::Run]: unknown exception"_sv);
        }
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
