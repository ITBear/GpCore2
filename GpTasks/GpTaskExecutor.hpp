#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTasks_global.hpp"
#include "../GpUtils/Threads/GpRunnable.hpp"

namespace GPlatform {

class GpTaskExecutorsPool;

class GP_TASKS_API GpTaskExecutor final: public GpRunnable
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskExecutor)
    CLASS_DD(GpTaskExecutor)

public:
    inline                  GpTaskExecutor  (const size_t           aId,
                                             GpTaskExecutorsPool&   aExecutorsPool) noexcept;
    virtual                 ~GpTaskExecutor (void) noexcept override final = default;

    virtual void            Run             (GpThreadStopToken aStopToken) noexcept override final;

private:
    const size_t            iId;
    GpTaskExecutorsPool&    iExecutorsPool;
};

GpTaskExecutor::GpTaskExecutor
(
    const size_t            aId,
    GpTaskExecutorsPool&    aExecutorsPool
) noexcept:
iId(aId),
iExecutorsPool(aExecutorsPool)
{
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
