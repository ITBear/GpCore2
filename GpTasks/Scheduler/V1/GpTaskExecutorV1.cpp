#include "GpTaskExecutorV1.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../../../GpService/GpService.hpp"
#include "GpTaskSchedulerV1.hpp"

namespace GPlatform {

GpTaskExecutorV1::GpTaskExecutorV1
(
    const size_t        aId,
    GpTaskSchedulerV1&  aTasksScheduler,
    ReadyTasksQueueT&   aReadyTasksQueue,
    DonePromiseT&&      aDonePromise
) noexcept:
iId             (aId),
iTasksScheduler (aTasksScheduler),
iReadyTasksQueue(aReadyTasksQueue),
iDonePromise    (std::move(aDonePromise))
{
}

GpTaskExecutorV1::~GpTaskExecutorV1 (void) noexcept
{
}

void    GpTaskExecutorV1::Run (std::atomic_flag& aStopRequest) noexcept
{
    std::u8string exMsg;

    try
    {
        // Repeat until stop requested
        while (!aStopRequest.test())
        {
            // Consume next task
            GpTask::C::Opt::SP  taskOpt = iReadyTasksQueue.WaitAndPop(0.5_si_s);

            if (!taskOpt.has_value())
            {
                continue;
            }

            GpTask::SP& taskSP  = taskOpt.value();
            GpTask&     task    = taskSP.V();

            // Run task
            const GpTaskRunRes::EnumT taskRes = task.Execute();

            // Reschedule task
            if (iTasksScheduler.Reschedule(taskRes, std::move(taskSP)) == false)
            {
                THROW_GP(u8"Failed to Reschedule"_sv);
            }
        }

        iDonePromise.Fulfill(ssize_t(1));

        return;
    } catch (const GpException& e)
    {
        exMsg = u8"[GpTaskExecutor::Run]: executor id: "_sv + Id() + u8", exception: "_sv + e.what();
    } catch (const std::exception& e)
    {
        exMsg = u8"[GpTaskExecutor::Run]: executor id: "_sv + Id() + u8", exception: "_sv + e.what();
    } catch (...)
    {
        exMsg = u8"[GpTaskExecutor::Run]: executor id: "_sv + Id() + u8",  unknown exception"_sv;
    }

    GpStringUtils::SCerr(exMsg);
    iDonePromise.Fulfill(GpException(exMsg));

    // Stop service
    GpService::SRequestStop();
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
