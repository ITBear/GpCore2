#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskSchedulerFactory.hpp>

#if defined(GP_USE_MULTITHREADING)

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

GpTask::DoneFutureT::C::Opts::SP GpTaskScheduler::NewToReadyDepend (GpSP<GpTask> aTaskSP)
{
    GpTask::DoneFutureT::SP doneFutureSP = aTaskSP->DoneFuture();

    return NewToReady(std::move(aTaskSP)) ? GpTask::DoneFutureT::C::Opts::SP{doneFutureSP} : std::nullopt;
}

GpTask::DoneFutureT::C::Opts::SP    GpTaskScheduler::RequestStop (GpTask& aTask)
{
    GpTask::DoneFutureT::SP doneFutureSP = aTask.DoneFuture();
    aTask.UpStopRequestFlag(GpMethodAccess{this});

    return MakeTaskReady(aTask.TaskId()) ? GpTask::DoneFutureT::C::Opts::SP{doneFutureSP} : std::nullopt;
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

#endif// #if defined(GP_USE_MULTITHREADING)
