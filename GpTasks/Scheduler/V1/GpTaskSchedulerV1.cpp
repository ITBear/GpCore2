#include "GpTaskSchedulerV1.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskExecutorV1.hpp"
#include "../../../GpUtils/Exceptions/GpExceptionUtils.hpp"
#include "../../../../GpLog/GpLogCore/GpLog.hpp"

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
    std::scoped_lock lock(iLock);

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
    TaskProducerConsumerT::SP tasksProducerConsumer = MakeSP<TaskProducerConsumerT>(aTasksMaxCount);
    iTasksProducer = MakeSP<TaskProducerT>(tasksProducerConsumer);

    iExecutorThreads.reserve(aExecutorsCount);

    for (size_t id = 0; id < aExecutorsCount; id++)
    {
        GpTaskExecutorV1::DonePromiseT executorDonePromise;

        iExecutorDoneFutures.emplace_back(executorDonePromise.Future());

        GpThread& executorThread = iExecutorThreads.emplace_back(MakeSP<GpThread>(u8"Task exec: "_sv + id)).V();

        executorThread.Run
        (
            MakeSP<GpTaskExecutorV1>
            (
                id,
                GpTaskExecutorV1::ConsumerT(tasksProducerConsumer),
                *this,
                std::move(executorDonePromise)
            )
        );
    }
}

void    GpTaskSchedulerV1::RequestStopAndJoin (void) noexcept
{
    try
    {
        GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: begin...");

        // Prepare executors for stop
        {
            std::scoped_lock lock(iLock);

            iIsRequestStopAndJoin = true;

            // Send stop request to all executors
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: Send stop request to all executors...");
            for (GpThread::SP& executorThread: iExecutorThreads)
            {
                executorThread->RequestStop();
            }
        }

        // Join executor threads
        {
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: Join executor threads...");
            for (GpThread::SP& executorThread: iExecutorThreads)
            {
                GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: Join to next executor thread...");
                executorThread->Join();
            }
        }

        // Clear executor threads
        {
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: Clear executor threads...");
            std::scoped_lock lock(iLock);
            iExecutorThreads.clear();
        }

        // Extract all redy tasks from iTasksProducer to local readyTasksQueue
        GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: Extract all redy tasks from iTasksProducer to local readyTasksQueue...");
        typename TaskProducerConsumerT::ItcResultT::C::Deque::Val readyTasksQueue;
        if (iTasksProducer.IsNotNULL())
        {
            readyTasksQueue = iTasksProducer.Vn().ProducerConsumer().V().ExtractQueue();
        }

        // Check excutor threads done result
        {
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: JCheck excutor threads done result...");

            while (!iExecutorDoneFutures.empty())
            {
                for (auto iter = iExecutorDoneFutures.begin(); iter != iExecutorDoneFutures.end(); )
                {
                    const bool isReady = ExecutorDoneFutureT::SCheckIfReady
                    (
                        iter->V(),
                        [](const ssize_t& aExecutorId)
                        {
                            LOG_INFO(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: executor done: "_sv + aExecutorId);
                        },
                        []()
                        {
                            LOG_INFO(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: done, empty result"_sv);
                        },
                        [](const GpException& aException)
                        {
                            std::u8string exStr = GpExceptionUtils::SToString(aException);

                            LOG_ERROR(u8"[GpUnitTestManager::WaitForRunners]: done with error: "_sv + exStr);

                            THROW_GP
                            (
                                u8"Unit Test Runner finished with error: "_sv + exStr,
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
            while (!readyTasksQueue.empty())
            {
                typename TaskProducerConsumerT::ItcResultT  taskRes = std::move(readyTasksQueue.front());
                GpTask::SP                                  taskSP  = std::move(taskRes.Payload());

                readyTasksQueue.pop_front();

                taskSP->UpStopRequestFlag();
                taskSP->Execute();
            }

            readyTasksQueue.clear();
        }

        // UpStopRequestFlag for all waiting tasks
        {
            GpStringUtils::SCout(u8"[GpTaskSchedulerV1::RequestStopAndJoin]: UpStopRequestFlag for all waiting tasks...");

            for (auto&[taskGuid, taskSP]: iWaitingTasks)
            {
                taskSP->UpStopRequestFlag();
                taskSP->Execute();
            }

            {
                std::scoped_lock lock(iLock);
                iWaitingTasks.clear();
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

void    GpTaskSchedulerV1::NewToReady
(
    GpTask::SP              aTask,
    const milliseconds_t    aWaitTimeout
)
{
    std::scoped_lock lock(iLock);

    THROW_COND_GP
    (
        iIsRequestStopAndJoin == false,
        u8"Task scheduler stopped"_sv
    );

    _MoveToReady(std::move(aTask), aWaitTimeout);
}

void    GpTaskSchedulerV1::NewToWaiting (GpTask::SP aTask)
{
    std::scoped_lock lock(iLock);

    THROW_COND_GP
    (
        iIsRequestStopAndJoin == false,
        u8"Task scheduler stopped"_sv
    );

    _MoveToWaiting(std::move(aTask));
}

void    GpTaskSchedulerV1::MakeTaskReady (const GpTask::IdT aTaskId)
{
    std::scoped_lock lock(iLock);

    _MakeTaskReady(aTaskId);
}

void    GpTaskSchedulerV1::MakeTaskReady
(
    const GpTask::IdT   aTaskId,
    GpAny               aPayload
)
{
    std::scoped_lock lock(iLock);

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
        std::scoped_lock lock(iLock);

        switch (aRunRes)
        {
            case GpTaskRunRes::READY_TO_RUN:
            {
                _MoveToReady(std::move(aTask), 0.0_si_ms);
            } break;
            case GpTaskRunRes::WAIT:
            {
                if (iMarkedAsReadyIds.contains(aTask->Id()))
                {
                    _MoveToReady(std::move(aTask), 0.0_si_ms);
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

void    GpTaskSchedulerV1::_MakeTaskReady (const GpTask::IdT aTaskId)
{
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
        _MoveToReady(waitingTaskIter->second, 0.0_si_ms);
        iWaitingTasks.erase(waitingTaskIter);
    } else
    {
        // Keep the aTaskId to set iMarkedAsReadyIds (In case a task is currently running...)
        iMarkedAsReadyIds.emplace(aTaskId);
    }
}

void    GpTaskSchedulerV1::_MoveToReady
(
    GpTask::SP              aTask,
    const milliseconds_t    aWaitTimeout
)
{
    const GpTask::IdT       taskId = aTask->Id();
    GpItcResult<GpTask::SP> itcRes(std::move(aTask));

    if (iTasksProducer->Produce(std::move(itcRes), aWaitTimeout) == false)
    {
        THROW_GP(u8"Produce timeout"_sv);
    }

    iMarkedAsReadyIds.erase(taskId);
}

void    GpTaskSchedulerV1::_MoveToWaiting (GpTask::SP aTask)
{
    const GpTask::IdT taskId = aTask->Id();
    iWaitingTasks.emplace(taskId, std::move(aTask));
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
