#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../../GpUtils/Threads/GpRunnable.hpp"
#include "../../ITC/GpItcConsumer.hpp"
#include "../../ITC/GpItcSharedPromise.hpp"
#include "../../GpTask.hpp"
#include "../GpTaskScheduler.hpp"

namespace GPlatform {

class GpTaskExecutorV1 final: public GpRunnable
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskExecutorV1)
    CLASS_DD(GpTaskExecutorV1)

    using ConsumerT     = GpItcConsumer<GpTask::SP>;
    using DonePromiseT  = GpItcSharedPromise<ssize_t>;

public:
                        GpTaskExecutorV1    (const size_t       aId,
                                             ConsumerT&&        aTasksConsumer,
                                             GpTaskScheduler&   aTasksScheduler,
                                             DonePromiseT&&     aDonePromise) noexcept;
    virtual             ~GpTaskExecutorV1   (void) noexcept override final;

    size_t              Id                  (void) const noexcept {return iId;}
    virtual void        Run                 (std::atomic_flag& aStopRequest) noexcept override final;

private:
    const size_t        iId             = 0;
    ConsumerT           iTasksConsumer;
    GpTaskScheduler&    iTasksScheduler;
    DonePromiseT        iDonePromise;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
