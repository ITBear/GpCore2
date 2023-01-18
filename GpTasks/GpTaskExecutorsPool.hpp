#pragma once

#include "GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <bitset>
#include "GpTaskExecutor.hpp"

namespace GPlatform {

class GpTask;

class GP_TASKS_API GpTaskExecutorsPool
{
    friend class GpTaskExecutor;

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskExecutorsPool)
    CLASS_DD(GpTaskExecutorsPool)

    using ExecutorT             = GpTaskExecutor;
    using ExecutorsContainerT   = std::array<ExecutorT::SP, GpTasksSettings::SMaxCoresCount()>;
    using ExecutorsFreeFlagsT   = std::bitset<GpTasksSettings::SMaxCoresCount()>;

public:
                                GpTaskExecutorsPool     (void) noexcept = default;
                                ~GpTaskExecutorsPool    (void) noexcept = default;

    void                        Start                   (const size_t aCount);
    void                        RequestStop             (void) noexcept;
    void                        Join                    (void) noexcept;
    inline void                 WakeupNextIdle          (void);

private:
    inline void                 MarkAsBusy              (const size_t aExecutorId);
    inline void                 MarkAsIdle              (const size_t aExecutorId);

private:
    mutable GpSpinlock          iLock;
    size_t                      iCount = 0;
    GpThread::C::Vec::Val       iThreads;
    ExecutorsContainerT         iExecutors;
    ExecutorsFreeFlagsT         iExecutorsIdleFlags;
};

void    GpTaskExecutorsPool::WakeupNextIdle (void)
{
    std::scoped_lock lock(iLock);

    const size_t id = iExecutorsIdleFlags._Find_first();

    if (id < iCount)
    {
        iExecutors.at(id).V().CVF().WakeupOne();
    }
}

void    GpTaskExecutorsPool::MarkAsBusy (const size_t aExecutorId)
{
    std::scoped_lock lock(iLock);
    iExecutorsIdleFlags.reset(aExecutorId);
}

void    GpTaskExecutorsPool::MarkAsIdle (const size_t aExecutorId)
{
    std::scoped_lock lock(iLock);
    iExecutorsIdleFlags.set(aExecutorId);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
