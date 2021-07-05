#include "GpTaskScheduler.hpp"
#include "GpTaskAccessor.hpp"

#include <iostream>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

static thread_local std::optional<GpTaskScheduler::WP> sCurrentTaskScheduler = std::nullopt;

GpTaskScheduler::GpTaskScheduler (void) noexcept
{
}

GpTaskScheduler::~GpTaskScheduler (void) noexcept
{
}

void    GpTaskScheduler::Start
(
    GpTaskScheduler::WP aSelfWP,
    const count_t       aExecutorsCount
)
{
    THROW_GPE_COND
    (
        aExecutorsCount >= 1_cnt,
        "Executors count must be >= 1"_sv
    );

    iExecutorsPool.SetScheduler(aSelfWP);
    iExecutorsPool.Init(aExecutorsCount, aExecutorsCount);
}

void    GpTaskScheduler::RequestStop (void) noexcept
{
    iExecutorsPool.RequestStop();
}

void    GpTaskScheduler::Join (void) noexcept
{
    iExecutorsPool.Join();
}

void    GpTaskScheduler::AfterJoin (void) noexcept
{
    iExecutorsPool.Clear();

    std::scoped_lock lock(iLock);

    //Terminate ready tasks
    bool isEmpty = false;

    while (!isEmpty)
    {
        GpTask::SP task;

        //Pick task
        {
            isEmpty = iReadyTasks.empty();
            if (!isEmpty)
            {
                task = iReadyTasks.front();
                iReadyTasks.pop();
            }
        }

        if (task.IsNotNULL())
        {
            //TODO: add tasks dependencies
            task.Vn().Terminate();
        }
    }

    //Terminate waiting tasks
    isEmpty = false;
    while (!isEmpty)
    {
        GpTask::SP task;

        //Pick task
        {
            isEmpty = iWaitingTasks.empty();
            if (!isEmpty)
            {
                auto iter = iWaitingTasks.begin();
                task = *iter;
                iWaitingTasks.erase(iter);
            }
        }

        if (task.IsNotNULL())
        {
            //TODO: add tasks dependencies
            task.Vn().Terminate();
        }
    }

    std::cout << "!![GpTaskScheduler::AfterJoin]: iReadyTasks.size()   = " << iReadyTasks.size() << std::endl;
    std::cout << "!![GpTaskScheduler::AfterJoin]: iWaitingTasks.size() = " << iWaitingTasks.size() << std::endl;

    std::cout << "!![GpTaskScheduler::AfterJoin]: Done!" << std::endl;
}

GpTask::SP  GpTaskScheduler::Reshedule
(
    GpTask::SP          aLastTask,
    const GpTask::ResT  aLastTaskExecRes
) noexcept
{
    // Process last task result
    if (aLastTask.IsNotNULL())
    {
        GpTask& task = aLastTask.Vn();

        std::scoped_lock lock(iLock, task.EventsLock());

        if (aLastTaskExecRes == GpTask::ResT::READY_TO_EXEC)
        {
            if (!iReadyTasks.empty())
            {
                GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);
                iReadyTasks.push(std::move(aLastTask));
            } else
            {
                return aLastTask;
            }
        } else if (aLastTaskExecRes == GpTask::ResT::WAITING)
        {
            // State can be: RUNNING (task just came from executor), READY_TO_RUN (task was moved to READY_TO_RUN from GpTaskScheduler::MoveToReady)
            if (GpTaskAccessor::SState(task) == GpTask::StateTE::RUNNING)
            {
                if (task.HasEventsNoLock())
                {
                    if (!iReadyTasks.empty())
                    {
                        GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);
                        iReadyTasks.push(std::move(aLastTask));
                    } else
                    {
                        return aLastTask;
                    }
                } else
                {
                    GpTaskAccessor::SUpdateState(task, GpTask::StateTE::WAITING);
                    iWaitingTasks.emplace(std::move(aLastTask));
                }
            } else//READY_TO_RUN
            {
                if (!iReadyTasks.empty())
                {
                    iReadyTasks.push(std::move(aLastTask));
                } else
                {
                    return aLastTask;
                }
            }
        } else if (aLastTaskExecRes == GpTask::ResT::DONE)
        {
            GpTaskAccessor::SUpdateState(task, GpTask::StateTE::FINISHED);
        }
    }//Process last task result

    // Get next ready task
    {
        std::scoped_lock lock(iLock);

        if (iReadyTasks.empty())
        {
            return GpTask::SP::SNull();
        } else
        {
            GpTask::SP nextTask = iReadyTasks.front();
            iReadyTasks.pop();
            GpTaskAccessor::SUpdateState(nextTask.Vn(), GpTask::StateTE::RUNNING);
            return nextTask;
        }
    }
}

void    GpTaskScheduler::AddTaskToReady (GpTask::SP aTask)
{
    {
        std::scoped_lock lock(iLock);

        GpTask& task = aTask.V();

        THROW_GPE_COND
        (
            GpTaskAccessor::SState(task) == GpTask::StateTE::NOT_ASSIGNED_TO_SCHEDULER,
            "Task already assigned to scheduler"_sv
        );

        GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);
        GpTaskAccessor::SSetScheduler(task, *this);
        GpTaskAccessor::SSetWeakPtr(task, aTask);

        iReadyTasks.push(std::move(aTask));
    }

    iExecutorsPool.WakeupOne();
}

void    GpTaskScheduler::AddTaskToWaiting (GpTask::SP aTask)
{
    std::scoped_lock lock(iLock);

    GpTask& task = aTask.V();

    THROW_GPE_COND
    (
        GpTaskAccessor::SState(task) == GpTask::StateTE::NOT_ASSIGNED_TO_SCHEDULER,
        "Task already assigned to scheduler"_sv
    );

    GpTaskAccessor::SUpdateState(task, GpTask::StateTE::WAITING);
    GpTaskAccessor::SSetScheduler(task, *this);
    GpTaskAccessor::SSetWeakPtr(task, aTask);

    iWaitingTasks.insert(std::move(aTask));
}

GpTaskScheduler::TaskSchedulerOptRefT   GpTaskScheduler::SCurrentScheduler (void) noexcept
{
    return sCurrentTaskScheduler;
}

void    GpTaskScheduler::SSetCurrentScheduler (TaskSchedulerOptRefT aValue) noexcept
{
    sCurrentTaskScheduler = aValue;
}

void    GpTaskScheduler::MoveToReady (GpTask::SP aTask)
{
    GpTask& task = aTask.V();

    bool needToWakeup = false;

    {
        std::scoped_lock lock_w(iLock);

        auto waitingIter = iWaitingTasks.find(aTask);

        if (waitingIter != iWaitingTasks.end())
        {
            iWaitingTasks.erase(waitingIter);

            GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);
            iReadyTasks.push(std::move(aTask));
            needToWakeup = true;
        }
    }

    if (needToWakeup)
    {
        iExecutorsPool.WakeupOne();
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)

/*
[GpTaskFiber::GpTaskFiber]: 'Proxy App (main service task)' counter = 1
[GpTaskFiber::GpTaskFiber]: 'db' counter = 2
[GpTaskFiber::GpTaskFiber]: 'http' counter = 3
[GpTaskFiber::GpTaskFiber]: '' counter = 4
[GpTaskFiber::GpTaskFiber]: 'EVA proxy HTTP server' counter = 5
[GpTaskFiber::~GpTaskFiber]: '' counter = 4
[GpTaskFiber::GpTaskFiber]: 'EVA proxy HTTP server: TCP server task' counter = 5
[GpTaskFiber::GpTaskFiber]: 'EVA proxy HTTP server: TCP server task: socket 6' counter = 6
[GpTaskFiber::GpTaskFiber]: 'EVA proxy HTTP server: TCP server task: socket 6: http socket task' counter = 7
[GpTaskFiber::GpTaskFiber]: 'postgresql: connection to DB' counter = 8
[GpTaskFiber::~GpTaskFiber]: 'postgresql: connection to DB' counter = 7
[GpTaskFiber::GpTaskFiber]: 'SQL async query' counter = 8
[GpTaskFiber::~GpTaskFiber]: 'SQL async query' counter = 7
[GpTaskFiber::GpTaskFiber]: 'SQL async query' counter = 8
[GpTaskFiber::~GpTaskFiber]: 'SQL async query' counter = 7
[GpTaskFiber::GpTaskFiber]: 'SQL async query' counter = 8
[GpTaskFiber::~GpTaskFiber]: 'SQL async query' counter = 7
[GpTaskFiber::GpTaskFiber]: 'SQL async query' counter = 8
[GpTaskFiber::~GpTaskFiber]: 'SQL async query' counter = 7
[GpTaskFiber::GpTaskFiber]: 'SQL async query' counter = 8
[GpTaskFiber::~GpTaskFiber]: 'SQL async query' counter = 7
[GpTaskFiber::~GpTaskFiber]: 'EVA proxy HTTP server: TCP server task: socket 6: http socket task' counter = 6
[GpTaskFiber::~GpTaskFiber]: 'EVA proxy HTTP server: TCP server task: socket 6' counter = 5
[GpTaskFiber::~GpTaskFiber]: 'Proxy App (main service task)' counter = 4
[GpTaskFiber::~GpTaskFiber]: 'EVA proxy HTTP server: TCP server task' counter = 3
[GpTaskFiber::~GpTaskFiber]: 'http' counter = 2
[GpTaskFiber::~GpTaskFiber]: 'EVA proxy HTTP server' counter = 1





[GpTaskFiber::GpTaskFiber]: 'db' counter = 2
*/


