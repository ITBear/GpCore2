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
    RequestStop();
    Join();

    iExecutorsPool.Clear();

    {
        std::scoped_lock lock(iReadyLock);
        while (!iReadyTasks.empty())
        {
            iReadyTasks.front().Vn().Terminate();
            iReadyTasks.pop();
        }
    }

    {
        std::scoped_lock lock(iWaitingLock);

        for (auto& task: iWaitingTasks)
        {
            const_cast<GpTask&>(task.VCn()).Terminate();
        }

        iWaitingTasks.clear();
    }   
}

void    GpTaskScheduler::Start (GpTaskScheduler::WP aSelfWP,
                                const count_t       aExecutorsCount)
{
    THROW_GPE_COND_CHECK_M(aExecutorsCount >= 1_cnt, "Executors count must be >= 1"_sv);

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

GpTask::SP  GpTaskScheduler::Reshedule (GpTask::SP          aLastTask,
                                        const GpTask::ResT  aLastTaskExecRes) noexcept
{
    // Process last task result
    if (aLastTaskExecRes == GpTask::ResT::READY_TO_EXEC)
    {
        std::scoped_lock lock(iReadyLock);

        if (!iReadyTasks.empty())
        {
            GpTaskAccessor::SUpdateState(aLastTask.Vn(), GpTask::StateTE::READY_TO_RUN);
            iReadyTasks.push(std::move(aLastTask));
        } else
        {
            return aLastTask;
        }
    } else if (aLastTaskExecRes == GpTask::ResT::WAITING)
    {
        std::scoped_lock lock_w(iWaitingLock);

        GpTask& task = aLastTask.Vn();

        // State can be: RUNNING (task just came from executor), READY_TO_RUN (task was moved to READY_TO_RUN from GpTaskScheduler::MoveToReady)
        if (GpTaskAccessor::SState(task) == GpTask::StateTE::RUNNING)
        {
            if (task.HasEvents())
            {
                if (!iReadyTasks.empty())
                {
                    GpTaskAccessor::SUpdateState(aLastTask.Vn(), GpTask::StateTE::READY_TO_RUN);
                    iReadyTasks.push(std::move(aLastTask));
                } else
                {
                    return aLastTask;
                }
            } else
            {
                GpTaskAccessor::SUpdateState(aLastTask.Vn(), GpTask::StateTE::WAITING);
                iWaitingTasks.emplace(std::move(aLastTask));
            }
        } else//READY_TO_RUN
        {
            std::scoped_lock lock_r(iReadyLock);

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
        if (aLastTask.IsNotNULL())
        {
            std::scoped_lock lock(iReadyLock);
            GpTaskAccessor::SUpdateState(aLastTask.Vn(), GpTask::StateTE::FINISHED);
        }
    }

    // Get next ready task
    {
        std::scoped_lock lock(iReadyLock);

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
        std::scoped_lock lock(iReadyLock);

        GpTask& task = aTask.V();

        THROW_GPE_COND_CHECK_M(GpTaskAccessor::SState(task) == GpTask::StateTE::NOT_ASSIGNED_TO_SCHEDULER,
                               "Task already assigned to scheduler"_sv);

        GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);
        GpTaskAccessor::SSetScheduler(task, *this);
        GpTaskAccessor::SSetWeakPtr(task, aTask);

        iReadyTasks.push(std::move(aTask));
    }

    iExecutorsPool.WakeupAll();
}

void    GpTaskScheduler::AddTaskToWaiting (GpTask::SP aTask)
{
    std::scoped_lock lock(iWaitingLock);

    GpTask& task = aTask.V();

    THROW_GPE_COND_CHECK_M(GpTaskAccessor::SState(task) == GpTask::StateTE::NOT_ASSIGNED_TO_SCHEDULER,
                           "Task already assigned to scheduler"_sv);

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
        std::scoped_lock lock_w(iWaitingLock);

        auto waitingIter = iWaitingTasks.find(aTask);

        if (waitingIter != iWaitingTasks.end())
        {
            iWaitingTasks.erase(waitingIter);

            std::scoped_lock lock_r(iReadyLock);

            GpTaskAccessor::SUpdateState(task, GpTask::StateTE::READY_TO_RUN);
            iReadyTasks.push(std::move(aTask));
            needToWakeup = true;
        }
    }

    if (needToWakeup)
    {
        iExecutorsPool.WakeupAll();
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
