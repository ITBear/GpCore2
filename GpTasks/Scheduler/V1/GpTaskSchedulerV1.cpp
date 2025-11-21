#include <GpCore2/GpTasks/Scheduler/V1/GpTaskSchedulerV1.hpp>
#include <GpCore2/GpTasks/Scheduler/V1/GpTaskExecutorV1.hpp>
#include <GpCore2/GpUtils/Exceptions/GpExceptionUtils.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>
#include <GpCore2/GpTasks/ITC/GpItcFutureUtils.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

constexpr u_int_32 COUNTER_STOP_BIT() {return u_int_32(1) << 31;}

GpTaskSchedulerV1::GpTaskSchedulerV1
(
    const size_t aExecutorsCount,
    const size_t aTasksMaxCount
) noexcept:
GpTaskScheduler{aExecutorsCount, aTasksMaxCount},
iOpsCounter{0}
{
}

GpTaskSchedulerV1::~GpTaskSchedulerV1 (void) noexcept
{
}

void    GpTaskSchedulerV1::SpawnReady (GpTask::SP aTaskSP)
{
    VERIFY
    (
        OpBegin(),
        "Task scheduler stopped"
    );

    GpDefer opEnd = [&]()
    {
        OpEnd();
    };

    aTaskSP.Vn().SetSelfWP(aTaskSP, GpMethodAccess{this});

    MoveToReady(std::move(aTaskSP));
}

void    GpTaskSchedulerV1::SpawnWaiting (GpTask::SP aTaskSP)
{
    VERIFY
    (
        OpBegin(),
        "Task scheduler stopped"
    );

    GpDefer opEnd = [&]()
    {
        OpEnd();
    };

    aTaskSP.Vn().SetSelfWP(aTaskSP, GpMethodAccess{this});

    MoveToWaiting(std::move(aTaskSP));
}

void    GpTaskSchedulerV1::Wakeup (GpTask& aTask)
{
    VERIFY
    (
        OpBegin(),
        "Task scheduler stopped"
    );

    GpDefer opEnd = [&]()
    {
        OpEnd();
    };

    GpTask::SP      waitingTaskSP   = ExtractFromWaiting(aTask.TaskId());
    RedyTaskQueueT& redyTaskQueue   = iReadyTaskQueueSP.Vn();

    if (waitingTaskSP.IsNotNULL()) [[likely]]
    {
        // Move task to ready queue
        if (redyTaskQueue.Push(std::move(waitingTaskSP)) > 0) [[unlikely]]
        {
            THROW("Ready tasks queue is full"_sv);
        }
    } else
    {
        aTask.UpDefferedWakeupFlag(GpMethodAccess{this});
    }
}

bool    GpTaskSchedulerV1::Reschedule
(
    const GpTaskRunRes::EnumT   aRunRes,
    GpTask::SP                  aTaskSP,
    GpMethodAccessGuard<GpTaskExecutor>
)
{
    if (OpBegin() == false)
    {
        return false;
    }

    GpDefer opEnd = [&]()
    {
        OpEnd();
    };

    switch (aRunRes)
    {
        case GpTaskRunRes::READY_TO_RUN:
        {
            MoveToReady(std::move(aTaskSP));
        } break;
        case GpTaskRunRes::WAIT:
        {
            if (aTaskSP.Vn().IsDefferedWakeup(GpMethodAccess{this}))
            {
                MoveToReady(std::move(aTaskSP));
            } else
            {
                MoveToWaiting(std::move(aTaskSP));
            }
        } break;
        case GpTaskRunRes::DONE:
        {
            aTaskSP.Clear();
        } break;
    }

    return true;
}

void    GpTaskSchedulerV1::Start (void)
{
    GpUniqueLock uniqueLock{SpinLockRW()};

    //------------------ Check -------------------
    VERIFY
    (
        iExecutorThreads.empty(),
        "Executor tasks already created"_sv
    );

    //------------------ Create executors -------------------
    const size_t executorsCount = ExecutorsCount();
    iExecutorThreads.reserve(executorsCount);
    iReadyTaskQueueSP = MakeSP<RedyTaskQueueT>(TasksMaxCount());

    for (size_t id = 0; id < executorsCount; id++)
    {
        // Executor promise
        GpTaskExecutorV1::DonePromiseT executorDonePromise;
        iExecutorDoneFutures.emplace_back(executorDonePromise.Future());

        // Create thread for executor
        GpThread::UP    threadUP    = std::make_unique<GpThread>(iExecutorStopFlag, "Executor: "_sv + id);
        GpThread&       thread      = *iExecutorThreads.emplace_back(std::move(threadUP));

        // Run executor
        thread.Run
        (
            std::make_unique<GpTaskExecutorV1>
            (
                id,
                *this,
                iReadyTaskQueueSP,
                std::move(executorDonePromise)
            )
        );
    }
}

void    GpTaskSchedulerV1::RequestStopAndJoin (void)
{
    std::vector<GpException> exceptions;

    // Set Stop Bit
    DisableAndStopOps();

    // Send stop request to all executors
    RedyTaskQueueT& redyTaskQueue = iReadyTaskQueueSP.Vn();

    iExecutorStopFlag.UpFlagAndNotifyAll();
    redyTaskQueue.Interrupt();

    // Join executor threads
    for (GpThread::UP& executorThread: iExecutorThreads)
    {
        executorThread->Join();
    }

    iExecutorThreads.clear();

    // Check excutor threads done result
    while (!iExecutorDoneFutures.empty())
    {
        for (auto iter = std::begin(iExecutorDoneFutures); iter != std::end(iExecutorDoneFutures); )
        {
            const bool isReady = GpItcFutureUtils::STryCheck
            (
                iter->V(),
                nullptr,
                [&exceptions](const GpException& aException)
                {
                    exceptions.emplace_back(aException);
                }
            );

            if (isReady)
            {
                iter = iExecutorDoneFutures.erase(iter);
            } else
            {
                iter++;
            }
        }
    }

    // UpStopRequestFlag for all ready tasks
    while (!redyTaskQueue.Empty())
    {
        auto                popRes  = redyTaskQueue.Pop();
        GpTask::C::Opts::SP taskOpt = std::move(popRes.iValue);

        if (taskOpt.has_value())
        {
            GpTask::SP& taskSP = taskOpt.value();

            taskSP.Vn().UpStopRequestFlag(GpMethodAccess{this});
            GpTask::SExecute
            (
                std::move(taskSP),
                GpMethodAccess{this}
            );
        }
    }

    redyTaskQueue.Clear();

    // UpStopRequestFlag for all waiting tasks
    {
        GpUniqueLock uniqueLock{SpinLockRW()};

        for (auto&[aTaskId, taskSP]: iWaitingTasks)
        {
            taskSP.Vn().UpStopRequestFlag(GpMethodAccess{this});
            GpTask::SExecute
            (
                std::move(taskSP),
                GpMethodAccess{this}
            );
        }

        iWaitingTasks.clear();
    }

    // Logout exceptions
    for (const GpException& e: exceptions)
    {
        GpOutUtils::S().Err("[GpTaskSchedulerV1::RequestStopAndJoin]: exception: "_sv + e.what());
    }
}

GpTask::SP  GpTaskSchedulerV1::ExtractFromWaiting (const GpTaskId aTaskId)
{
    GpUniqueLock uniqueLockP{SpinLockRW()};

    // Try to find task
    auto iter = iWaitingTasks.find(aTaskId.Value());

    if (iter == std::end(iWaitingTasks)) [[unlikely]]
    {
        return nullptr;
    }

    return iWaitingTasks.extract(iter).second;
}

bool    GpTaskSchedulerV1::OpBegin (void) noexcept
{
    const u_int_32 oldCounter = iOpsCounter.fetch_add(1, std::memory_order_acq_rel);

    if (oldCounter & COUNTER_STOP_BIT())
    {
        iOpsCounter.fetch_sub(1, std::memory_order_acq_rel);
        return false;
    }

    return true;
}

void    GpTaskSchedulerV1::OpEnd (void) noexcept
{
    iOpsCounter.fetch_sub(1, std::memory_order_acq_rel);
}

void    GpTaskSchedulerV1::DisableAndStopOps (void) noexcept
{
    // UP Stop bit
    iOpsCounter.fetch_or(COUNTER_STOP_BIT(), std::memory_order_acq_rel);

    // Wait for counter == 0
    constexpr const u_int_32 counterMask = ~COUNTER_STOP_BIT();

    while ((iOpsCounter.load(std::memory_order_acquire) & counterMask) > 0)
    {
        GP_ASM_SPIN_PAUSE();
    }
}

void    GpTaskSchedulerV1::MoveToReady (GpTask::SP aTaskSP)
{
    // Move task to the "ready" queue
    RedyTaskQueueT& redyTaskQueue = iReadyTaskQueueSP.Vn();
    if (redyTaskQueue.Push(std::move(aTaskSP)) > 0) [[unlikely]]
    {
        THROW("Ready tasks queue is full"_sv);
    }
}

void    GpTaskSchedulerV1::MoveToWaiting (GpTask::SP aTaskSP)
{
    const auto taskId = aTaskSP.Vn().TaskId().Value();

    GpUniqueLock uniqueLock{SpinLockRW()};
    iWaitingTasks.emplace(taskId, std::move(aTaskSP));
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
