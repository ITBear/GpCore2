#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskExecutor.hpp"
#include "../GpUtils/SyncPrimitives/GpSpinlock.hpp"
#include "../GpUtils/Threads/GpThread.hpp"
#include "../GpUtils/Types/Bits/GpBitOps.hpp"

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
    using ExecutorsFreeFlagsT   = GpTasksSettings::bistset_type;

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
    ExecutorsFreeFlagsT         iExecutorsIdleFlags = std::numeric_limits<ExecutorsFreeFlagsT>::max();
};

void    GpTaskExecutorsPool::WakeupNextIdle (void)
{
    std::scoped_lock lock(iLock);

    const size_t id_plus_one = BitOps::LeastSignificantBit(iExecutorsIdleFlags);

    if (id_plus_one > 0)
    {
        if (id_plus_one < iCount)
        {
            iExecutors.at(id_plus_one - 1).V().CVF().WakeupOne();
        }
    }
}

void    GpTaskExecutorsPool::MarkAsBusy (const size_t aExecutorId)
{
    std::scoped_lock lock(iLock);

    iExecutorsIdleFlags = BitOps::Down_by_id(iExecutorsIdleFlags, aExecutorId);
}

void    GpTaskExecutorsPool::MarkAsIdle (const size_t aExecutorId)
{
    std::scoped_lock lock(iLock);
    iExecutorsIdleFlags = BitOps::Up_by_id(iExecutorsIdleFlags, aExecutorId);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
