#pragma once

#include <GpCore2/GpUtils/Threads/GpThread.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/Scheduler/V1/GpTaskExecutorV1.hpp>
#include <GpCore2/Config/IncludeExt/unordered_dense.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_set.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpTaskSchedulerV1 final: public GpTaskScheduler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskSchedulerV1)
    CLASS_DD(GpTaskSchedulerV1)
    TAG_SET(THREAD_SAFE)

    using ExecutorDoneFutureT   = GpItcFuture<ssize_t>;
    using WaitingTasksT         = ankerl::unordered_dense::map<GpTaskId::value_type, GpTask::SP>;
    using RedyTaskQueueT        = GpTaskExecutorV1::TaskQueueT;

public:
                    GpTaskSchedulerV1   (size_t aExecutorsCount,
                                         size_t aTasksMaxCount) noexcept;
    virtual         ~GpTaskSchedulerV1  (void) noexcept override final;

    // Task wait/ready
    virtual void    SpawnReady          (GpTask::SP aTaskSP) override final;
    virtual void    SpawnWaiting        (GpTask::SP aTaskSP) override final;
    virtual void    Wakeup              (GpTask& aTask) override final;

    // Scheduler
    virtual bool    Reschedule          (GpTaskRunRes::EnumT    aRunRes,
                                         GpTask::SP             aTaskSP,
                                         GpMethodAccessGuard<GpTaskExecutor>) override final;

protected:
    virtual void    Start               (void) override final;
    virtual void    RequestStopAndJoin  (void) override final;

private:
    GpTask::SP      ExtractFromWaiting  (GpTaskId aTaskId);

    bool            OpBegin             (void) noexcept;
    void            OpEnd               (void) noexcept;
    void            DisableAndStopOps   (void) noexcept;

    void            MoveToReady         (GpTask::SP aTaskSP);
    void            MoveToWaiting       (GpTask::SP aTaskSP);

private:
    // Executors
    GpThread::C::Vec::UP            iExecutorThreads;
    ExecutorDoneFutureT::C::Vec::SP iExecutorDoneFutures;
    GpConditionVarFlag              iExecutorStopFlag;

    WaitingTasksT                   iWaitingTasks GUARDED_BY(SpinLockRW());
    RedyTaskQueueT::SP              iReadyTaskQueueSP;
    std::atomic<u_int_32>           iOpsCounter;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
