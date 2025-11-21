#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>
#include <GpCore2/GpTasks/ITC/GpItcConditionVar.hpp>

namespace GPlatform {

class GpItcLockImpl
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcLockImpl)

public:
    inline          GpItcLockImpl   (void) noexcept;

    inline void     lock            (void);
    inline void     unlock          (void);
    inline bool     try_lock        (void);

private:
    GpItcConditionVar   iItcCv;
    std::atomic<bool>   iState = {false};
};

GpItcLockImpl::GpItcLockImpl (void) noexcept
{
}

void    GpItcLockImpl::lock (void)
{
    for (;;)
    {
        if (!iState.exchange(true, std::memory_order_acquire))
        {
            return;
        }

        // Wait for unlock
        iItcCv.Wait
        (
            [&]()
            {
                return iState.load(std::memory_order_acquire) == false;
            }
        );
    }
}

void    GpItcLockImpl::unlock (void)
{
    iState.store(false, std::memory_order_release);

    {
        GpUniqueLock uniqueLock{iItcCv.SpinLockRW()};
        iItcCv.NotifyAll();
    }
}

bool    GpItcLockImpl::try_lock (void)
{
    bool expected = false;
    return iState.compare_exchange_strong(expected, true, std::memory_order_acq_rel, std::memory_order_acquire);
}

template<ThreadSafety::LockTraceModeE LTM = ThreadSafety::LockTraceModeE::TRACE_ENABLED>
using GpItcLock = ThreadSafety::SyncPrimitiveWrap<GpItcLockImpl, LTM>;

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
