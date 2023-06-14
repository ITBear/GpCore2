#include "GpTaskScheduler.hpp"
#include "GpTaskAccessor.hpp"
#include "Fibers/GpTaskFiberAccessor.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <iostream>
#include "../GpUtils/Other/GpRAIIonDestruct.hpp"

namespace GPlatform {

GpTaskScheduler GpTaskScheduler::sInstance;

GpTaskScheduler::GpTaskScheduler (void) noexcept
{
}

GpTaskScheduler::~GpTaskScheduler (void) noexcept
{
}

void    GpTaskScheduler::Start (const size_t aExecutorsCount)
{
    THROW_COND_GP
    (
        aExecutorsCount >= 1,
        "Executors count must be >= 1"_sv
    );

    {
        std::scoped_lock lock(iLock);

        THROW_COND_GP
        (
            iIsStarted == false,
            "Already started"_sv
        );

        iExecutorsPool.Start(aExecutorsCount);
        iIsStarted      = true;
        iExecutorsCount = aExecutorsCount;
    }
}

void    GpTaskScheduler::RequestStop (void) noexcept
{
    //Check if started
    {
        std::scoped_lock lock(iLock);
        if (iIsStarted == false)
        {
            return;
        }
    }

    Clear();

    iExecutorsPool.RequestStop();
}

void    GpTaskScheduler::Join (void) noexcept
{
    //Check if started
    {
        std::scoped_lock lock(iLock);
        if (iIsStarted == false)
        {
            return;
        }
    }

    iExecutorsPool.Join();
}

void    GpTaskScheduler::NewToReady (GpTask::SP aTask)
{
    _NewToReady(std::move(aTask), false);
}

GpTaskScheduler::CompleteItcFutureT::SP GpTaskScheduler::NewToReadyDepend (GpTask::SP aTask)
{
    return _NewToReady(std::move(aTask), true);
}

void    GpTaskScheduler::NewToWaiting (GpTask::SP aTask)
{
    _NewToWaiting(std::move(aTask), false);
}

GpTaskScheduler::CompleteItcFutureT::SP GpTaskScheduler::NewToWaitingDepend (GpTask::SP aTask)
{
    return _NewToWaiting(std::move(aTask), true);
}

void    GpTaskScheduler::MakeTaskReady (const GpUUID& aTaskGuid)
{
    std::scoped_lock lock(iLock);

    if (iIsStopRequested)
    {
        return;
    }

    //Try to find task in waiting
    {
        auto iter = iWaitingTasks.find(aTaskGuid);

        if (iter != iWaitingTasks.end())
        {
            GpTask::SP  taskSP  = std::move(iter->second);
            GpTask&     task    = taskSP.V();
            iWaitingTasks.erase(iter);

            GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);
            iReadyTasks.push_back(std::move(taskSP));

            iExecutorsPool.WakeupNextIdle();
        }

        return;
    }

    //Try to find in running
    {
        auto taskOpt = _FindInRunningTasks(aTaskGuid);

        if (taskOpt.has_value())
        {
            taskOpt.value()->UpRunRequestFlag();
        }
    }
}

GpTask::SP  GpTaskScheduler::_Reshedule
(
    GpTask::SP          aLastTaskSP,
    const GpTaskDoRes   aLastTaskExecRes
) noexcept
{
    //TODO: check noexcept

    // Process last task result
    if (aLastTaskSP.IsNotNULL())
    {
        GpTask& lastTask = aLastTaskSP.Vn();

        if (iIsStopRequested == false)
        {
            std::scoped_lock schedulerTaskScopedLock(iLock, lastTask.EventsLock());

            _RemoveFromRuningTasks(lastTask.Guid());

            if (lastTask.IsStopRequested() == false)
            {
                if (aLastTaskExecRes == GpTaskDoRes::READY_TO_EXEC)
                {
                    if (iReadyTasks.empty())
                    {
                        _AddToRuningTasks(aLastTaskSP);
                        return aLastTaskSP;
                    }

                    GpTaskAccessor::SUpdateState(lastTask, GpTask::StateTE::READY_TO_RUN);
                    iReadyTasks.push_back(std::move(aLastTaskSP));
                } else if (aLastTaskExecRes == GpTaskDoRes::WAITING)
                {
                    // State can be: RUNNING (task just came from executor), READY_TO_RUN (task was moved to READY_TO_RUN)
                    if (GpTaskAccessor::SState(lastTask) == GpTask::StateTE::RUNNING)
                    {
                        if (   lastTask.HasEventsNoLock()
                            || lastTask.RunRequestFlag() == true)
                        {
                            if (iReadyTasks.empty())
                            {
                                _AddToRuningTasks(aLastTaskSP);
                                return aLastTaskSP;
                            }

                            GpTaskAccessor::SUpdateState(lastTask, GpTask::StateTE::READY_TO_RUN);
                            iReadyTasks.push_back(std::move(aLastTaskSP));
                        } else
                        {
                            GpTaskAccessor::SUpdateState(lastTask, GpTask::StateTE::WAITING);
                            iWaitingTasks.try_emplace(lastTask.Guid(), std::move(aLastTaskSP));
                        }
                    } else//READY_TO_RUN
                    {
                        if (iReadyTasks.empty())
                        {
                            _AddToRuningTasks(aLastTaskSP);
                            return aLastTaskSP;
                        }

                        iReadyTasks.push_back(std::move(aLastTaskSP));
                    }
                } else if (aLastTaskExecRes == GpTaskDoRes::DONE)
                {
                    GpTaskAccessor::SUpdateState(lastTask, GpTask::StateTE::FINISHED);
                }
            } else
            {
                GpTaskAccessor::SUpdateState(lastTask, GpTask::StateTE::FINISHED);
            }
        }

        if (aLastTaskSP.IsNotNULL())
        {
            _DestroyTaskSP(std::move(aLastTaskSP));
        }
    }

    // Get next ready task
    if (iIsStopRequested)
    {
        return GpTask::SP::SNull();
    }

    {
        std::scoped_lock lock(iLock);

        if (iReadyTasks.empty())
        {
            return GpTask::SP::SNull();
        }

        GpTask::SP nextTask = iReadyTasks.front();
        iReadyTasks.pop_front();
        GpTaskAccessor::SUpdateState(nextTask.Vn(), GpTask::StateTE::RUNNING);

        _AddToRuningTasks(aLastTaskSP);
        return nextTask;
    }
}

void    GpTaskScheduler::_DestroyTaskSP (GpTask::SP&& aTask)
{
    std::scoped_lock lock(iReadyToDestroyLock);

    GpTask& task = aTask.V();

    _RemoveFromRuningTasks(task.Guid());

    //Check for child tasks
    if (task.ChildTasksCount() > 0)
    {
        iReadyToDestroyTasks.try_emplace(task.Guid(), std::move(aTask));
        return;
    }

    _DestroyTaskRef(std::move(aTask));
}

void    GpTaskScheduler::_DestroyTaskRef (GpTask::SP&& aTask)
{
    GpTask* parentTask = nullptr;

    {
        GpRAIIonDestruct onDestruct
        (
            [&]()
            {
                aTask.Clear();
                GpTask::SClearCurrent();
            }
        );

        GpTask& task = aTask.V();

        _RemoveFromRuningTasks(task.Guid());

        parentTask = task.ParentTask();

        GpTask::SSetCurrent(&task);

        try
        {
            //Request stop
            GpTaskAccessor::SRequestStop(task);

            //Get task type
            const GpTaskType taskType = task.TaskType();

            if (taskType == GpTaskType::FIBER)
            {
                GpTaskFiber& taskFiber = static_cast<GpTaskFiber&>(task);

                {
                    std::scoped_lock lock(taskFiber.EventsLock());
                    GpTaskFiberAccessor::SClearCtx(taskFiber);
                }
            } else if (taskType == GpTaskType::THREAD)
            {
                GpThreadStopSource  stopSource;
                GpThreadStopToken   stopToken = stopSource.get_token();
                stopSource.request_stop();

                GpTaskAccessor::SRun(task, stopToken);
            }
        } catch (const GpException& e)
        {
            GpStringUtils::SCerr("[GpTaskScheduler::DestroyTask]: "_sv + e.what() + "\n"_sv);
        } catch (const std::exception& e)
        {
            GpStringUtils::SCerr("[GpTaskScheduler::DestroyTask]: "_sv + e.what() + "\n"_sv);
        } catch (...)
        {
            GpStringUtils::SCerr("[GpTaskScheduler::DestroyTask]: Unknown exception\n"_sv);
        }
    }

    if (parentTask != nullptr)
    {
        if (parentTask->ChildTasksDec() == 0)
        {
            const GpUUID parentTaskId = parentTask->Guid();

            auto iter = iReadyToDestroyTasks.find(parentTaskId);

            if (iter != iReadyToDestroyTasks.end())
            {
                GpTask::SP taskToDestroy = iter->second;
                iReadyToDestroyTasks.erase(iter);
                _DestroyTaskRef(std::move(taskToDestroy));
            }
        }
    }
}

GpTaskScheduler::CompleteItcFutureT::SP GpTaskScheduler::_NewToReady
(
    GpTask::SP  aTask,
    const bool  aIsDependent
)
{
    std::scoped_lock lock(iLock);

    CompleteItcFutureT::SP future;

    THROW_COND_GP
    (
        iIsStopRequested == false,
        "Stop requested"_sv
    );

    GpTask& task = aTask.V();

    THROW_COND_GP
    (
        GpTaskAccessor::SState(task) == GpTask::StateTE::NOT_ASSIGNED_TO_SCHEDULER,
        "Task already assigned to scheduler"_sv
    );

    GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);

    if (aIsDependent)
    {
        GpTask* currentTask = GpTask::SCurrent();

        if (currentTask != nullptr)
        {
            GpTaskAccessor::SSetParentTask(task, currentTask);
            currentTask->ChildTasksInc();
        }

        future = aTask.V().Future();
    }

    iReadyTasks.push_back(std::move(aTask));
    iExecutorsPool.WakeupNextIdle();

    return future;
}

GpTaskScheduler::CompleteItcFutureT::SP GpTaskScheduler::_NewToWaiting
(
    GpTask::SP  aTask,
    const bool  aIsDependent
)
{
    std::scoped_lock lock(iLock);

    CompleteItcFutureT::SP future;

    THROW_COND_GP
    (
        iIsStopRequested == false,
        "Stop requested"_sv
    );

    GpTask& task = aTask.V();

    THROW_COND_GP
    (
        GpTaskAccessor::SState(task) == GpTask::StateTE::NOT_ASSIGNED_TO_SCHEDULER,
        "Task already assigned to scheduler"_sv
    );

    GpTaskAccessor::SUpdateState(task, GpTask::StateTE::WAITING);

    if (aIsDependent)
    {
        GpTask* currentTask = GpTask::SCurrent();

        if (currentTask != nullptr)
        {
            GpTaskAccessor::SSetParentTask(task, currentTask);
            currentTask->ChildTasksInc();
        }

        future = aTask.V().Future();
    }

    iWaitingTasks.try_emplace(task.Guid(), std::move(aTask));

    return future;
}

void    GpTaskScheduler::Clear (void) noexcept
{
    //Set iIsStopRequested flag
    {
        std::scoped_lock lock(iLock);
        iIsStopRequested = true;
    }

    //
    //iExecutorsPool.WakeupNextIdle();

    //Destroy ready tasks
    {
        bool        fin = false;
        GpTask::SP  nextTask;

        while (!fin)
        {
            {
                std::scoped_lock lock(iLock);
                fin = (iReadyTasks.size() == 0);

                if (!fin)
                {
                    nextTask = iReadyTasks.front();
                    iReadyTasks.pop_front();
                }
            }

            if (nextTask.IsNotNULL())
            {
                _DestroyTaskSP(std::move(nextTask));
                nextTask.Clear();
            }
        }
    }

    //Destroy waiting tasks
    {
        bool        fin = false;
        GpTask::SP  nextTask;

        while (!fin)
        {
            {
                std::scoped_lock lock(iLock);
                fin = (iWaitingTasks.size() == 0);

                if (!fin)
                {
                    auto iter = iWaitingTasks.begin();
                    nextTask = iter->second;
                    iWaitingTasks.erase(iter);
                }
            }

            if (nextTask.IsNotNULL())
            {
                _DestroyTaskSP(std::move(nextTask));
            }
        }
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)

