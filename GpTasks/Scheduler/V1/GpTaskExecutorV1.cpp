#include <GpCore2/GpTasks/Scheduler/V1/GpTaskExecutorV1.hpp>
#include <GpCore2/GpTasks/Scheduler/V1/GpTaskSchedulerV1.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskExecutorV1::GpTaskExecutorV1
(
    const size_t        aId,
    GpTaskSchedulerV1&  aScheduler,
    TaskQueueT::SP      aTaskQueueSP,
    DonePromiseT&&      aDonePromise
) noexcept:
iId         {aId},
iScheduler  {aScheduler},
iTaskQueueSP{std::move(aTaskQueueSP)},
iDonePromise{std::move(aDonePromise)}
{
}

GpTaskExecutorV1::~GpTaskExecutorV1 (void) noexcept
{
}

void    GpTaskExecutorV1::Run (GpConditionVarFlag& aStopFlag) noexcept
{
    std::string exMsg;

    try
    {
        TaskQueueT& taskQueue = iTaskQueueSP.V();

        // Repeat until stop requested
        bool stopFlagValue = aStopFlag.Test();
        while (!stopFlagValue)
        {
            // Consume next task
            auto                popRes  = taskQueue.PopWaitFor(100.0_si_ms);
            GpTask::C::Opts::SP taskOpt = std::move(popRes.iValue);

            if (taskOpt.has_value() == false)
            {
                if (popRes.iFlags & (TaskQueueT::FlagsT(TaskQueueT::FlagE::INTERRUPT) | TaskQueueT::FlagsT(TaskQueueT::FlagE::STOP_PRODUCE)))
                {
                    break;
                }

                continue;
            }

            // Run task
            GpTask::SP&                 taskSP  = taskOpt.value();
            const GpTaskRunRes::EnumT   taskRes = GpTask::SExecute(taskSP, GpMethodAccess{this});

            // Reschedule task
            if (iScheduler.Reschedule(taskRes, taskSP, GpMethodAccess{this}) == false)
            {
                // Scheduler stopped
                if (taskRes != GpTaskRunRes::DONE)
                {
                    std::ignore = taskQueue.Push(taskSP);
                }

                break;
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

    iDonePromise.Fulfill(GpException{std::move(exMsg)});
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
