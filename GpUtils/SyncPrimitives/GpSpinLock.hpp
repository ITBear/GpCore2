#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpAsmSpinPause.hpp>

#include <atomic>

#if defined(TSAN_ENABLED)
#   include <sanitizer/tsan_interface.h>
#endif// #if defined(TSAN_ENABLED)

namespace GPlatform {

class GpSpinLockImpl
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSpinLockImpl)

public:
    inline          GpSpinLockImpl  (void) noexcept;
    inline          ~GpSpinLockImpl (void) noexcept;

    inline void     lock            (void) noexcept;
    inline void     unlock          (void) noexcept;
    inline bool     try_lock        (void) noexcept;

private:
    std::atomic<bool>   iState = {0};
};

GpSpinLockImpl::GpSpinLockImpl (void) noexcept
{
#if defined(TSAN_ENABLED)
    __tsan_mutex_create(this, __tsan_mutex_not_static);
#endif//#if defined(TSAN_ENABLED)
}

GpSpinLockImpl::~GpSpinLockImpl (void) noexcept
{
#if defined(TSAN_ENABLED)
    __tsan_mutex_destroy(this, __tsan_mutex_not_static);
#endif//#if defined(TSAN_ENABLED)
}

void    GpSpinLockImpl::lock (void) noexcept
{
#if defined(TSAN_ENABLED)
    __tsan_mutex_pre_lock(this, __tsan_mutex_try_lock);
#endif//#if defined(TSAN_ENABLED)

    for (;;)
    {
        if (!iState.exchange(true, std::memory_order_acquire))
        {
#if defined(TSAN_ENABLED)
            __tsan_mutex_post_lock(this, __tsan_mutex_try_lock, 0);
#endif//#if defined(TSAN_ENABLED)

            return;         
        }

#if defined(TSAN_ENABLED)
        __tsan_mutex_post_lock(this, __tsan_mutex_try_lock_failed, 0);
#endif//#if defined(TSAN_ENABLED)

        while (iState.load(std::memory_order_relaxed))
        {
            GP_ASM_SPIN_PAUSE();
        }
    }
}

void    GpSpinLockImpl::unlock (void) noexcept
{
#if defined(TSAN_ENABLED)
     __tsan_mutex_pre_unlock(this, __tsan_mutex_try_lock);
#endif//#if defined(TSAN_ENABLED)

    iState.store(false, std::memory_order_release);

#if defined(TSAN_ENABLED)
     __tsan_mutex_post_unlock(this, __tsan_mutex_try_lock);
#endif//#if defined(TSAN_ENABLED)
}

bool    GpSpinLockImpl::try_lock (void) noexcept
{
#if defined(TSAN_ENABLED)
    __tsan_mutex_pre_lock(this, __tsan_mutex_try_lock);
#endif//#if defined(TSAN_ENABLED)

    const bool isLocked = (!iState.load(std::memory_order_relaxed)) && (!iState.exchange(true, std::memory_order_acquire));

#if defined(TSAN_ENABLED)
    if (isLocked)
    {
        __tsan_mutex_post_lock(this, __tsan_mutex_try_lock, 0);
    } else
    {
        __tsan_mutex_post_lock(this, __tsan_mutex_try_lock_failed, 0);
    }
#endif//#if defined(TSAN_ENABLED)

    return isLocked;
}

using GpSpinLock = ThreadSafety::MutexWrap<GpSpinLockImpl>;

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
