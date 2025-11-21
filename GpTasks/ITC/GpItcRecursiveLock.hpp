#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>
#include <GpCore2/GpTasks/ITC/GpItcConditionVar.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLock.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSyncPrimitives.hpp>

namespace GPlatform {

class GpItcRecursiveLockImpl
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcRecursiveLockImpl)

public:
                                GpItcRecursiveLockImpl  (void) noexcept = default;

    inline void                 lock                    (void);
    inline void                 unlock                  (void);
    inline bool                 try_lock                (void);

private:
    inline GpTaskId::value_type GetCurrentTaskId        (void) const noexcept;

private:
    GpItcConditionVar                   iItcCV;
    u_int_32                            iRecursiveDepth = {0};
    std::atomic<GpTaskId::value_type>   iLockTaskId     = {0};
};

void    GpItcRecursiveLockImpl::lock (void)
{
    const GpTaskId::value_type currentTaskId = GetCurrentTaskId();

    while (true)
    {
        GpTaskId::value_type expectedTaskId = currentTaskId;

        // Try recursive increment under lock ownership
        if (iLockTaskId.compare_exchange_strong(expectedTaskId, currentTaskId, std::memory_order_acq_rel, std::memory_order_acquire))
        {
            // We already owned it - this was a recursive call
            iRecursiveDepth++;
            return;
        }

        expectedTaskId = 0;
        if (iLockTaskId.compare_exchange_strong(expectedTaskId, currentTaskId, std::memory_order_acq_rel, std::memory_order_acquire))
        {
            // Successfully acquired free lock
            iRecursiveDepth = 1;
            return;
        }

        // Wait for release
        iItcCV.Wait
        (
            [&]()
            {
                return iLockTaskId.load(std::memory_order_acquire) == 0;
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
        GpUniqueLock uniqueLock{iItcCV.SpinLockRW()};
        iItcCV.NotifyAll();
    }
}

bool    GpItcRecursiveLockImpl::try_lock (void)
{
    const GpTaskId::value_type currentTaskId = GetCurrentTaskId();

    // Try recursive increment first
    GpTaskId::value_type expectedTaskId = currentTaskId;
    if (iLockTaskId.compare_exchange_strong(expectedTaskId, currentTaskId, std::memory_order_acq_rel, std::memory_order_acquire))
    {
        // We already owned it - this was a recursive call
        iRecursiveDepth++;
        return true;
    }

    // Try to acquire free lock
    expectedTaskId = 0;
    if (iLockTaskId.compare_exchange_strong(expectedTaskId, currentTaskId, std::memory_order_acq_rel, std::memory_order_acquire))
    {
        // Successfully acquired free lock
        iRecursiveDepth = 1;
        return true;
    }

    // Lock is held by another task
    return false;
}

GpTaskId::value_type    GpItcRecursiveLockImpl::GetCurrentTaskId (void) const noexcept
{
    return GpTask::SCurrentTask()->TaskId().Value();
}

template<ThreadSafety::LockTraceModeE LTM = ThreadSafety::LockTraceModeE::TRACE_ENABLED>
using GpItcRecursiveLock = ThreadSafety::SyncPrimitiveWrap<GpItcRecursiveLockImpl, LTM>;

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
