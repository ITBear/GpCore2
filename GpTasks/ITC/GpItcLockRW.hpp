#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>
#include <GpCore2/GpTasks/ITC/GpItcCondition.hpp>

namespace GPlatform {

class GpItcLockRwImpl
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcLockRwImpl)

public:
                    GpItcLockRwImpl (void) noexcept = default;

    inline void     lock_shared     (void) noexcept;
    inline void     unlock_shared   (void) noexcept;

    inline void     lock            (void) noexcept;
    inline void     unlock          (void) noexcept;

private:
    std::atomic_int32_t     iLocksCounter = 0;  // 0 = unlocked, positive values = read count, -1 = write lock
    mutable GpItcCondition  iItcCondition;
};

void    GpItcLockRwImpl::lock_shared (void) noexcept
{
    s_int_32 expected;

    do
    {
        if (expected = iLocksCounter.load(std::memory_order_relaxed); expected < 0) // Lock in write mode
        {
            // Wait for writer
            iItcCondition.Wait
            (
                [&]()
                {
                    return (expected = iLocksCounter.load(std::memory_order_relaxed)) >= 0;
                }
            );
        }
    } while (!iLocksCounter.compare_exchange_weak(expected, expected + 1, std::memory_order_acquire));
}

void    GpItcLockRwImpl::unlock_shared (void) noexcept
{
    const auto counter = iLocksCounter.fetch_sub(1, std::memory_order_release);

    if (counter == 0)
    {
        GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};
        iItcCondition.NotifyAll();
    }
}

void    GpItcLockRwImpl::lock (void) noexcept
{
    s_int_32 expected = 0;
    if (!iLocksCounter.compare_exchange_weak(expected, -1, std::memory_order_acquire))
    {
        // Wait for readers/writer
        iItcCondition.Wait
        (
            [this]()
            {
                s_int_32 expected = 0;
                return iLocksCounter.compare_exchange_weak(expected, -1, std::memory_order_acquire);
            }
        );
    }
}

void    GpItcLockRwImpl::unlock (void) noexcept
{
     iLocksCounter.store(0, std::memory_order_release);

     {
         GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};
         iItcCondition.NotifyAll();
     }
}

using GpItcLockRW = ThreadSafety::SharedMutexWrap<GpItcLockRwImpl>;

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
