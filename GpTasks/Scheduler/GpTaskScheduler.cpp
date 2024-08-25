#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskSchedulerFactory.hpp>

namespace GPlatform {

GpTaskScheduler::SP GpTaskScheduler::sInstance;

GpTaskScheduler::GpTaskScheduler (StopServiceFnT aStopServiceFn) noexcept:
iStopServiceFn{aStopServiceFn}
{
}

void    GpTaskScheduler::SStart
(
    const GpTaskSchedulerFactory&   aFactory,
    const size_t                    aExecutorsCount,
    const size_t                    aTasksMaxCount,
    StopServiceFnT                  aStopServiceFn
)
{
    sInstance = aFactory.NewInstance(aStopServiceFn);

    S().Start
    (
        aExecutorsCount,
        aTasksMaxCount
    );
}

void    GpTaskScheduler::SStopAndClear (void)
{
    if (GpTaskScheduler::sInstance.IsNotNULL())
    {
        S().RequestStopAndJoin();
        sInstance.Clear();
    }
}

void    GpTaskScheduler::StopService (void)
{
    iStopServiceFn();
}

GpTask::DoneFutureT::SP GpTaskScheduler::NewToReadyDepend (GpSP<GpTask> aTaskSP)
{
    GpTask::DoneFutureT::SP doneFuture = aTaskSP->GetDoneFuture();
    NewToReady(std::move(aTaskSP));
    return doneFuture;
}

GpTask::DoneFutureT::SP GpTaskScheduler::RequestStop (GpTask& aTask)
{
    GpTask::DoneFutureT::SP doneFuture = aTask.GetDoneFuture();
    aTask.UpStopRequestFlag(GpMethodAccess<GpTaskScheduler>{this});
    MakeTaskReady(aTask.TaskId());
    return doneFuture;
}

void    GpTaskScheduler::Start
(
    const size_t aExecutorsCount,
    const size_t aTasksMaxCount
)
{
    iExecutorsCount = aExecutorsCount;
    iTasksMaxCount  = aTasksMaxCount;
}

}// namespace GPlatform
