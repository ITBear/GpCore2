#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../Threads/GpRunnable.hpp"
#include "GpTask.hpp"

namespace GPlatform {

class GpTaskScheduler;

class GPCORE_API GpTaskExecutor final: public GpRunnable
{
public:
    CLASS_REMOVE_CTRS(GpTaskExecutor);
    CLASS_DECLARE_DEFAULTS(GpTaskExecutor);

public:
                        GpTaskExecutor  (GpTaskScheduler&   aScheduler,
                                         GpConditionVar::SP aCondVar) noexcept;
    virtual             ~GpTaskExecutor (void) noexcept override final;

    virtual void        Run             (GpThreadStopToken aStopToken) noexcept override final;

private:
    GpTaskScheduler&    iScheduler;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
