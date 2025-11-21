#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskSchedulerFactory.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskScheduler::UP GpTaskScheduler::sInstance;
std::atomic_flag    GpTaskScheduler::sRequestStopAndJoinCall = ATOMIC_FLAG_INIT;

GpTaskScheduler::GpTaskScheduler
(
    const size_t aExecutorsCount,
    const size_t aTasksMaxCount
) noexcept:
iExecutorsCount{aExecutorsCount},
iTasksMaxCount {aTasksMaxCount}
{
}

void    GpTaskScheduler::SStart
(
    const GpTaskSchedulerFactory&   aFactory,
    const size_t                    aExecutorsCount,
    const size_t                    aTasksMaxCount
)
{
    VERIFY
    (
        aExecutorsCount > 0,
        "Executors count must be > 0"_sv
    );

    VERIFY
    (
        aTasksMaxCount > 0,
        "Tasks max count must be > 0"_sv
    );

    sRequestStopAndJoinCall.clear(std::memory_order_release);

    // Create instance
    sInstance = aFactory.NewInstance(aExecutorsCount, aTasksMaxCount);

    // Start
    sInstance->Start();
}

void    GpTaskScheduler::SStop (void)
{
    if (sRequestStopAndJoinCall.test_and_set(std::memory_order_acq_rel))
    {
        // Only single call allowed
        return;
    }

    if (sInstance != nullptr)
    {
        try
        {
            S().RequestStopAndJoin();
        } catch (const GpException& e)
        {
            GpOutUtils::S().Err("[GpTaskScheduler::SStop]: exception: "_sv + e.what());
        } catch (const std::exception& e)
        {
            GpOutUtils::S().Err("[GpTaskScheduler::SStop]: exception: "_sv + e.what());
        } catch (...)
        {
            GpOutUtils::S().Err("[GpTaskScheduler::SStop]: unknown exception"_sv);
        }

        sInstance.reset();
    }
}

GpTask::DoneFutureT::SP GpTaskScheduler::RequestStop (GpTask& aTask)
{
    GpTask::DoneFutureT::SP doneFutureSP = aTask.DoneFuture();
    aTask.UpStopRequestFlag(GpMethodAccess{this});

    Wakeup(aTask);

    return doneFutureSP;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
