#include <GpCore2/GpTasks/ITC/GpItcCondition.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>

namespace GPlatform {

bool    GpItcCondition::NotifyOne (void)
{
    bool res = true;

    // Notify fiber task
    if (!iFiberTaskIDs.empty())
    {
        res &= GpTaskScheduler::S().MakeTaskReady
        (
            *std::begin(iFiberTaskIDs)
        );
    }

    // Notify thread
    if (iThreadsWaiting > 0)
    {
        iThreadsCV.NotifyOne();
    }

    return res;
}

bool    GpItcCondition::NotifyAll (void)
{
    bool res = true;

    // Notify fiber tasks
    for (const GpTaskId& taskId: iFiberTaskIDs)
    {
        res &= GpTaskScheduler::S().MakeTaskReady(taskId);
    }

    // Notify threads
    if (iThreadsWaiting > 0)
    {
        iThreadsCV.NotifyAll();
    }

    return res;
}

GpItcCondition::TaskInfo    GpItcCondition::SCurrentTaskInfo (void)
{
    // Collect current task info
    std::optional<std::reference_wrapper<GpTask>>   taskOptRef  = GpTask::SCurrentTask();
    GpTaskMode::EnumT                               taskMode    = GpTaskMode::THREAD;
    GpTaskId                                        taskId      = {};

    if (taskOptRef.has_value()) [[likely]]
    {
        const GpTask& task = taskOptRef.value().get();

        taskMode    = task.TaskMode();
        taskId      = task.TaskId();
    }

    return {taskMode, taskId};
}

void    GpItcCondition::SYield (const milliseconds_t aTimeout)
{
    std::ignore = YIELD_WAIT(aTimeout);
}

void    GpItcCondition::SYield (void)
{
    YIELD_WAIT();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
