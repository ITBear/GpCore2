#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>
#include <GpCore2/GpTasks/ITC/GpItcCondition.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>

namespace GPlatform {

class GpItcRecursiveLockImpl
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcRecursiveLockImpl)

public:
                                        GpItcRecursiveLockImpl  (void) noexcept = default;

    inline void                         lock                    (void);
    inline void                         unlock                  (void);
    inline bool                         try_lock                (void);

private:
    mutable GpItcCondition              iItcCondition;
    std::atomic<GpTaskId::value_type>   iLockTaskId     = {0};
    size_t                              iRecursiveDepth = {0};
};

void    GpItcRecursiveLockImpl::lock (void)
{
    const GpTaskId::value_type currentTaskIdToLock = GpTask::SCurrentTask().value().get().TaskId().Value();

    while(true)
    {
        GpTaskId::value_type currentLockTaskId = iLockTaskId.load(std::memory_order_relaxed);

        if (currentLockTaskId == currentTaskIdToLock) // Recursive call (already locked)
        {
            iRecursiveDepth++;
            return;
        }

        if (currentLockTaskId == 0) // No lock
        {
            if (iLockTaskId.compare_exchange_strong(currentLockTaskId, currentTaskIdToLock, std::memory_order_acquire))
            {
                // Successfully locked
                iRecursiveDepth = 1;
                return;
            }
        }

        // Wait for unlock
        iItcCondition.Wait
        (
            [&]()
            {
                return iLockTaskId.load(std::memory_order_relaxed) == 0;
            }
        );
    }
}

void    GpItcRecursiveLockImpl::unlock (void)
{
    iRecursiveDepth--;

    if (iRecursiveDepth > 0)
    {
        return;
    }

    iLockTaskId.store(0, std::memory_order_release);

    {
        GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};
        iItcCondition.NotifyAll();
    }
}

bool    GpItcRecursiveLockImpl::try_lock (void)
{
    const GpTaskId::value_type  currentTaskIdToLock = GpTask::SCurrentTask().value().get().TaskId().Value();
    GpTaskId::value_type        currentLockTaskId   = iLockTaskId.load(std::memory_order_relaxed);

    if (currentLockTaskId == currentTaskIdToLock) // Recursive call (already locked)
    {
        iRecursiveDepth++;
        return true;
    }

    if (currentLockTaskId == 0) // No lock
    {
        if (iLockTaskId.compare_exchange_strong(currentLockTaskId, currentTaskIdToLock, std::memory_order_acquire))
        {
            // Successfully locked
            iRecursiveDepth = 1;
            return true;
        }
    }

    return false;
}

using GpItcRecursiveLock = ThreadSafety::MutexWrap<GpItcRecursiveLockImpl>;

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
