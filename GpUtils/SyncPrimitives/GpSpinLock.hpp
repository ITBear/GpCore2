#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SYNC_PRIMITIVES)

#include "../Macro/GpMacroClass.hpp"
#include "../Threads/GpThreadsSafety.hpp"
#include "GpAsmSpinPause.hpp"
#include <atomic>

namespace GPlatform{

class GpSpinLockImpl
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSpinLockImpl)

public:
                        GpSpinLockImpl  (void) noexcept = default;
                        ~GpSpinLockImpl (void) noexcept = default;

    inline void         lock            (void) noexcept;
    inline void         unlock          (void) noexcept;
    inline bool         try_lock        (void) noexcept;

private:
    std::atomic<bool>   iState = {0};
};

void    GpSpinLockImpl::lock (void) noexcept
{
    for (;;)
    {
        if (!iState.exchange(true, std::memory_order_acquire))
        {
            return;
        }

        while (iState.load(std::memory_order_relaxed))
        {
            GP_ASM_SPIN_PAUSE();
        }
    }
}

void    GpSpinLockImpl::unlock (void) noexcept
{
    iState.store(false, std::memory_order_release);
}

bool    GpSpinLockImpl::try_lock (void) noexcept
{
    return     (!iState.load(std::memory_order_relaxed))
            && (!iState.exchange(true, std::memory_order_acquire));
}

using GpSpinLock = ThreadSafety::MutexWrap<GpSpinLockImpl>;

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
