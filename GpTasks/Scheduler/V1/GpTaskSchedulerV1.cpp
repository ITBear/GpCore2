#include "GpTaskSchedulerV1.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskExecutorV1.hpp"
#include "../../../GpUtils/Exceptions/GpExceptionUtils.hpp"
#include "../../../../GpLog/GpLogCore/GpLog.hpp"
#include "GpTaskExecutorV1.hpp"
#include <tuple>
#include <utility>

namespace GPlatform {

GpTaskSchedulerV1::GpTaskSchedulerV1 (void) noexcept
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
    GpUniqueLock<GpMutex> uniqueLock(iMutex);

    GpTaskScheduler::Start
    (
        aExecutorsCount,
        aTasksMaxCount
    );

    //------------------ Check -------------------
    THROW_COND_GP
    (
        aExecutorsCount > 0,
        u8"Executors count must be > 0"_sv
    );

    THROW_COND_GP
    (
        aTasksMaxCount > 0,
        u8"Tasks max count must be > 0"_sv
    );

    THROW_COND_GP
    (
        iExecutorThreads.empty(),
        u8"Executor tasks already created"_sv
    );

    //------------------ Create executors -------------------
    iExecutorThreads.reserve(aExecutorsCount);

    for (size_t id = 0; id < aExecutorsCount; id++)
    {
        GpTaskExecutorV1::DonePromiseT executorDonePromise;

        iExecutorDoneFutures.emplace_back(executorDonePromise.Future());

        GpThread&               executorThread  = iExecutorThreads.emplace_back(MakeSP<GpThread>(u8"Task exec: "_sv + id)).V();
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
        GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: begin...");

        // Prepare executors for stop
        {
            // Send stop request to all executors
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: Send stop request to all executors...");

            GpThread::C::Vec::SP executorThreads;
            {
                GpUniqueLock<GpMutex> uniqueLock(iMutex);
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
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: Join executor threads...");

            GpThread::C::Vec::SP executorThreads;
            {
                GpUniqueLock<GpMutex> uniqueLock(iMutex);
                executorThreads = iExecutorThreads;
            }

            for (GpThread::SP& executorThread: executorThreads)
            {
                GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: Join to next executor thread...");
                executorThread->Join();
            }
        }

        // Clear executor threads
        {
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: Clear executor threads...");
            GpUniqueLock<GpMutex> uniqueLock(iMutex);
            iExecutorThreads.clear();
        }

        // Check excutor threads done result
        {
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: Check excutor threads done result...");

            while (!iExecutorDoneFutures.empty())
            {
                for (auto iter = iExecutorDoneFutures.begin(); iter != iExecutorDoneFutures.end(); )
                {
                    const bool isReady = ExecutorDoneFutureT::SCheckIfReady
                    (
                        iter->V(),
                        [](ssize_t& aExecutorId)
                        {
                            LOG_INFO(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: executor done: "_sv + aExecutorId);
                        },
                        [](const GpException& aException)
                        {
                            std::u8string exStr = GpExceptionUtils::SToString(aException);

                            LOG_ERROR(u8"[GpTaskSchedulerV1::WaitForRunners]: done with error: "_sv + exStr);

                            THROW_GP
                            (
                                u8"[GpTaskSchedulerV1::WaitForRunners]: done with error: "_sv + exStr,
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
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: UpStopRequestFlag for all ready tasks...");

            while (!iReadyTasks.Empty())
            {
                GpTask::C::Opt::SP taskOpt = iReadyTasks.WaitAndPop(0.0_si_s).value();

                if (taskOpt.has_value())
                {
                    GpTask& task = taskOpt.value().V();

                    task.UpStopRequestFlag();
                    task.Execute();
                }
            }

            iReadyTasks.Clear();
        }

        // UpStopRequestFlag for all waiting tasks
        {
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: UpStopRequestFlag for all waiting tasks...");

            WaitingTasksT waitingTasks;
            {
                GpUniqueLock<GpMutex> uniqueLock(iMutex);
                waitingTasks = std::move(iWaitingTasks);
                iWaitingTasks.clear();
            }

            for (auto&[taskGuid, taskSP]: waitingTasks)
            {
                taskSP->UpStopRequestFlag();
                taskSP->Execute();
            }
        }

        GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: END!");
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
    GpUniqueLock<GpMutex> uniqueLock(iMutex);

    THROW_COND_GP
    (
        iIsRequestStopAndJoin == false,
        u8"Task scheduler stopped"_sv
    );

    _MoveToReady(std::move(aTask));
}

void    GpTaskSchedulerV1::NewToWaiting (GpTask::SP aTask)
{
    GpUniqueLock<GpMutex> uniqueLock(iMutex);

    THROW_COND_GP
    (
        iIsRequestStopAndJoin == false,
        u8"Task scheduler stopped"_sv
    );

    _MoveToWaiting(std::move(aTask));
}

void    GpTaskSchedulerV1::MakeTaskReady (const GpTaskId aTaskId)
{
    GpUniqueLock<GpMutex> uniqueLock(iMutex);

    _MakeTaskReady(aTaskId);
}

void    GpTaskSchedulerV1::MakeTaskReady
(
    const GpTaskId  aTaskId,
    GpAny           aPayload
)
{
    GpUniqueLock<GpMutex> uniqueLock(iMutex);

    _MakeTaskReady(aTaskId);

    // Store payload in GpTaskPayloadStorage
    GpTaskPayloadStorage::S().PushPayload
    (
        aTaskId,
        std::move(aPayload)
    );
}

bool    GpTaskSchedulerV1::Reschedule
(
    const GpTaskRunRes::EnumT   aRunRes,
    GpTask::SP&&                aTask
) noexcept
{
    try
    {
        GpUniqueLock<GpMutex> uniqueLock(iMutex);

        switch (aRunRes)
        {
            case GpTaskRunRes::READY_TO_RUN:
            {
                // TODO: reimplement iTasksProducer with...
                _MoveToReady(std::move(aTask));
            } break;
            case GpTaskRunRes::WAIT:
            {
                if (iMarkedAsReadyIds.contains(aTask->Id()))
                {
                    // TODO: reimplement iTasksProducer with...
                    _MoveToReady(std::move(aTask));
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

void    GpTaskSchedulerV1::_MakeTaskReady (const GpTaskId aTaskId)
{
#if defined(DEBUG_BUILD)
    if (iIsRequestStopAndJoin)
    {
        __builtin_trap();
    }
#endif //#if defined(DEBUG_BUILD)

    THROW_COND_GP
    (
        iIsRequestStopAndJoin == false,
        u8"Task scheduler stopped"_sv
    );

    // Try to find task in iWaitingTasks
    auto waitingTaskIter = iWaitingTasks.find(aTaskId);

    if (waitingTaskIter != iWaitingTasks.end())
    {
        //Move task to ready
        _MoveToReady(waitingTaskIter->second);
        iWaitingTasks.erase(waitingTaskIter);
    } else
    {
        // Keep the aTaskId to set iMarkedAsReadyIds (In case a task is currently running...)
        iMarkedAsReadyIds.emplace(aTaskId);
    }
}

void    GpTaskSchedulerV1::_MoveToReady (GpTask::SP aTask)
{
    const GpTaskId taskId = aTask->Id();

    if (iReadyTasks.PushAndNotifyOne(std::move(aTask)) == false) [[unlikely]]
    {
        THROW_GP(u8"Ready tasks queue is full"_sv);
    }

    iMarkedAsReadyIds.erase(taskId);
}

void    GpTaskSchedulerV1::_MoveToWaiting (GpTask::SP aTask)
{
    const GpTaskId taskId = aTask->Id();
    iWaitingTasks.emplace(taskId, std::move(aTask));
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
