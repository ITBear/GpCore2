#include "GpItcSharedCondition.hpp"

#include "../Scheduler/GpTaskScheduler.hpp"
#include "../Fibers/GpTaskFiber.hpp"
#include "../GpTask.hpp"

namespace GPlatform {

void    GpItcSharedCondition::NotifyOne (void)
{
    // Notify fiber task
    if (iFiberTaskIDs.size() > 0)
    {
        GpTaskScheduler::S().MakeTaskReady(*iFiberTaskIDs.begin());
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
    for (const GpTaskId taskId: iFiberTaskIDs)
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

        taskMode    = task.Mode();
        taskId      = task.Id();
    }

    return {taskMode, taskId};
}

void    GpItcSharedCondition::SYeld (const milliseconds_t aTimeout)
{
    YELD_WAIT(aTimeout);
}

}//namespace GPlatform
