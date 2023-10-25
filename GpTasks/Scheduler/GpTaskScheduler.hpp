#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../GpUtils/Macro/GpMacroTags.hpp"
#include "../../GpUtils/Macro/GpMacroClass.hpp"
#include "../../GpUtils/Types/Containers/GpContainersT.hpp"
#include "../../GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp"
#include "../../GpUtils/Types/Containers/GpAny.hpp"

#include "../GpTaskEnums.hpp"
#include "../GpTask.hpp"

namespace GPlatform {

class GpTaskSchedulerFactory;

class GP_TASKS_API GpTaskScheduler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskScheduler)
    CLASS_DD(GpTaskScheduler)
    TAG_SET(THREAD_SAFE)

    using TaskOptRefT = std::optional<std::reference_wrapper<GpTask>>;

public:
                                    GpTaskScheduler     (void) noexcept = default;
    virtual                         ~GpTaskScheduler    (void) noexcept = default;

    static GpTaskScheduler&         S                   (void) noexcept {return sInstance.Vn();}
    static void                     SStart              (const GpTaskSchedulerFactory&  aFactory,
                                                         const size_t                   aExecutorsCount,
                                                         const size_t                   aTasksMaxCount);
    static void                     SStopAndClear       (void);

    size_t                          ExecutorsCount      (void) const noexcept {return iExecutorsCount;}
    size_t                          TasksMaxCount       (void) const noexcept {return iTasksMaxCount;}

    inline GpTask::DoneFutureT::SP  NewToReadyDepend    (GpSP<GpTask> aTask);

    virtual void                    NewToReady          (GpSP<GpTask> aTask) = 0;
    virtual void                    NewToWaiting        (GpSP<GpTask> aTask) = 0;
    virtual void                    MakeTaskReady       (const GpTask::IdT  aTaskGuid) = 0;
    virtual void                    MakeTaskReady       (const GpTask::IdT  aTaskGuid,
                                                         GpAny              aPayload) = 0;

    virtual bool                    Reschedule          (const GpTaskRunRes::EnumT  aRunRes,
                                                         GpSP<GpTask>&&             aTask) noexcept = 0;

protected:
    virtual void                    Start               (const size_t aExecutorsCount,
                                                         const size_t aTasksMaxCount);
    virtual void                    RequestStopAndJoin  (void) noexcept = 0;

private:
    size_t                          iExecutorsCount = 0;
    size_t                          iTasksMaxCount  = 0;

    static GpTaskScheduler::SP      sInstance;
};

GpTask::DoneFutureT::SP GpTaskScheduler::NewToReadyDepend (GpSP<GpTask> aTask)
{
    GpTask::DoneFutureT::SP doneFuture = aTask->GetDoneFuture();
    NewToReady(aTask);
    return doneFuture;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
