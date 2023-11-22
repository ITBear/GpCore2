#pragma once

#include "../../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../../GpUtils/Threads/GpThread.hpp"
#include "../../../GpUtils/SyncPrimitives/GpMutex.hpp"

#include "../GpTaskScheduler.hpp"
#include "../../GpTask.hpp"
#include "GpTaskExecutorV1.hpp"

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
    using WaitingTasksT         = std::unordered_map<GpTaskId, GpTask::SP>;

public:
                                        GpTaskSchedulerV1       (void) noexcept;
    virtual                             ~GpTaskSchedulerV1      (void) noexcept override final;

    virtual void                        Start                   (const size_t aExecutorsCount,
                                                                 const size_t aTasksMaxCount) override final;
    virtual void                        RequestStopAndJoin      (void) noexcept override final;

    virtual void                        NewToReady              (GpTask::SP aTask) override final;
    virtual void                        NewToWaiting            (GpTask::SP aTask) override final;
    virtual void                        MakeTaskReady           (const GpTaskId aTaskId) override final;
    virtual void                        MakeTaskReady           (const GpTaskId aTaskId,
                                                                 GpAny          aPayload) override final;

    virtual bool                        Reschedule              (const GpTaskRunRes::EnumT  aRunRes,
                                                                 GpTask::SP&&               aTask) noexcept override final;

private:
    void                                _MakeTaskReady          (const GpTaskId aTaskId) REQUIRES(iMutex);
    void                                _MoveToReady            (GpTask::SP     aTask)   REQUIRES(iMutex);
    void                                _MoveToWaiting          (GpTask::SP     aTask)   REQUIRES(iMutex);

private:
    ExecutorDoneFutureT::C::Vec::SP     iExecutorDoneFutures;
    ReadyTasksQueueT                    iReadyTasks;

    mutable GpMutex                     iMutex;
    GpThread::C::Vec::SP                iExecutorThreads        GUARDED_BY(iMutex);
    WaitingTasksT                       iWaitingTasks           GUARDED_BY(iMutex);
    std::unordered_set<GpTaskId>        iMarkedAsReadyIds       GUARDED_BY(iMutex);
    bool                                iIsRequestStopAndJoin   GUARDED_BY(iMutex) = false;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
