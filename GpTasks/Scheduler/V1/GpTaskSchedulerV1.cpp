#include "GpTaskSchedulerV1.hpp"

#include "GpTaskExecutorV1.hpp"
#include "../../GpTaskGroupsManager.hpp"
#include "GpTaskExecutorV1.hpp"

#include <GpCore2/GpUtils/Exceptions/GpExceptionUtils.hpp>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>

#include <tuple>
#include <utility>

namespace GPlatform {

GpTaskSchedulerV1::GpTaskSchedulerV1 (StopServiceFnT aStopServiceFn) noexcept:
GpTaskScheduler(aStopServiceFn)
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
    THROW_COND_GP
    (
        aExecutorsCount > 0,
        "Executors count must be > 0"_sv
    );

    THROW_COND_GP
    (
        aTasksMaxCount > 0,
        "Tasks max count must be > 0"_sv
    );

    THROW_COND_GP
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
        GpStringUtils::SCout("[GpTaskSchedulerV1::RequestStopAndJoin]: begin...");

        // Prepare executors for stop
        {
            // Send stop request to all executors
            GpStringUtils::SCout("[GpTaskSchedulerV1::RequestStopAndJoin]: Send stop request to all executors...");

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
            GpStringUtils::SCout("[GpTaskSchedulerV1::RequestStopAndJoin]: Join executor threads...");

            GpThread::C::Vec::SP executorThreads;
            {
                GpUniqueLock<GpMutex> uniqueLock{iMutex};
                executorThreads = iExecutorThreads;
            }

            for (GpThread::SP& executorThread: executorThreads)
            {
                GpStringUtils::SCout("[GpTaskSchedulerV1::RequestStopAndJoin]: Join to next executor thread...");
                executorThread->Join();
            }
        }

        // Clear executor threads
        {
            GpStringUtils::SCout("[GpTaskSchedulerV1::RequestStopAndJoin]: Clear executor threads...");
            GpUniqueLock<GpMutex> uniqueLock{iMutex};
            iExecutorThreads.clear();
        }

        // Check excutor threads done result
        {
            GpStringUtils::SCout("[GpTaskSchedulerV1::RequestStopAndJoin]: Check excutor threads done result...");

            while (!iExecutorDoneFutures.empty())
            {
                for (auto iter = std::begin(iExecutorDoneFutures); iter != std::end(iExecutorDoneFutures); )
                {
                    const bool isReady = ExecutorDoneFutureT::SCheckIfReady
                    (
                        iter->V(),
                        [](ssize_t& aExecutorId)
                        {
                            GpStringUtils::SCout("[GpTaskSchedulerV1::RequestStopAndJoin]: executor done: "_sv + aExecutorId);
                        },
                        [](const GpException& aException)
                        {
                            std::string exStr = GpExceptionUtils::SToString(aException);

                            THROW_GP
                            (
                                "[GpTaskSchedulerV1::WaitForRunners]: done with error: "_sv + exStr,
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
            GpStringUtils::SCout("[GpTaskSchedulerV1::RequestStopAndJoin]: UpStopRequestFlag for all ready tasks...");

            while (!iReadyTasks.Empty())
            {
                GpTask::C::Opt::SP taskOpt = iReadyTasks.WaitAndPop(0.0_si_s).value();

                if (taskOpt.has_value())
                {
                    GpTask& task = taskOpt.value().V();

                    task.UpStopRequestFlag(GpMethodAccess<GpTaskScheduler>{this});
                    task.Execute(GpMethodAccess<GpTaskScheduler>{this});
                }
            }

            iReadyTasks.Clear();
        }

        // UpStopRequestFlag for all waiting tasks
        {
            GpStringUtils::SCout("[GpTaskSchedulerV1::RequestStopAndJoin]: UpStopRequestFlag for all waiting tasks...");

            WaitingTasksT waitingTasks;
            {
                GpUniqueLock<GpMutex> uniqueLock{iMutex};
                waitingTasks = std::move(iWaitingTasks);
                iWaitingTasks.clear();
            }

            for (auto&[taskGuid, taskSP]: waitingTasks)
            {
                taskSP->UpStopRequestFlag(GpMethodAccess<GpTaskScheduler>{this});
                taskSP->Execute(GpMethodAccess<GpTaskScheduler>{this});
            }
        }

        GpStringUtils::SCout("[GpTaskSchedulerV1::RequestStopAndJoin]: END!");
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

void    GpTaskSchedulerV1::NewToReady (GpTask::SP aTask)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    THROW_COND_GP
    (
        iIsRequestStopAndJoin == false,
        "Task scheduler stopped"_sv
    );

    _MoveToReady(std::move(aTask), {});
}

void    GpTaskSchedulerV1::NewToWaiting (GpTask::SP aTask)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    THROW_COND_GP
    (
        iIsRequestStopAndJoin == false,
        "Task scheduler stopped"_sv
    );

    _MoveToWaiting(std::move(aTask));
}

void    GpTaskSchedulerV1::MakeTaskReady (const GpTaskId aTaskId)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    _MakeTaskReady(aTaskId, {});
}

void    GpTaskSchedulerV1::MakeTaskReady
(
    const GpTaskId  aTaskId,
    GpAny           aMessage
)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    _MakeTaskReady(aTaskId, std::move(aMessage));
}

void    GpTaskSchedulerV1::MakeTasksReadyByGroupId
(
    const GpTaskGroupId aGpTaskGroupId,
    GpAny               aMessage
)
{
    GpTaskGroupsManager::TaskIdVecT tasks;
    GpTaskGroupsManager::S().FindTasksByGroupId(aGpTaskGroupId, tasks);

    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    for (const GpTaskId& taskId: tasks)
    {
        _MakeTaskReady(taskId, aMessage);
    }
}

bool    GpTaskSchedulerV1::AddTaskToGroup
(
    const GpTaskId      aTaskGuid,
    const GpTaskGroupId aGpTaskGroupId
)
{
    return GpTaskGroupsManager::S().AddTaskToGroup(aTaskGuid, aGpTaskGroupId);
}

bool    GpTaskSchedulerV1::RemoveTaskFromGroup
(
    const GpTaskId      aTaskGuid,
    const GpTaskGroupId aGpTaskGroupId
)
{
    return GpTaskGroupsManager::S().RemoveTaskFromGroup(aTaskGuid, aGpTaskGroupId);
}

bool    GpTaskSchedulerV1::Reschedule
(
    const GpTaskRunRes::EnumT   aRunRes,
    GpTask::SP&&                aTask
) noexcept
{
    try
    {
        GpUniqueLock<GpMutex> uniqueLock{iMutex};

        switch (aRunRes)
        {
            case GpTaskRunRes::READY_TO_RUN:
            {
                // TODO: reimplement iTasksProducer with...
                _MoveToReady(std::move(aTask), {});
            } break;
            case GpTaskRunRes::WAIT:
            {
                if (iMarkedAsReadyIds.contains(aTask->TaskId()))
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
#if defined(DEBUG_BUILD)
    if (iIsRequestStopAndJoin)
    {
        GpDebugging::SBreakpoint();
    }
#endif //#if defined(DEBUG_BUILD)

    THROW_COND_GP
    (
        iIsRequestStopAndJoin == false,
        "Task scheduler stopped"_sv
    );

    // Try to find task in iWaitingTasks
    auto waitingTaskIter = iWaitingTasks.find(aTaskId);

    if (waitingTaskIter != std::end(iWaitingTasks)) [[likely]]
    {
        // Move task to ready
        GpTask::SP taskSP = std::move(waitingTaskIter->second);
        iWaitingTasks.erase(waitingTaskIter);
        _MoveToReady(std::move(taskSP), std::move(aMessage));
    } else      
    {
        // Keep the aTaskId to set iMarkedAsReadyIds (In case a task is currently running...)
        std::optional<GpTask*> taskPtrOpt = GpTask::STaskById(aTaskId);
        if (taskPtrOpt.has_value())
        {
            taskPtrOpt.value()->PushMessage(std::move(aMessage));
        }

        iMarkedAsReadyIds.emplace(aTaskId);
    }
}

void    GpTaskSchedulerV1::_MoveToReady
(
    GpTask::SP  aTask,
    GpAny       aMessage
)
{
    GpTask&         task    = aTask.V();
    const GpTaskId  taskId  = task.TaskId();

    if (!aMessage.Empty())
    {
        task.PushMessage(std::move(aMessage));
    }

    if (iReadyTasks.PushAndNotifyOne(std::move(aTask)) == false) [[unlikely]]
    {
        THROW_GP("Ready tasks queue is full"_sv);
    }

    iMarkedAsReadyIds.erase(taskId);
}

void    GpTaskSchedulerV1::_MoveToWaiting (GpTask::SP aTask)
{
    const GpTaskId taskId = aTask->TaskId();
    iWaitingTasks.emplace(taskId, std::move(aTask));
}

}// namespace GPlatform
