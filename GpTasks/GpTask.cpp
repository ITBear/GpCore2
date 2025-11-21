#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/ITC/GpItcFutureUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

thread_local GpTask::WP __GpTask__thread_current_task = nullptr;
GpTask::IdCounterT      GpTask::sIdCounter = {1};

GpTask::GpTask
(
    std::string             aName,
    const GpTaskMode::EnumT aTaskMode
) noexcept:
iName{std::move(aName)},
iId  {SNextId()},
iMode{aTaskMode}
{
}

GpTask::GpTask (const GpTaskMode::EnumT aTaskMode) noexcept:
iId  {SNextId()},
iMode{aTaskMode}
{
}

GpTask::~GpTask (void) noexcept
{
    try
    {
        // Fulfill promises
        StartPromise(GpMethodAccess{this}).Fulfill(StartPromiseRes{});
        DonePromise(GpMethodAccess{this}).Fulfill(DonePromiseRes{});
    } catch (const std::exception& e)
    {
        GpOutUtils::S().StdErr(fmt::format("[GpTask::~GpTask]: {}", e.what()));
    } catch (...)
    {
        GpOutUtils::S().StdErr("[GpTask::~GpTask]: unknown exception"_sv);
    }
}

GpUUID  GpTask::TaskIdAsUUID (void) const noexcept
{
    GpUUID::DataT uuid;

    const GpTaskId id = TaskId();

    std::memcpy(std::data(uuid) + 0, "GpTask::", 8);

    GpTaskId::value_type zeros = 0;
    std::memcpy(std::data(uuid) + 8, &zeros, 4);

    std::memcpy(std::data(uuid) + 8 + 4, &id, 4);

    return GpUUID{uuid};
}

GpTask::DoneFutureT::SP GpTask::RequestStop (void)
{
    return GpTaskScheduler::S().RequestStop(*this);
}

void    GpTask::RequestStopAndWait (void)
{
    // Request stop
    GpTask::DoneFutureT::SP doneFutureOptSP = RequestStop();
    GpTask::DoneFutureT&    doneFuture      = doneFutureOptSP.V();

    // Wait for stop
    GpItcFutureUtils::SWait
    (
        doneFuture,
        [](typename DoneFutureT::value_type&&)// OnSuccessFnT
        {
            // NOP
        },
        [](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        }
    );
}

GpTask::WP GpTask::SCurrentTask (void) noexcept
{
    return __GpTask__thread_current_task;
}

GpTaskRunRes::EnumT GpTask::SExecute
(
    GpTask::SP aTaskSP,
    GpMethodAccessGuard<GpTaskScheduler, GpTaskExecutor>
) noexcept
{
    __GpTask__thread_current_task = aTaskSP;

    // Run task (noexcept)
    GpTask& task = aTaskSP.Vn();
    task.iDefferedWakeupFlag.clear(std::memory_order_release);
    const GpTaskRunRes::EnumT runRes = task.Run();

    __GpTask__thread_current_task = nullptr;

    return runRes;
}

u_int_64    GpTask::TypeUID (void) const noexcept
{
    return 0;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
