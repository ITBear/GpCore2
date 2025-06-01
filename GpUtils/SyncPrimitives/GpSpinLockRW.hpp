#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

#include <GpCore2/GpUtils/SyncPrimitives/GpAsmSpinPause.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp>
#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>

#include <atomic>

#if defined(TSAN_ENABLED)
#   include <sanitizer/tsan_interface.h>
#endif// #if defined(TSAN_ENABLED)

namespace GPlatform {

class GpSpinLockRwImpl
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSpinLockRwImpl)

public:
    inline          GpSpinLockRwImpl    (void) noexcept;
    inline          ~GpSpinLockRwImpl   (void) noexcept;

    inline void     lock_shared         (void) noexcept;
    inline void     unlock_shared       (void) noexcept;

    inline void     lock                (void) noexcept;
    inline void     unlock              (void) noexcept;

private:
    std::atomic_int32_t iLocksCounter = 0;  // 0 = unlocked, positive values = read count, -1 = write lock
};

GpSpinLockRwImpl::GpSpinLockRwImpl (void) noexcept
{
#if defined(TSAN_ENABLED)
    __tsan_mutex_create(this, __tsan_mutex_not_static);
#endif//#if defined(TSAN_ENABLED)
}

GpSpinLockRwImpl::~GpSpinLockRwImpl (void) noexcept
{
#if defined(TSAN_ENABLED)
    __tsan_mutex_destroy(this, __tsan_mutex_not_static);
#endif//#if defined(TSAN_ENABLED)
}

void    GpSpinLockRwImpl::lock_shared (void) noexcept
{
#if defined(TSAN_ENABLED)
    __tsan_mutex_pre_lock(this, __tsan_mutex_read_lock);
#endif//#if defined(TSAN_ENABLED)

    s_int_32 expected;

    do
    {
        // Wait for writers
        while ((expected = iLocksCounter.load(std::memory_order_relaxed)) < 0)
        {
            GP_ASM_SPIN_PAUSE();
        }
    } while (!iLocksCounter.compare_exchange_weak(expected, expected + 1, std::memory_order_acquire));

#if defined(TSAN_ENABLED)
    __tsan_mutex_post_lock(this, __tsan_mutex_read_lock, 0);
#endif//#if defined(TSAN_ENABLED)
}

void    GpSpinLockRwImpl::unlock_shared (void) noexcept
{
#if defined(TSAN_ENABLED)
    __tsan_mutex_pre_unlock(this, __tsan_mutex_read_lock);
#endif//#if defined(TSAN_ENABLED)

    iLocksCounter.fetch_sub(1, std::memory_order_release);

#if defined(TSAN_ENABLED)
    __tsan_mutex_post_unlock(this, __tsan_mutex_read_lock);
#endif//#if defined(TSAN_ENABLED)
}

void    GpSpinLockRwImpl::lock (void) noexcept
{
#if defined(TSAN_ENABLED)
    __tsan_mutex_pre_lock(this, __tsan_mutex_try_lock);
#endif//#if defined(TSAN_ENABLED)

    s_int_32 expected = 0;

    while (!iLocksCounter.compare_exchange_weak(expected, -1, std::memory_order_acquire))
    {
        expected = 0;
    }

#if defined(TSAN_ENABLED)
    __tsan_mutex_post_lock(this, __tsan_mutex_try_lock, 0);
#endif//#if defined(TSAN_ENABLED)
}

void    GpSpinLockRwImpl::unlock (void) noexcept
{
#if defined(TSAN_ENABLED)
     __tsan_mutex_pre_unlock(this, __tsan_mutex_try_lock);
#endif//#if defined(TSAN_ENABLED)

     iLocksCounter.store(0, std::memory_order_release);

#if defined(TSAN_ENABLED)
     __tsan_mutex_post_unlock(this, __tsan_mutex_try_lock);
#endif//#if defined(TSAN_ENABLED)
}

using GpSpinLockRW = ThreadSafety::SharedMutexWrap<GpSpinLockRwImpl>;

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
