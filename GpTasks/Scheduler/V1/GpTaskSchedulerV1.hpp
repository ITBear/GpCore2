#pragma once

#include "../../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../ITC/GpItcProducer.hpp"
#include "../../ITC/GpItcSharedFuture.hpp"
#include "../../../GpUtils/Threads/GpThread.hpp"
#include "../../../GpUtils/SyncPrimitives/GpSpinlock.hpp"

#include "../GpTaskScheduler.hpp"
#include "../../GpTask.hpp"

namespace GPlatform {

class GpTaskSchedulerV1 final: public GpTaskScheduler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskSchedulerV1)
    CLASS_DD(GpTaskSchedulerV1)
    TAG_SET(THREAD_SAFE)

    using TaskProducerT         = GpItcProducer<GpTask::SP>;
    using TaskProducerConsumerT = GpItcProducerConsumer<GpTask::SP>;
    using ExecutorDoneFutureT   = GpItcSharedFuture<ssize_t>;
    using WaitingTasksT         = std::unordered_map<GpTask::IdT, GpTask::SP>;

public:
                                        GpTaskSchedulerV1   (void) noexcept;
    virtual                             ~GpTaskSchedulerV1  (void) noexcept override final;

    virtual void                        Start               (const size_t aExecutorsCount,
                                                             const size_t aTasksMaxCount) override final;
    virtual void                        RequestStopAndJoin  (void) noexcept override final;

    virtual void                        NewToReady          (GpTask::SP             aTask,
                                                             const milliseconds_t   aWaitTimeout) override final;
    virtual void                        NewToWaiting        (GpTask::SP aTask) override final;
    virtual void                        MakeTaskReady       (const GpTask::IdT aTaskId) override final;
    virtual void                        MakeTaskReady       (const GpTask::IdT aTaskId,
                                                             GpAny              aPayload) override final;

    virtual bool                        Reschedule          (const GpTaskRunRes::EnumT  aRunRes,
                                                             GpTask::SP&&               aTask) noexcept override final;

private:
    void                                _MakeTaskReady      (const GpTask::IdT      aTaskId);
    void                                _MoveToReady        (GpTask::SP             aTask,
                                                             const milliseconds_t   aWaitTimeout);
    void                                _MoveToWaiting      (GpTask::SP             aTask);

private:
    mutable GpSpinlock                  iLock;
    GpThread::C::Vec::SP                iExecutorThreads;
    ExecutorDoneFutureT::C::Vec::SP     iExecutorDoneFutures;
    TaskProducerT::SP                   iTasksProducer;
    WaitingTasksT                       iWaitingTasks;
    std::unordered_set<GpTask::IdT>     iMarkedAsReadyIds;
    bool                                iIsRequestStopAndJoin   = false;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
