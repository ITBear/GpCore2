#include <GpCore2/GpTasks/Scheduler/V1/GpTaskExecutorV1.hpp>
#include <GpCore2/GpTasks/Scheduler/V1/GpTaskSchedulerV1.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskExecutorV1::GpTaskExecutorV1
(
    const size_t        aId,
    GpTaskSchedulerV1&  aTasksScheduler,
    ReadyTasksQueueT&   aReadyTasksQueue,
    DonePromiseT&&      aDonePromise
) noexcept:
iId             {aId},
iTasksScheduler {aTasksScheduler},
iReadyTasksQueue{aReadyTasksQueue},
iDonePromise    {std::move(aDonePromise)}
{
}

GpTaskExecutorV1::~GpTaskExecutorV1 (void) noexcept
{
}

void    GpTaskExecutorV1::Run (std::atomic_flag& aStopRequest) noexcept
{
    std::string exMsg;

    try
    {
        // Repeat until stop requested
        while (!aStopRequest.test())
        {
            // Consume next task
            GpTask::C::Opt::SP taskOpt = iReadyTasksQueue.WaitAndPop(0.25_si_s);

            if (!taskOpt.has_value())
            {
                continue;
            }

            GpTask::SP& taskSP  = taskOpt.value();
            GpTask&     task    = taskSP.V();

            // Run task
            const GpTaskRunRes::EnumT taskRes = task.Execute(GpMethodAccess<GpTaskExecutor>{this});

            // Reschedule task
            if (iTasksScheduler.Reschedule(taskRes, std::move(taskSP)) == false)
            {
                THROW_GP("Failed to Reschedule"_sv);
            }
        }

        iDonePromise.Fulfill(ssize_t{1});

        return;
    } catch (const GpException& e)
    {
        exMsg = fmt::format
        (
            "[GpTaskExecutor::Run]: executor id: {} , exception: {}",
            Id(),
            e.what()
        );
    } catch (const std::exception& e)
    {
        exMsg = fmt::format
        (
            "[GpTaskExecutor::Run]: executor id: {} , exception: {}",
            Id(),
            e.what()
        );
    } catch (...)
    {
        exMsg = fmt::format
        (
            "[GpTaskExecutor::Run]: executor id: {} , exception: {}",
            Id(),
            "unknown exception"
        );
    }

    // GpStringUtils::SCerr(exMsg);
    iDonePromise.Fulfill(GpException{std::move(exMsg)});

    // Stop service
    GpTaskScheduler::S().StopService();
}

void    GpTaskExecutorV1::OnNotify (void) noexcept
{
    iReadyTasksQueue.Interrupt();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
