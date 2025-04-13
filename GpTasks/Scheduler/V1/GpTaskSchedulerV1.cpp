#include <GpCore2/GpTasks/Scheduler/V1/GpTaskSchedulerV1.hpp>
#include <GpCore2/GpTasks/Scheduler/V1/GpTaskExecutorV1.hpp>
#include <GpCore2/GpTasks/GpTaskGroupsManager_.hpp>
#include <GpCore2/GpUtils/Exceptions/GpExceptionUtils.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>
#include <GpCore2/GpTasks/ITC/GpItcFutureUtils.hpp>

#include <tuple>
#include <utility>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskSchedulerV1::GpTaskSchedulerV1 (StopServiceFnT aStopServiceFn) noexcept:
GpTaskScheduler{aStopServiceFn}
{
}

GpTaskSchedulerV1::~GpTaskSchedulerV1 (void) noexcept
{
}

void    GpTaskSchedulerV1::Start
(
    const size_t aExecutorsCount,
    const size_t aTasksMaxCount
)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    GpTaskScheduler::Start
    (
        aExecutorsCount,
        aTasksMaxCount
    );

    //------------------ Check -------------------
    VERIFY
    (
        aExecutorsCount > 0,
        "Executors count must be > 0"_sv
    );

    VERIFY
    (
        aTasksMaxCount > 0,
        "Tasks max count must be > 0"_sv
    );

    VERIFY
    (
        iExecutorThreads.empty(),
        "Executor tasks already created"_sv
    );

    //------------------ Create executors -------------------
    iExecutorThreads.reserve(aExecutorsCount);

    for (size_t id = 0; id < aExecutorsCount; id++)
    {
        GpTaskExecutorV1::DonePromiseT executorDonePromise;

        iExecutorDoneFutures.emplace_back(executorDonePromise.Future());

        GpThread&               executorThread  = iExecutorThreads.emplace_back(MakeSP<GpThread>("Task exec: "_sv + id)).V();
        GpTaskExecutorV1::SP    executorSP      = MakeSP<GpTaskExecutorV1>
        (
            id,
            *this,
            iReadyTasks,
            std::move(executorDonePromise)
        );

        executorThread.Run(executorSP);
    }
}

void    GpTaskSchedulerV1::RequestStopAndJoin (void) noexcept
{
    try
    {
        // Prepare executors for stop
        {
            // Send stop request to all executors
            GpThread::C::Vec::SP executorThreads;
            {
                GpUniqueLock<GpMutex> uniqueLock{iMutex};

                executorThreads         = iExecutorThreads;
                iIsRequestStopAndJoin   = true;
            }

            for (GpThread::SP& executorThread: executorThreads)
            {
                executorThread->RequestStop();
            }
        }

        // Join executor threads
        {
            GpThread::C::Vec::SP executorThreads;
            {
                GpUniqueLock<GpMutex> uniqueLock{iMutex};

                executorThreads = iExecutorThreads;
            }

            for (GpThread::SP& executorThread: executorThreads)
            {
                executorThread->Join();
            }
        }

        // Clear executor threads
        {
            GpUniqueLock<GpMutex> uniqueLock{iMutex};

            iExecutorThreads.clear();
        }

        // Check excutor threads done result
        {
            while (!iExecutorDoneFutures.empty())
            {
                for (auto iter = std::begin(iExecutorDoneFutures); iter != std::end(iExecutorDoneFutures); )
                {
                    const bool isReady = GpItcFutureUtils::STryCheck
                    (
                        iter->V(),
                        [](ssize_t& /*aExecutorId*/)
                        {
                            // NOP
                        },
                        [](const GpException& aException)
                        {
                            THROW
                            (
                                fmt::format
                                (
                                    "[GpTaskSchedulerV1::WaitForRunners]: done with error: {}",
                                    aException.what()
                                ),
                                aException.SourceLocation()
                            );
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
        }

        // UpStopRequestFlag for all ready tasks
        {
            while (!iReadyTasks.Empty())
            {
                GpTask::C::Opts::SP taskOpt = iReadyTasks.WaitAndPop(0.0_si_s).value();

                if (taskOpt.has_value())
                {
                    GpTask& task = taskOpt.value().V();

                    task.UpStopRequestFlag(GpMethodAccess{this});
                    task.Execute(GpMethodAccess{this});
                }
            }

            iReadyTasks.Clear();
        }

        // UpStopRequestFlag for all waiting tasks
        {
            WaitingTasksT waitingTasks;
            {
                GpUniqueLock<GpMutex> uniqueLock{iMutex};

                waitingTasks = std::move(iWaitingTasks);
                iWaitingTasks.clear();
            }

            for (auto&[taskGuid, taskSP]: waitingTasks)
            {
                taskSP->UpStopRequestFlag(GpMethodAccess{this});
                taskSP->Execute(GpMethodAccess{this});
                taskSP.Clear();
            }
        }
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr("[GpTaskSchedulerV1::RequestStopAndJoin]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr("[GpTaskSchedulerV1::RequestStopAndJoin]: exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr("[GpTaskSchedulerV1::RequestStopAndJoin]: unknown exception"_sv);
    }
}

bool    GpTaskSchedulerV1::NewToReady (GpTask::SP aTask)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    if (iIsRequestStopAndJoin)
    {
        return false;
    }

    _MoveToReady(std::move(aTask), {});

    return true;
}

bool    GpTaskSchedulerV1::NewToWaiting (GpTask::SP aTask)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    if (iIsRequestStopAndJoin)
    {
        return false;
    }

    _MoveToWaiting(std::move(aTask));
    return true;
}

bool    GpTaskSchedulerV1::MakeTaskReady (const GpTaskId aTaskId)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    if (iIsRequestStopAndJoin)
    {
        return false;
    }

    _MakeTaskReady(aTaskId, {});
    return true;
}

bool    GpTaskSchedulerV1::MakeTaskReady
(
    const GpTaskId  aTaskId,
    GpAny           aMessage
)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    if (iIsRequestStopAndJoin)
    {
        return false;
    }

    _MakeTaskReady(aTaskId, std::move(aMessage));
    return true;
}

bool    GpTaskSchedulerV1::Reschedule
(
    const GpTaskRunRes::EnumT   aRunRes,
    GpTask::SP&&                aTask
) noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    if (iIsRequestStopAndJoin)
    {
        return false;
    }

    try
    {
        switch (aRunRes)
        {
            case GpTaskRunRes::READY_TO_RUN:
            {
                // TODO: reimplement iTasksProducer with...
                _MoveToReady(std::move(aTask), {});
            } break;
            case GpTaskRunRes::WAIT:
            {
                if (iMarkedAsReadyIds.contains(aTask.Vn().TaskId().Value()))
                {
                    // TODO: reimplement iTasksProducer with...
                    _MoveToReady(std::move(aTask), {});
                } else
                {
                    _MoveToWaiting(std::move(aTask));
                }
            } break;
            case GpTaskRunRes::DONE:
            {
                aTask.Clear();
            } break;
        }

        return true;
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr("[GpTaskSchedulerV1::Reschedule]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr("[GpTaskSchedulerV1::Reschedule]: exception: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr("[GpTaskSchedulerV1::Reschedule]: unknown exception"_sv);
    }

    return false;
}

void    GpTaskSchedulerV1::_MakeTaskReady
(
    const GpTaskId  aTaskId,
    GpAny           aMessage
)
{
    if (iIsRequestStopAndJoin == true)
    {
        return;
    }

    // Try to find task in iWaitingTasks
    auto waitingTaskIter = iWaitingTasks.find(aTaskId.Value());

    if (waitingTaskIter != std::end(iWaitingTasks)) [[likely]]
    {
        // Move task to ready
        GpTask::SP taskSP = std::move(waitingTaskIter->second);
        iWaitingTasks.erase(waitingTaskIter);
        _MoveToReady(std::move(taskSP), std::move(aMessage));
    } else
    {
        // Keep the aTaskId to set iMarkedAsReadyIds (In case a task is currently running...)
        if (!aMessage.Empty())
        {
            std::optional<GpTask*> taskPtrOpt = GpTask::STaskById(aTaskId);
            if (taskPtrOpt.has_value())
            {
                taskPtrOpt.value()->PushMessage
                (
                    std::move(aMessage),
                    GpMethodAccess(this)
                );
            }
        }

        iMarkedAsReadyIds.emplace(aTaskId.Value());
    }
}

void    GpTaskSchedulerV1::_MoveToReady
(
    GpTask::SP  aTask,
    GpAny       aMessage
)
{
    if (iIsRequestStopAndJoin == true)
    {
        return;
    }

    GpTask&         task    = aTask.Vn();
    const GpTaskId  taskId  = task.TaskId();

    task.UpStartRequestFlag(GpMethodAccess{this});

    if (!aMessage.Empty())
    {
        task.PushMessage
        (
            std::move(aMessage),
            GpMethodAccess(this)
        );
    }

    if (iReadyTasks.PushAndNotifyOne(std::move(aTask)) == false) [[unlikely]]
    {
        THROW("Ready tasks queue is full"_sv);
    }

    iMarkedAsReadyIds.erase(taskId.Value());
}

void    GpTaskSchedulerV1::_MoveToWaiting (GpTask::SP aTask)
{
    if (iIsRequestStopAndJoin == true)
    {
        return;
    }

    GpTask&         task    = aTask.Vn();
    const GpTaskId  taskId  = task.TaskId();

    task.UpStartRequestFlag(GpMethodAccess{this});

    iWaitingTasks.emplace(taskId.Value(), std::move(aTask));
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
