#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpSharedQueueMPMC.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskExecutor.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpTaskSchedulerV1;

class GpTaskExecutorV1 final: public GpTaskExecutor
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskExecutorV1)
    CLASS_DD(GpTaskExecutorV1)
    TAG_SET(THREAD_SAFE)

    using TaskQueueT    = GpSharedQueueMPMC<GpTask::SP>;
    using DonePromiseT  = GpItcPromise<ssize_t>;

public:
                    GpTaskExecutorV1    (size_t             aId,
                                         GpTaskSchedulerV1& aScheduler,
                                         TaskQueueT::SP     aTaskQueueDataSP,
                                         DonePromiseT&&     aDonePromise) noexcept;
    virtual         ~GpTaskExecutorV1   (void) noexcept override final;

    size_t          Id                  (void) const noexcept {return iId;}
    virtual void    Run                 (GpConditionVarFlag& aStopFlag) noexcept override final;

private:
    const size_t        iId = 0;
    GpTaskSchedulerV1&  iScheduler;
    TaskQueueT::SP      iTaskQueueSP;
    DonePromiseT        iDonePromise;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
