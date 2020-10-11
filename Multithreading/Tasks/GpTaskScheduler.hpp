#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskExecutorsPool.hpp"
#include "../SyncPrimitives/GpSyncPrimitives.hpp"

namespace GPlatform {

class GpTaskSchedulerAccessor;

class GPCORE_API GpTaskScheduler
{
    friend class GpTaskSchedulerAccessor;

public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTaskScheduler)
    CLASS_DECLARE_DEFAULTS(GpTaskScheduler)

    using ExecutorT             = GpTaskExecutor;
    using TaskSchedulerOptRefT  = std::optional<std::reference_wrapper<GpTaskScheduler>>;

public:
                                GpTaskScheduler         (void) noexcept;
                                ~GpTaskScheduler        (void) noexcept;

    void                        Start                   (const count_t aExecutorsCount);
    void                        RequestStop             (void) noexcept;
    void                        Join                    (void) noexcept;

    GpTask::SP                  Reshedule               (GpTask::SP         aLastTask,
                                                         const GpTask::Res  aLastTaskExecRes) noexcept;

    void                        AddTaskToReady          (GpTask::SP aTask);
    void                        AddTaskToWaiting        (GpTask::SP aTask);

    static TaskSchedulerOptRefT SCurrentScheduler       (void) noexcept;
    static void                 SSetCurrentScheduler    (TaskSchedulerOptRefT aValue) noexcept;

private:
    void                        MoveToReady             (GpTask::SP aTask);

private:
    mutable GpSpinlock          iReadyLock;
    GpTask::C::Queue::SP        iReadyTasks;

    mutable GpSpinlock          iWaitingLock;
    GpTask::C::Set::SP          iWaitingTasks;

    GpTaskExecutorsPool     iExecutorsPool;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
