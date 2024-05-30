#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpCore2/GpTasks/GpTaskGroupsManager.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>

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

GpTask::~GpTask (void) noexcept
{
    const GpTaskId taskId = TaskId();

    // Remove task data from GpTaskVarStorage
    try
    {
        GpTaskVarStorage::S().RemoveTask(taskId);
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr
        (
            fmt::format
            (
                "[GpTask::~GpTask]: GpTaskVarStorage::RemoveTask exception: {}",
                e.what()
            )
        );
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr
        (
            fmt::format
            (
                "[GpTask::~GpTask]: GpTaskVarStorage::RemoveTask exception: {}",
                e.what()
            )
        );
    } catch (...)
    {
        GpStringUtils::SCerr("[GpTask::~GpTask]: GpTaskVarStorage::RemoveTask unknown exception"_sv);
    }

    // Remove task from GpTaskGroupsManager
    try
    {
        GpTaskGroupsManager::S().RemoveTaskFromAllGroups(taskId);
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr
        (
            fmt::format
            (
                "[GpTask::~GpTask]: GpTaskGroupsManager::RemoveTaskFromAllGroups exception: {}",
                e.what()
            )
        );
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr
        (
            fmt::format
            (
                "[GpTask::~GpTask]: GpTaskGroupsManager::RemoveTaskFromAllGroups exception: {}",
                e.what()
            )
        );
    } catch (...)
    {
        GpStringUtils::SCerr("[GpTask::~GpTask]: GpTaskGroupsManager::RemoveTaskFromAllGroups unknown exception"_sv);
    }

    // Fullfill promises
    StartPromise().Fulfill(GpAny{});
    DonePromise().Fulfill(GpAny{});

    //
    GpTask::sAllTasksDictionary.Erase(iId);
}

GpTask::DoneFutureT::SP GpTask::RequestStop (void)
{
    return GpTaskScheduler::S().RequestStop(*this);
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
    std::string     aKey,
    GpAny           aValue
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
    __GpTask__thread_current_task = *this;
    const GpTaskRunRes::EnumT runRes = Run();
    __GpTask__thread_current_task.reset();

    return runRes;
}

}// namespace GPlatform
