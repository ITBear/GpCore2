#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>
#include <GpCore2/GpTasks/ITC/GpItcConditionVar.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>

namespace GPlatform {

/*
class GpItcRecursiveLockRwImpl
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcRecursiveLockRwImpl)

    using ReaderDepthsMapT = boost::container::small_flat_map<GpTaskId::value_type, u_int_8, 4>;

public:
                                GpItcRecursiveLockRwImpl    (void) noexcept = default;
                                ~GpItcRecursiveLockRwImpl   (void) noexcept = default;

    // Reader interface
    inline void                 lock_shared                 (void);
    inline void                 unlock_shared               (void);
    inline bool                 try_lock_shared             (void);

    // Writer interface
    inline void                 lock                        (void);
    inline void                 unlock                      (void);
    inline bool                 try_lock                    (void);

private:
    inline GpTaskId::value_type GetCurrentTaskId            (void) const noexcept;
    inline bool                 IsWriterOwner               (GpTaskId::value_type aTaskId) const noexcept;
    inline size_t               GetReaderDepth              (GpTaskId::value_type aTaskId) const;
    inline void                 IncrementReaderDepth        (GpTaskId::value_type aTaskId);
    inline void                 DecrementReaderDepth        (GpTaskId::value_type aTaskId);

private:
    GpItcCondition                      iItcCondition;
    std::atomic<s_int_32>               iLockState              = {0};

    // Writer ownership and recursion tracking
    std::atomic<GpTaskId::value_type>   iWriterTaskId           = {0};
    u_int_8                             iWriterRecursiveDepth   = {0};

    // Reader recursion tracking per task
    mutable GpSpinLockRW<>              iReaderDepthsLock;
    ReaderDepthsMapT                    iReaderDepths;
};

void GpItcRecursiveLockRwImpl::lock_shared (void)
{
    const GpTaskId::value_type currentTaskId = GetCurrentTaskId();

    // Check if current task is the writer (writer can acquire reader locks)
    if (IsWriterOwner(currentTaskId))
    {
        IncrementReaderDepth(currentTaskId);
        return;
    }

    // Check if current task already has reader locks (recursive reader)
    if (GetReaderDepth(currentTaskId) > 0)
    {
        // Recursive reader case - just increment counter and reader count
        s_int_32 expected;
        do
        {
            while ((expected = iLockState.load(std::memory_order_relaxed)) < 0)
            {
                // Wait for writer to finish
                iItcCondition.Wait
                (
                    [&]()
                    {
                        return iLockState.load(std::memory_order_acquire) >= 0;
                    }
                );
            }
        } while (!iLockState.compare_exchange_weak(expected, expected + 1, std::memory_order_acquire));

        IncrementReaderDepth(currentTaskId);
        return;
    }

    // New reader case
    s_int_32 expected;
    do
    {
        // Wait for writers to finish
        while ((expected = iLockState.load(std::memory_order_relaxed)) < 0)
        {
            iItcCondition.Wait
            (
                [&]()
                {
                    return iLockState.load(std::memory_order_acquire) >= 0;
                }
            );
        }
    } while (!iLockState.compare_exchange_weak(expected, expected + 1, std::memory_order_acquire));

    IncrementReaderDepth(currentTaskId);
}

void GpItcRecursiveLockRwImpl::unlock_shared (void)
{
    const GpTaskId::value_type currentTaskId = GetCurrentTaskId();

    // Decrement reader depth for this task
    DecrementReaderDepth(currentTaskId);

    // Only decrement global reader count if this task has no more reader locks
    if (GetReaderDepth(currentTaskId) == 0)
    {
        iLockState.fetch_sub(1, std::memory_order_release);

        // Notify waiting writers if no more readers
        if (iLockState.load(std::memory_order_relaxed) == 0)
        {
            GpUniqueLock uniqueLock{iItcCondition.SpinLock()};
            iItcCondition.NotifyAll();
        }
    }
}

bool GpItcRecursiveLockRwImpl::try_lock_shared (void)
{
    const GpTaskId::value_type currentTaskId = GetCurrentTaskId();

    // Writer can always acquire reader locks
    if (IsWriterOwner(currentTaskId)) {
        IncrementReaderDepth(currentTaskId);
        return true;
    }

    // Recursive reader case
    if (GetReaderDepth(currentTaskId) > 0) {
        s_int_32 expected = iLockState.load(std::memory_order_relaxed);
        if (expected < 0) return false; // Writer active

        if (iLockState.compare_exchange_strong(expected, expected + 1, std::memory_order_acquire)) {
            IncrementReaderDepth(currentTaskId);
            return true;
        }
        return false;
    }

    // New reader case
    s_int_32 expected = iLockState.load(std::memory_order_relaxed);
    if (expected < 0) return false; // Writer active

    if (iLockState.compare_exchange_strong(expected, expected + 1, std::memory_order_acquire)) {
        IncrementReaderDepth(currentTaskId);
        return true;
    }

    return false;
}

void GpItcRecursiveLockRwImpl::lock(void)
{
    const GpTaskId::value_type currentTaskId = GetCurrentTaskId();

    // Check for recursive writer lock
    if (IsWriterOwner(currentTaskId))
    {
        iWriterRecursiveDepth++;
        return;
    }

    // Acquire exclusive writer lock
    while (true)
    {
        // Try to acquire when completely unlocked
        s_int_32 expected = 0;

        if (iLockState.compare_exchange_strong(expected, -1, std::memory_order_acquire)) {
            // Successfully acquired writer lock
            iWriterTaskId.store(currentTaskId, std::memory_order_relaxed);
            iWriterRecursiveDepth = 1;
            return;
        }

        // Wait for all readers and writers to finish
        iItcCondition.Wait([&]() {
            return iLockState.load(std::memory_order_acquire) == 0;
        });
    }
}

void GpItcRecursiveLockRwImpl::unlock(void)
{
    // Assume RAII ensures only lock owner calls this
    iWriterRecursiveDepth--;

    if (iWriterRecursiveDepth > 0) {
        return; // Still recursively locked
    }

    // Release writer lock completely
    iWriterTaskId.store(0, std::memory_order_relaxed);
    iLockState.store(0, std::memory_order_release);

    {
        GpUniqueLock uniqueLock{iItcCondition.SpinLock()};
        iItcCondition.NotifyAll();
    }
}

bool GpItcRecursiveLockRwImpl::try_lock(void)
{
    const GpTaskId::value_type currentTaskId = GetCurrentTaskId();

    // Check for recursive writer lock
    if (IsWriterOwner(currentTaskId)) {
        iWriterRecursiveDepth++;
        return true;
    }

    // Try to acquire exclusive writer lock
    s_int_32 expected = 0;
    if (iLockState.compare_exchange_strong(expected, -1, std::memory_order_acquire)) {
        iWriterTaskId.store(currentTaskId, std::memory_order_relaxed);
        iWriterRecursiveDepth = 1;
        return true;
    }

    return false;
}

GpTaskId::value_type GpItcRecursiveLockRwImpl::GetCurrentTaskId() const noexcept
{
    return GpTask::SCurrentTask().value().get().TaskId().Value();
}

bool GpItcRecursiveLockRwImpl::IsWriterOwner (const GpTaskId::value_type aTaskId) const noexcept
{
    return iWriterTaskId.load(std::memory_order_release) == aTaskId;
}

size_t GpItcRecursiveLockRwImpl::GetReaderDepth(const GpTaskId::value_type aTskId) const
{
    GpSharedLock sharedLock{iReaderDepthsLock};

    auto it = iReaderDepths.find(aTskId);
    return (it != iReaderDepths.end()) ? it->second : 0;
}

void GpItcRecursiveLockRwImpl::IncrementReaderDepth (const GpTaskId::value_type aTskId)
{
    GpUniqueLock lock(iReaderDepthsLock);
    ++iReaderDepths[aTskId];
}

void GpItcRecursiveLockRwImpl::DecrementReaderDepth (const GpTaskId::value_type aTskId)
{
    GpUniqueLock lock(iReaderDepthsLock);
    auto it = iReaderDepths.find(aTskId);
    if (it != iReaderDepths.end()) {
        if (--it->second == 0) {
            iReaderDepths.erase(it);
        }
    }
}

template<ThreadSafety::LockTraceModeE LTM = ThreadSafety::LockTraceModeE::TRACE_ENABLED>
using GpItcRecursiveLockRW = ThreadSafety::SharedSyncPrimitiveWrap<GpItcRecursiveLockRwImpl, LTM>;

}// namespace GPlatform
*/

#endif// #if defined(GP_USE_MULTITHREADING)
