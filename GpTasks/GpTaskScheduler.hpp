#pragma once

#include "GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTask.hpp"
#include "GpTaskExecutorsPool.hpp"

namespace GPlatform {

class GpTaskExecutor;

class GP_TASKS_API GpTaskScheduler
{
    friend class GpTaskExecutor;

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskScheduler)
    CLASS_DD(GpTaskScheduler)

    using ExecutorT             = GpTaskExecutor;
    using TaskSchedulerOptRefT  = std::optional<GpTaskScheduler::WP>;

public:
                                    GpTaskScheduler         (void) noexcept;
                                    ~GpTaskScheduler        (void) noexcept;

    static GpTaskScheduler&         S                       (void) noexcept {return sTaskScheduler;}

    size_t                          ExecutorsCount          (void) const noexcept {return iExecutorsCount;}

    void                            Start                   (const size_t aExecutorsCount);
    void                            RequestStop             (void) noexcept;
    void                            Join                    (void) noexcept;

    void                            NewToReady              (GpTask::SP aTask);
    [[nodiscard]] GpItcFuture::SP   NewToReadyDepend        (GpTask::SP aTask);
    void                            NewToWaiting            (GpTask::SP aTask);
    [[nodiscard]] GpItcFuture::SP   NewToWaitingDepend      (GpTask::SP aTask);
    void                            MakeTaskReady           (const GpUUID& aTaskGuid);

private:
    GpTask::SP                      _Reshedule              (GpTask::SP         aLastTaskSP,
                                                             const GpTaskDoRes  aLastTaskExecRes) noexcept;
    void                            _DestroyTaskSP          (GpTask::SP&& aTask);
    void                            _DestroyTaskRef         (GpTask::SP&& aTask);
    GpItcFuture::SP                 _NewToReady             (GpTask::SP aTask,
                                                             const bool aIsDependent);
    GpItcFuture::SP                 _NewToWaiting           (GpTask::SP aTask,
                                                             const bool aIsDependent);
    inline void                     _AddToRuningTasks       (GpTask::SP aTask);
    inline void                     _RemoveFromRuningTasks  (const GpUUID& aTaskGuid);
    inline std::optional<GpTask*>   _FindInRunningTasks     (const GpUUID& aTaskGuid);

    void                            Clear                   (void) noexcept;

private:
    mutable GpSpinlock              iLock;
    size_t                          iExecutorsCount     = 0;
    GpTask::C::Deque::SP            iReadyTasks;
    GpTask::C::UMapUuid::SP         iWaitingTasks;
    std::vector<GpTask::SP>         iRunningTasks;
    GpTaskExecutorsPool             iExecutorsPool;
    bool                            iIsStarted          = false;
    bool                            iIsStopRequested    = false;

    GpSpinlock                      iReadyToDestroyLock;
    GpTask::C::UMapUuid::SP         iReadyToDestroyTasks;

    static GpTaskScheduler          sTaskScheduler;
};

void    GpTaskScheduler::_AddToRuningTasks (GpTask::SP aTask)
{
    const size_t runningTasksCount = iRunningTasks.size();

    for (size_t id = 0; id < runningTasksCount; id++)
    {
        GpTask::SP& taskSP = iRunningTasks[id];

        if (taskSP.IsNULL())
        {
            taskSP = std::move(aTask);
            return;
        }
    }

    iRunningTasks.emplace_back(std::move(aTask));
}

void    GpTaskScheduler::_RemoveFromRuningTasks (const GpUUID& aTaskGuid)
{
    const size_t runningTasksCount = iRunningTasks.size();

    for (size_t id = 0; id < runningTasksCount; id++)
    {
        GpTask::SP& taskSP = iRunningTasks[id];

        if (taskSP.IsNULL())
        {
            continue;
        }

        if (taskSP.Vn().Guid() == aTaskGuid)
        {
            taskSP.Clear();
            return;
        }
    }
}

std::optional<GpTask*>  GpTaskScheduler::_FindInRunningTasks (const GpUUID& aTaskGuid)
{
    const size_t runningTasksCount = iRunningTasks.size();

    for (size_t id = 0; id < runningTasksCount; id++)
    {
        GpTask* task = iRunningTasks[id].Pn();

        if (task == nullptr)
        {
            continue;
        }

        if (aTaskGuid == task->Guid())
        {
            return task;
        }
    }

    return std::nullopt;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
