#include <GpCore2/GpTasks/ITC/GpItcSharedCondition.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>

namespace GPlatform {

void    GpItcSharedCondition::NotifyOne (void)
{
    // Notify fiber task
    if (!iFiberTaskIDs.empty())
    {
        GpTaskScheduler::S().MakeTaskReady(*std::begin(iFiberTaskIDs));
    }

    // Notify thread
    if (iThreadsWaiting > 0)
    {
        iThreadsCV.NotifyOne();
    }
}

void    GpItcSharedCondition::NotifyAll (void)
{
    // Notify fiber tasks
    for (const GpTaskId& taskId: iFiberTaskIDs)
    {
        GpTaskScheduler::S().MakeTaskReady(taskId);
    }

    // Notify threads
    if (iThreadsWaiting > 0)
    {
        iThreadsCV.NotifyAll();
    }
}

GpItcSharedCondition::TaskInfo  GpItcSharedCondition::SCurrentTaskInfo (void)
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

GpTaskFiberCtx::TimeoutRes  GpItcSharedCondition::SYeld (const milliseconds_t aTimeout)
{
    return YELD_WAIT(aTimeout);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
