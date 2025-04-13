#pragma once

#include <GpCore2/GpUtils/Threads/GpThread.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpCore2/GpTasks/Scheduler/V1/GpTaskExecutorV1.hpp>
#include <GpCore2/Config/IncludeExt/unordered_dense.hpp>

#include <bitset>
#include <array>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpTaskSchedulerV1 final: public GpTaskScheduler
{
    friend class GpTaskExecutorV1;

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskSchedulerV1)
    CLASS_DD(GpTaskSchedulerV1)
    TAG_SET(THREAD_SAFE)

    using ExecutorDoneFutureT   = GpItcFuture<ssize_t>;
    using ReadyTasksQueueT      = GpItcQueue<GpTask::SP>;
    using WaitingTasksT         = ankerl::unordered_dense::map<GpTaskId::value_type, GpTask::SP>;
    using MarkedAsReadyIdsT     = ankerl::unordered_dense::set<GpTaskId::value_type>;

public:
                                    GpTaskSchedulerV1       (StopServiceFnT aStopServiceFn) noexcept;
    virtual                         ~GpTaskSchedulerV1      (void) noexcept override final;

    virtual void                    Start                   (size_t aExecutorsCount,
                                                             size_t aTasksMaxCount) override final;
    virtual void                    RequestStopAndJoin      (void) noexcept override final;

    [[nodiscard]] virtual bool      NewToReady              (GpTask::SP aTask) override final;
    [[nodiscard]] virtual bool      NewToWaiting            (GpTask::SP aTask) override final;
    [[nodiscard]] virtual bool      MakeTaskReady           (GpTaskId aTaskId) override final;
    [[nodiscard]] virtual bool      MakeTaskReady           (GpTaskId       aTaskId,
                                                             GpAny          aMessage) override final;

    [[nodiscard]] virtual bool      Reschedule              (GpTaskRunRes::EnumT    aRunRes,
                                                             GpTask::SP&&           aTask) noexcept override final;

private:
    void                            _MakeTaskReady          (GpTaskId   aTaskId,
                                                             GpAny      aMessage) REQUIRES(iMutex);
    void                            _MoveToReady            (GpTask::SP aTask,
                                                             GpAny      aMessage) REQUIRES(iMutex);
    void                            _MoveToWaiting          (GpTask::SP aTask)   REQUIRES(iMutex);

private:
    ExecutorDoneFutureT::C::Vec::SP iExecutorDoneFutures;
    ReadyTasksQueueT                iReadyTasks;

    mutable GpMutex                 iMutex;
    GpThread::C::Vec::SP            iExecutorThreads        GUARDED_BY(iMutex);
    WaitingTasksT                   iWaitingTasks           GUARDED_BY(iMutex);
    MarkedAsReadyIdsT               iMarkedAsReadyIds       GUARDED_BY(iMutex);
    bool                            iIsRequestStopAndJoin   GUARDED_BY(iMutex) = false;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
