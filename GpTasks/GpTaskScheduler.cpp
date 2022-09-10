#include "GpTaskScheduler.hpp"
#include "GpTaskAccessor.hpp"
#include "Fibers/GpTaskFiberAccessor.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <iostream>

namespace GPlatform {

GpTaskScheduler GpTaskScheduler::sTaskScheduler;

GpTaskScheduler::GpTaskScheduler (void) noexcept
{
}

GpTaskScheduler::~GpTaskScheduler (void) noexcept
{
}

void    GpTaskScheduler::Start (const size_t aExecutorsCount)
{
    {
        std::scoped_lock lock(iLock);

        THROW_COND_GP
        (
            iIsStarted == false,
            "Already started"_sv
        );

        iIsStarted = true;
    }

    THROW_COND_GP
    (
        aExecutorsCount >= 1,
        "Executors count must be >= 1"_sv
    );

    iExecutorsPool.Init
    (
        aExecutorsCount,
        aExecutorsCount
    );
}

void    GpTaskScheduler::RequestStop (void) noexcept
{
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
    {
        std::scoped_lock lock(iLock);
        if (iIsStarted == false)
        {
            return;
        }
    }

    iExecutorsPool.Join();
    iExecutorsPool.Clear();
}

GpUUID  GpTaskScheduler::NewToReady (GpTask::SP aTask)
{
    GpUUID taskGuid;

    {
        std::scoped_lock lock(iLock);

        THROW_COND_GP
        (
            iIsStopRequested == false,
            "Stop requested"_sv
        );

        GpTask& task = aTask.V();
        taskGuid = task.Guid();

        THROW_COND_GP
        (
            GpTaskAccessor::SState(task) == GpTask::StateTE::NOT_ASSIGNED_TO_SCHEDULER,
            "Task already assigned to scheduler"_sv
        );

        GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);

        iReadyTasks.push_back(std::move(aTask));

        iExecutorsPool.WakeupOne();
    }

    return taskGuid;
}

GpItcFuture::SP GpTaskScheduler::NewToReadyDepend (GpTask::SP aTask)
{
    GpItcFuture::SP future = aTask.V().Future();
    NewToReady(std::move(aTask));
    return future;
}

GpUUID  GpTaskScheduler::NewToWaiting (GpTask::SP aTask)
{
    GpUUID taskGuid;

    {
        std::scoped_lock lock(iLock);

        THROW_COND_GP
        (
            iIsStopRequested == false,
            "Stop requested"_sv
        );

        GpTask& task = aTask.V();
        taskGuid = task.Guid();

        THROW_COND_GP
        (
            GpTaskAccessor::SState(task) == GpTask::StateTE::NOT_ASSIGNED_TO_SCHEDULER,
            "Task already assigned to scheduler"_sv
        );

        GpTaskAccessor::SUpdateState(task, GpTask::StateTE::WAITING);

        iWaitingTasks.try_emplace(taskGuid, std::move(aTask));
    }

    return taskGuid;
}

GpItcFuture::SP GpTaskScheduler::NewToWaitingDepend (GpTask::SP aTask)
{
    GpItcFuture::SP future = aTask.V().Future();
    NewToWaiting(std::move(aTask));
    return future;
}

void    GpTaskScheduler::MakeTaskReady (const GpUUID& aTaskGuid)
{
    std::scoped_lock lock(iLock);

    THROW_COND_GP
    (
        iIsStopRequested == false,
        "Stop requested"_sv
    );

    auto iter = iWaitingTasks.find(aTaskGuid);

    if (iter != iWaitingTasks.end())
    {
        GpTask::SP  taskSP  = std::move(iter->second);
        GpTask&     task    = taskSP.V();
        iWaitingTasks.erase(iter);

        GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);
        iReadyTasks.push_back(std::move(taskSP));

        iExecutorsPool.WakeupOne();
    }
}

/*void  GpTaskScheduler::RequestTaskStop (const GpUUID& aTaskGuid)
{
    std::scoped_lock lock(iLock);

    if (iIsStopRequested)
    {
        return;
    }

    //Try to find in waiting tasks
    auto iter = iWaitingTasks.find(aTaskGuid);

    if (iter != iWaitingTasks.end())
    {
        GpTask::SP  taskSP  = std::move(iter->second);
        GpTask&     task    = taskSP.V();
        iWaitingTasks.erase(iter);

        GpTaskAccessor::SRequestStop(task);
        GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);

        iReadyTasks.push_back(taskSP);
        iExecutorsPool.WakeupOne();

        return taskSP;
    }

    //Try to find in ready tasks
    for (GpTask::SP& taskSP: iReadyTasks)
    {
        GpTask& task = taskSP.V();
        if (task.Guid() == aTaskGuid)
        {
            GpTaskAccessor::SRequestStop(task);
            return taskSP;
        }
    }

    return GpTask::SP::SNull();
}*/

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

            if (lastTask.IsStopRequested() == false)
            {
                if (aLastTaskExecRes == GpTaskDoRes::READY_TO_EXEC)
                {
                    if (!iReadyTasks.empty())
                    {
                        GpTaskAccessor::SUpdateState(lastTask, GpTask::StateTE::READY_TO_RUN);
                        iReadyTasks.push_back(std::move(aLastTaskSP));
                    } else
                    {
                        return aLastTaskSP;
                    }
                } else if (aLastTaskExecRes == GpTaskDoRes::WAITING)
                {
                    // State can be: RUNNING (task just came from executor), READY_TO_RUN (task was moved to READY_TO_RUN)
                    if (GpTaskAccessor::SState(lastTask) == GpTask::StateTE::RUNNING)
                    {
                        if (lastTask.HasEventsNoLock())
                        {
                            if (!iReadyTasks.empty())
                            {
                                GpTaskAccessor::SUpdateState(lastTask, GpTask::StateTE::READY_TO_RUN);
                                iReadyTasks.push_back(std::move(aLastTaskSP));
                            } else
                            {
                                return aLastTaskSP;
                            }
                        } else
                        {
                            GpTaskAccessor::SUpdateState(lastTask, GpTask::StateTE::WAITING);
                            iWaitingTasks.try_emplace(lastTask.Guid(), std::move(aLastTaskSP));
                        }
                    } else//READY_TO_RUN
                    {
                        if (!iReadyTasks.empty())
                        {
                            iReadyTasks.push_back(std::move(aLastTaskSP));
                        } else
                        {
                            return aLastTaskSP;
                        }
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

        if (   (lastTask.IsStopRequested())
            || (iIsStopRequested == true)
            || (GpTaskAccessor::SState(lastTask) == GpTask::StateTE::FINISHED))
        {
            _SDestroyTask(lastTask);
        }

        aLastTaskSP.Clear();
    }

    // Get next ready task
    if (iIsStopRequested == false)
    {
        std::scoped_lock lock(iLock);

        if (iReadyTasks.empty())
        {
            return GpTask::SP::SNull();
        } else
        {
            GpTask::SP nextTask = iReadyTasks.front();
            iReadyTasks.pop_front();
            GpTaskAccessor::SUpdateState(nextTask.Vn(), GpTask::StateTE::RUNNING);
            return nextTask;
        }
    } else
    {
        return GpTask::SP::SNull();
    }
}

void    GpTaskScheduler::_SDestroyTask (GpTask& aTask)
{
    GpRAIIonDestruct onDestruct
    ([](){
        GpTask::SClearCurrent();
    });

    GpTask::SSetCurrent(&aTask);

    try
    {
        //Request stop
        GpTaskAccessor::SRequestStop(aTask);

        //Get task type
        const GpTaskType taskType = aTask.TaskType();

        if (taskType == GpTaskType::FIBER)
        {
            GpTaskFiber& taskFiber = static_cast<GpTaskFiber&>(aTask);

            {
                std::scoped_lock lock(taskFiber.EventsLock());
                GpTaskFiberAccessor::SClearCtx(taskFiber);
            }
        } else if (taskType == GpTaskType::THREAD)
        {
            GpThreadStopSource  stopSource;
            GpThreadStopToken   stopToken = stopSource.get_token();
            stopSource.request_stop();

            GpTaskAccessor::SRun(aTask, stopToken);
        }
    }  catch (const GpException& e)
    {
        std::cerr << "[GpTaskScheduler::DestroyTask]: " << e.what() << std::endl;
    } catch (const std::exception& e)
    {
        std::cerr << "[GpTaskScheduler::DestroyTask]: " << e.what() << std::endl;
    } catch (...)
    {
        std::cerr << "[GpTaskScheduler::DestroyTask]: Unknown exception" << std::endl;
    }
}

void    GpTaskScheduler::Clear (void) noexcept
{
    //Set iIsStopRequested flag
    {
        std::scoped_lock lock(iLock);
        iIsStopRequested = true;
    }

    //
    iExecutorsPool.WakeupAll();

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
                _SDestroyTask(nextTask.V());
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
                _SDestroyTask(nextTask.V());
                nextTask.Clear();
            }
        }
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)

