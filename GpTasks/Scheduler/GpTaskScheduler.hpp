#pragma once

#include "../GpTasks_global.hpp"
#include "../GpTaskEnums.hpp"
#include "../GpTask.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpAny.hpp>

namespace GPlatform {

class GpTaskSchedulerFactory;

class GP_TASKS_API GpTaskScheduler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskScheduler)
    CLASS_DD(GpTaskScheduler)
    TAG_SET(THREAD_SAFE)

    using StopServiceFnT = std::function<void()>;

public:
                                GpTaskScheduler         (StopServiceFnT aStopServiceFn) noexcept;
    virtual                     ~GpTaskScheduler        (void) noexcept = default;

    static GpTaskScheduler&     S                       (void) noexcept {return sInstance.Vn();}
    static void                 SStart                  (const GpTaskSchedulerFactory&  aFactory,
                                                         size_t                         aExecutorsCount,
                                                         size_t                         aTasksMaxCount,
                                                         StopServiceFnT                 aStopServiceFn);
    static void                 SStopAndClear           (void);

    void                        StopService             (void);

    size_t                      ExecutorsCount          (void) const noexcept {return iExecutorsCount;}
    size_t                      TasksMaxCount           (void) const noexcept {return iTasksMaxCount;}

    GpTask::DoneFutureT::SP     NewToReadyDepend        (GpSP<GpTask> aTaskSP);

    // Task wait/ready
    virtual void                NewToReady              (GpSP<GpTask> aTaskSP) = 0;
    virtual void                NewToWaiting            (GpSP<GpTask> aTaskSP) = 0;
    virtual void                MakeTaskReady           (GpTaskId   aTaskGuid) = 0;
    virtual void                MakeTaskReady           (GpTaskId   aTaskGuid,
                                                         GpAny      aMessage) = 0;

    // Task groups
    virtual void                MakeTasksReadyByGroupId (GpTaskGroupId  aGpTaskGroupId,
                                                         GpAny          aMessage) = 0;
    virtual bool                AddTaskToGroup          (GpTaskId       aTaskGuid,
                                                         GpTaskGroupId  aGpTaskGroupId) = 0;
    virtual bool                RemoveTaskFromGroup     (GpTaskId       aTaskGuid,
                                                         GpTaskGroupId  aGpTaskGroupId) = 0;

    GpTask::DoneFutureT::SP     RequestStop             (GpTask& aTask);
    virtual bool                Reschedule              (GpTaskRunRes::EnumT    aRunRes,
                                                         GpSP<GpTask>&&         aTaskSP) noexcept = 0;

protected:
    virtual void                Start                   (size_t aExecutorsCount,
                                                         size_t aTasksMaxCount);
    virtual void                RequestStopAndJoin      (void) noexcept = 0;

private:
    size_t                      iExecutorsCount = 0;
    size_t                      iTasksMaxCount  = 0;
    StopServiceFnT              iStopServiceFn;

    static GpTaskScheduler::SP  sInstance;
};

}// namespace GPlatform
