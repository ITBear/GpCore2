#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

#include <atomic>
#include <GpCore2/GpUtils/SyncPrimitives/GpAsmSpinPause.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp>
#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>

namespace GPlatform {

class GpSpinLockRwImpl
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSpinLockRwImpl)

public:
    GpSpinLockRwImpl (void) noexcept = default;

    void    lock_shared (void) noexcept
    {
        s_int_32 expected;

        do
        {
            // Wait for writers
            while ((expected = iLocksCounter.load(std::memory_order_relaxed)) < 0)
            {
                GP_ASM_SPIN_PAUSE();
            }
        } while (!iLocksCounter.compare_exchange_weak(expected, expected + 1, std::memory_order_acquire));
    }

    void    unlock_shared (void) noexcept
    {
        iLocksCounter.fetch_sub(1, std::memory_order_release);
    }

    void    lock (void) noexcept
    {
        s_int_32 expected = 0;

        while (!iLocksCounter.compare_exchange_weak(expected, -1, std::memory_order_acquire))
        {
            expected = 0;
        }
    }

    void    unlock (void) noexcept
    {
         iLocksCounter.store(0, std::memory_order_release);
    }

private:
    std::atomic_int32_t iLocksCounter = 0;  // 0 = unlocked, positive values = read count, -1 = write lock
};

using GpSpinLockRW = ThreadSafety::SharedMutexWrap<GpSpinLockRwImpl>;

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
