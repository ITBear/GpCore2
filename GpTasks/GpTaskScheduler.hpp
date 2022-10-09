#pragma once

#include "GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTask.hpp"
#include "GpTaskExecutorsPool.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskScheduler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskScheduler)
    CLASS_DD(GpTaskScheduler)

    using ExecutorT             = GpTaskExecutor;
    using TaskSchedulerOptRefT  = std::optional<GpTaskScheduler::WP>;

public:
                                    GpTaskScheduler     (void) noexcept;
                                    ~GpTaskScheduler    (void) noexcept;

    static GpTaskScheduler&         S                   (void) noexcept {return sTaskScheduler;}

    void                            Start               (const size_t aExecutorsCount);
    void                            RequestStop         (void) noexcept;
    void                            Join                (void) noexcept;

    GpUUID                          NewToReady          (GpTask::SP aTask);
    [[nodiscard]] GpItcFuture::SP   NewToReadyDepend    (GpTask::SP aTask);
    GpUUID                          NewToWaiting        (GpTask::SP aTask);
    [[nodiscard]] GpItcFuture::SP   NewToWaitingDepend  (GpTask::SP aTask);
    void                            MakeTaskReady       (const GpUUID& aTaskGuid);
    //void                          RequestTaskStop     (const GpUUID& aTaskGuid);

    GpTask::SP                      _Reshedule          (GpTask::SP         aLastTaskSP,
                                                         const GpTaskDoRes  aLastTaskExecRes) noexcept;
    static void                     _SDestroyTask       (GpTask& aTask);

private:
    void                            Clear               (void) noexcept;

private:
    mutable GpSpinlock              iLock;
    GpTask::C::Deque::SP            iReadyTasks;
    GpTask::C::UMapUuid::SP         iWaitingTasks;
    GpTaskExecutorsPool             iExecutorsPool;
    bool                            iIsStarted          = false;
    bool                            iIsStopRequested    = false;

    static GpTaskScheduler          sTaskScheduler;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
