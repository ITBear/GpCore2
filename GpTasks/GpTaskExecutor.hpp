#pragma once

#include "GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GP_TASKS_API GpTaskExecutor final: public GpRunnable
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskExecutor)
    CLASS_DD(GpTaskExecutor)

public:
    inline                  GpTaskExecutor  (GpConditionVar::SP aCondVar) noexcept;
    virtual                 ~GpTaskExecutor (void) noexcept override final = default;

    virtual void            Run             (GpThreadStopToken aStopToken) noexcept override final;
};

GpTaskExecutor::GpTaskExecutor (GpConditionVar::SP aCondVar) noexcept:
GpRunnable(std::move(aCondVar))
{
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
