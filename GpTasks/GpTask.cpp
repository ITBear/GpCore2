#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpCore2/GpTasks/GpTaskGroupsManager.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedFutureUtils.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

thread_local GpTask::C::Opt::Ref    __GpTask__thread_current_task;
std::atomic<GpTaskId::value_type>   GpTask::sIdCounter = {1};
GpTask::AllTasksDictionaryT         GpTask::sAllTasksDictionary;

GpTask::GpTask
(
    std::string             aName,
    const GpTaskMode::EnumT aTaskMode
) noexcept:
iName{std::move(aName)},
iId  {SNextId()},
iMode{aTaskMode}
{
    GpTask::sAllTasksDictionary.SetOrUpdate(iId, this);
}

GpTask::GpTask (const GpTaskMode::EnumT aTaskMode) noexcept:
iId  {SNextId()},
iMode{aTaskMode}
{
    GpTask::sAllTasksDictionary.SetOrUpdate(iId, this);
}

GpTask::GpTask
(
    std::string             aName,
    const GpTaskMode::EnumT aTaskMode,
    const GpTaskId          aId
) noexcept:
iName{std::move(aName)},
iId  {aId},
iMode{aTaskMode}
{
    GpTask::sAllTasksDictionary.SetOrUpdate(iId, this);
}

GpTask::GpTask
(
    const GpTaskMode::EnumT aTaskMode,
    const GpTaskId          aId
) noexcept:
iId  {aId},
iMode{aTaskMode}
{
    GpTask::sAllTasksDictionary.SetOrUpdate(iId, this);
}

GpTask::~GpTask (void) noexcept
{
    try
    {
        const GpTaskId taskId = TaskId();

        // Remove task data from GpTaskVarStorage
        GpTaskVarStorage::S().RemoveTask(taskId);

        // Remove task from GpTaskGroupsManager
        GpTaskGroupsManager::S().RemoveTaskFromAllGroups(taskId);


        // Fulfill promises
        StartPromise().Fulfill(StartPromiseRes{});
        DonePromise().Fulfill(DonePromiseRes{});

        //
        GpTask::sAllTasksDictionary.Erase(iId);
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr
        (
            fmt::format
            (
                "[GpTask::~GpTask]: {}",
                e.what()
            )
        );
    } catch (...)
    {
        GpStringUtils::SCerr("[GpTask::~GpTask]: unknown exception"_sv);
    }   
}

GpUUID  GpTask::TaskIdAsUUID (void) const noexcept
{
    GpUUID::DataT uuid;

    const GpTaskId id = TaskId();

    std::memcpy(std::data(uuid) + 0, "GpTask::", 8);
    std::memcpy(std::data(uuid) + 8, &id, 8);

    return GpUUID(uuid);
}

GpTask::DoneFutureT::SP GpTask::RequestTaskStop (void)
{
    return GpTaskScheduler::S().RequestStop(*this);
}

void    GpTask::RequestAndWaitForStop (void)
{
    // Request stop
    GpTask::DoneFutureT::SP doneFutureSP    = RequestTaskStop();
    GpTask::DoneFutureT&    doneFuture      = doneFutureSP.V();

    // Wait for stop
    if (IsStartRequested() == false) [[unlikely]]
    {
        return;
    }

    GpItcSharedFutureUtils::SWaitForInf
    (
        doneFuture,
        [&](typename DoneFutureT::value_type&)// OnSuccessFnT
        {
            // NOP
        },
        [&](const GpException& aEx)// OnExceptionFnT
        {
            throw aEx;
        },
        100.0_si_ms
    );
}

GpTask::C::Opt::Ref GpTask::SCurrentTask (void) noexcept
{
    return __GpTask__thread_current_task;
}

std::optional<GpTask*>  GpTask::STaskById (GpTaskId aTaskId) noexcept
{
    return GpTask::sAllTasksDictionary.GetOpt(aTaskId);
}

void    GpTask::PushMessage (GpAny aMessage)
{
    std::ignore = iMessagesQueue.Push(std::move(aMessage));
}

GpAny::C::Opt::Val  GpTask::PopMessage (void)
{
    return iMessagesQueue.Pop();
}

void    GpTask::SetVar
(
    std::string aKey,
    GpAny       aValue
)
{
    GpTaskVarStorage::S().SetVar
    (
        TaskId(),
        std::move(aKey),
        std::move(aValue)
    );
}

GpTaskVarStorage::AnyOptT   GpTask::GetVarCopy (std::string_view aKey) const
{
    return GpTaskVarStorage::S().GetVarCopy(TaskId(), aKey);
}

GpTaskVarStorage::AnyOptCRefT   GpTask::GetVarRef (std::string_view aKey) const
{
    return GpTaskVarStorage::S().GetVarRef(TaskId(), aKey);
}

GpTaskRunRes::EnumT GpTask::Execute (GpMethodAccessGuard<GpTaskScheduler, GpTaskExecutor>) noexcept
{
    // Update task for current thread
    __GpTask__thread_current_task = *this;

    // Update task state (GpTaskState::RUNING)
    iState.store(GpTaskState::RUNING, std::memory_order_release);

    // Run task
    const GpTaskRunRes::EnumT runRes = Run();

    // Update task state
    iState.store
    (
        runRes != GpTaskRunRes::DONE ? GpTaskState::WAITING : GpTaskState::DONE,
        std::memory_order_release
    );

    __GpTask__thread_current_task.reset();

    return runRes;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
