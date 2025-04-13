#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>
#include <GpCore2/GpTasks/ITC/GpItcCondition.hpp>

namespace GPlatform {

class GpItcLockImpl
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcLockImpl)

public:
                    GpItcLockImpl   (void) noexcept = default;

    inline void     lock            (void);
    inline void     unlock          (void);
    inline bool     try_lock        (void);

private:
    std::atomic<bool>       iState = {false};
    mutable GpItcCondition  iItcCondition;
};

void    GpItcLockImpl::lock (void)
{
    for (;;)
    {
        if (!iState.exchange(true, std::memory_order_acquire))
        {
            return;
        }

        // Wait for unlock
        iItcCondition.Wait
        (
            [&]()
            {
                return iState.load(std::memory_order_relaxed) == false;
            }
        );
    }
}

void    GpItcLockImpl::unlock (void)
{
    iState.store(false, std::memory_order_release);

    {
        GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};
        iItcCondition.NotifyAll();
    }
}

bool    GpItcLockImpl::try_lock (void)
{
    return     (!iState.load(std::memory_order_relaxed))
            && (!iState.exchange(true, std::memory_order_acquire));
}

using GpItcLock = ThreadSafety::MutexWrap<GpItcLockImpl>;

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
