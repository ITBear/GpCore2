#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskExecutor.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedQueue.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpTaskSchedulerV1;

class GpTaskExecutorV1 final: public GpTaskExecutor
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskExecutorV1)
    CLASS_DD(GpTaskExecutorV1)
    TAG_SET(THREAD_SAFE)

    using ReadyTasksQueueT  = GpItcSharedQueue<GpTask::SP>;
    using DonePromiseT      = GpItcSharedPromise<ssize_t>;

public:
                        GpTaskExecutorV1    (const size_t       aId,
                                             GpTaskSchedulerV1& aTasksScheduler,
                                             ReadyTasksQueueT&  aReadyTasksQueue,
                                             DonePromiseT&&     aDonePromise) noexcept;
    virtual             ~GpTaskExecutorV1   (void) noexcept override final;

    size_t              Id                  (void) const noexcept {return iId;}
    virtual void        Run                 (std::atomic_flag& aStopRequest) noexcept override final;

protected:
    virtual void        OnNotify            (void) noexcept override final;

private:
    const size_t        iId             = 0;
    GpTaskSchedulerV1&  iTasksScheduler;
    ReadyTasksQueueT&   iReadyTasksQueue;
    DonePromiseT        iDonePromise;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
