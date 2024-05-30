#pragma once

#include "../GpTaskScheduler.hpp"
#include "../../GpTask.hpp"
#include "GpTaskExecutorV1.hpp"

#include <GpCore2/GpUtils/Threads/GpThread.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>

#include <bitset>
#include <array>

namespace GPlatform {

class GpTaskSchedulerV1 final: public GpTaskScheduler
{
    friend class GpTaskExecutorV1;

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskSchedulerV1)
    CLASS_DD(GpTaskSchedulerV1)
    TAG_SET(THREAD_SAFE)

    using ExecutorDoneFutureT   = GpItcSharedFuture<ssize_t>;
    using ReadyTasksQueueT      = GpItcSharedQueue<GpTask::SP>;
    using WaitingTasksT         = std::unordered_map<GpTaskId, GpTask::SP>; //TODO: check perfomance and grow of id (test)
    using MarkedAsReadyIdsT     = std::unordered_set<GpTaskId>; //TODO: check perfomance and grow of id (test)

public:
                                    GpTaskSchedulerV1       (StopServiceFnT aStopServiceFn) noexcept;
    virtual                         ~GpTaskSchedulerV1      (void) noexcept override final;

    virtual void                    Start                   (const size_t aExecutorsCount,
                                                             const size_t aTasksMaxCount) override final;
    virtual void                    RequestStopAndJoin      (void) noexcept override final;

    virtual void                    NewToReady              (GpTask::SP aTask) override final;
    virtual void                    NewToWaiting            (GpTask::SP aTask) override final;
    virtual void                    MakeTaskReady           (GpTaskId aTaskId) override final;
    virtual void                    MakeTaskReady           (GpTaskId       aTaskId,
                                                             GpAny          aMessage) override final;
    virtual void                    MakeTasksReadyByGroupId (GpTaskGroupId  aGpTaskGroupId,
                                                             GpAny          aMessage) override final;
    virtual bool                    AddTaskToGroup          (GpTaskId       aTaskGuid,
                                                             GpTaskGroupId  aGpTaskGroupId) override final;
    virtual bool                    RemoveTaskFromGroup     (GpTaskId       aTaskGuid,
                                                             GpTaskGroupId  aGpTaskGroupId) override final;

    virtual bool                    Reschedule              (GpTaskRunRes::EnumT    aRunRes,
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
